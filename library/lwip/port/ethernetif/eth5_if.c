/************************************************************************************
 *
 * 	Дравер микросхемы ethernet eth 5600ВГ1У
 *  	Copyright (c) rauschenbach 
 *      sokareis@mail.ru
 *
 ***********************************************************************************/
#include "opt.h"
#include "def.h"
#include "mem.h"
#include "pbuf.h"
#include <stats.h>
#include <snmp.h>
#include "etharp.h"
#include "ppp_oe.h"
#include "main.h"
#include <string.h>
#include "eth_common.h"

/* Зачем это? все равно не будем искать */
#define IFNAME0 'e'
#define IFNAME1 '5'

/* MAC address */
#define 	FULL_MAC		"Алупка"


/*  Базовый адрес 5-го контроллера Ethernet */
#define	EXT_ETH_BASE_ADDR                ETH5_BASE_ADDR
#define	MDR_EXT_ETH_RX_BUF_BASE		(EXT_ETH_BASE_ADDR)
#define	MDR_EXT_ETH_DSC_RX_TBL_BASE	(EXT_ETH_BASE_ADDR + 4 * 0x0800)
#define	MDR_EXT_ETH_TX_BUF_BASE		(EXT_ETH_BASE_ADDR + 4 * 0x1000)
#define	MDR_EXT_ETH_DSC_TX_TBL_BASE	(EXT_ETH_BASE_ADDR + 4 * 0x1800)
#define	MDR_EXT_ETH_REGS		(EXT_ETH_BASE_ADDR + 4 * 0x1FC0)
#define	MDR_EXT_ETH_DSC_RX_TBL		((MDR_EXT_ETH_DSC_RX_TBL_TypeDef *)MDR_EXT_ETH_DSC_RX_TBL_BASE)
#define	MDR_EXT_ETH_DSC_TX_TBL		((MDR_EXT_ETH_DSC_TX_TBL_TypeDef *)MDR_EXT_ETH_DSC_TX_TBL_BASE)
#define	MDR_EXT_ETH			((MDR_EXT_ETH_TypeDef *)MDR_EXT_ETH_REGS)
#define MDR_EXT_IRQ_NUM			EXT_INT3_IRQn



static struct netif *s_pxNetIf @ ".fastdata" = NULL;
static xSemaphoreHandle s_xSemaphore @ ".fastdata" = NULL;	/* Семафор для ожидающей задачи приема */
static u8 RxDescNum @ ".fastdata" = 0;
static u8 TxDescNum @ ".fastdata" = 0;
static u16 *RxDst @ ".fastdata" = NULL;
static u16 *TxSrc @ ".fastdata" = NULL;
static u32 *RxSrc @ ".fastdata" = NULL;
static u32 *TxDst @ ".fastdata" = NULL;
static u32 *pTrBuf @ ".fastdata" = NULL;
static u16 rx_reg @ ".fastdata" = 0;	/* Биты регистров по чтению */

#pragma pack(4)
static unsigned char EthFr[ETH_FRAME_SIZE] @ ".fastdata" = { 0 };
static tEthFrame EthFrame @ ".fastdata" = {0};

/* Задача и функции сброса */
static void eth5_if_task(void *);
static void eth5_input(struct netif *);
static void eth5_prepare_tx_desc(void);
static void eth5_prepare_rx_desc(void);

/**
 * Очистка буферов приема/передачи и дескрипторов
 */
static void EthClrRAM(void)
{
    unsigned char i;

    memset((u32 *) MDR_EXT_ETH_RX_BUF_BASE, 0, MDR_EXT_ETH_RX_BUF_SZ);
    memset((u32 *) MDR_EXT_ETH_DSC_RX_TBL_BASE, 0, MDR_EXT_ETH_DSC_RX_TBL_SZ);
    memset((u32 *) MDR_EXT_ETH_TX_BUF_BASE, 0, MDR_EXT_ETH_TX_BUF_SZ);
    memset((u32 *) MDR_EXT_ETH_DSC_TX_TBL_BASE, 0, MDR_EXT_ETH_DSC_TX_TBL_SZ);

    /* Дескриптор приема + разрешим прерывания */
    for (i = 0; i < EXT_ETH_DSC_RX_NUM_MAX; i++)
	MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[i].CTRL |= EXT_ETH_DSC_RX_CTRL_RDY | EXT_ETH_DSC_RX_CTRL_IRQ_EN;

    MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[EXT_ETH_DSC_RX_NUM_MAX - 1].CTRL |= EXT_ETH_DSC_RX_CTRL_WRAP;

    /* Дескриптор передачи */
    MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[EXT_ETH_DSC_TX_NUM_MAX - 1].CTRL |= EXT_ETH_DSC_TX_CTRL_WRAP;
}


/**
 * Инициализация Ethernet
 */
static void EthInit(void)
{
    EthFrame.Data = EthFr;

    /* Убираем CS если используем парал. шину */
    eth_spi_cs_off(MDR_EXT_ETH);

    RxDescNum = 0;
    TxDescNum = 0;

    pTrBuf = (u32 *) MDR_EXT_ETH_TX_BUF_BASE;

    /* Сброс микросхемы. 1 - весь контроллер сброшен */
    MDR_EXT_ETH->GCTRL |= EXT_ETH_GCTRL_GLBL_RST;

    vTaskDelay(5);

    /* Регистр управления стыка HOST-контроллер - он так называется */	
    MDR_EXT_ETH->GCTRL =
	(0 << EXT_ETH_GCTRL_ASYNC_MODE_Pos) |    	/* 0 - Синхронный режим сигнала RDY */
	(0 << EXT_ETH_GCTRL_SPI_RST_Pos) | 		/* 0 - Последовательный порт в рабочем состоянии (нам не нужен) */
	(1 << EXT_ETH_GCTRL_READ_CLR_STAT_Pos) | 	/* 1 - При чтении регистры флагов чистяца  */
	(0 << EXT_ETH_GCTRL_GLBL_RST_Pos);		/* 0 - Убираем ресет с микросхемы. рабочее состояние */

    /* Регистр MAC - уровня */
    MDR_EXT_ETH->MAC_CTRL = 
	(0 << EXT_ETH_MAC_CTRL_LB_EN_Pos) |    		/* 0- Тестовое замыкание Tx на Rx выключено */
	(0 << EXT_ETH_MAC_CTRL_BIG_ENDIAN_Pos) |	/* 0 - Режим Litle endian */
        (0 << EXT_ETH_MAC_CTRL_HALFD_EN_Pos) |		/* 0 - Полудуплекс отключен. Коллизии НЕ могут быть!!!  */
        (0 << EXT_ETH_MAC_CTRL_BCKOF_DIS_Pos) |         /* 0 - Отключение интервала ожидания случае коллизии */
/*  0 */	(1 << EXT_ETH_MAC_CTRL_ERR_FRAME_EN_Pos) |	/* Пакеты с ошибками не принимаем */
/*  0 */	(1 << EXT_ETH_MAC_CTRL_SHRT_FRAME_EN_Pos) |	/* Короткие пакеты не принимаем */
/*  0 */	(1 << EXT_ETH_MAC_CTRL_LONG_FRAME_EN_Pos) |	/* Длинные пакеты не приниамем */
/*  0 */	(1 << EXT_ETH_MAC_CTRL_CTRL_FRAME_EN_Pos) |	/* Управляющие пакеты не приниаем */
	(0 << EXT_ETH_MAC_CTRL_MCA_EN_Pos) |		/* 0 - вЫключение приема по HASH таблице */
	(1 << EXT_ETH_MAC_CTRL_BCA_EN_Pos) |		/* 1 - Широковещательные пакеты принимаем */
	(0 << EXT_ETH_MAC_CTRL_PRO_EN_Pos) |		/* 0 - Promisc. режим отключаем */
        (1 << EXT_ETH_MAC_CTRL_PAUSE_EN_Pos) |		/* 1 - Пакет PAUSE обрабатывается автоматически */
	(0 << EXT_ETH_MAC_CTRL_DSCR_SCAN_EN_Pos) |      /* 0 - При неготовности RDY - ожидание этого же дескриптора */
	(0 << EXT_ETH_MAC_CTRL_RX_RST_Pos) | 		/* 0 - Приемник НЕ сброшен */
	(0 << EXT_ETH_MAC_CTRL_TX_RST_Pos);		/* 0 - Передатчик НЕ сброшен */

    /* Настройка окна коллизий - поменять!!! */		
    MDR_EXT_ETH->COLL_CONF = 
	(5 << EXT_ETH_COLL_CONF_COLL_WND_Pos) | 
	(5 << EXT_ETH_COLL_CONF_RETRY_LIM_Pos);

    MDR_EXT_ETH->IPGTx = 10;                            /*  Минимальный интервал отправки пакетов */

    MDR_EXT_ETH->PHY_CTRL = 
	(0 << EXT_ETH_PHY_CTRL_LB_Pos) |		/* 0 - Тестовое замыкание на вЫходе PHY выключено. Штатный режим */
	(0 << EXT_ETH_PHY_CTRL_DLB_Pos) |		/* 0 - Тестовое замыкание на входе PHY выключено. Штатный режим */
	(0 << EXT_ETH_PHY_CTRL_HALFD_Pos) |		/* 0 - Дуплекс. включается в двух местах еще и в MAC  */
	(0 << EXT_ETH_PHY_CTRL_EARLY_DV_Pos) |          /* 0 - Сигнал RxDV формируется вместе с первыми битами пакета  */
	(1 << EXT_ETH_PHY_CTRL_DIR_Pos) |               /* 1 - Передача битов MSB */
	(0 << EXT_ETH_PHY_CTRL_BASE_2_Pos) |		/* 0 - По витой паре */
	(7 << EXT_ETH_PHY_CTRL_LINK_PERIOD_Pos) | 	/* Период LINK импульсов */
	(0 << EXT_ETH_PHY_CTRL_RXEN_Pos) | 		/* 0 - Rx отключен */	
	(0 << EXT_ETH_PHY_CTRL_TXEN_Pos) |              /* 0 - Tx отключен */
	(0 << EXT_ETH_PHY_CTRL_RST_Pos);                /* 0 - Рабочее состояние PHY */

    /* Маска прерываний - реагируем на всё */
    MDR_EXT_ETH->INT_MSK = 0x0000;
    MDR_EXT_ETH->INT_SRC = 0xFFFF;

    EthClrRAM();

    /* Установим прерывание на ноге PC7 */
    eth_irq_init(MDR_EXT_ETH);
}

/**
 * Читаем кадр Ethernet
 */
static uint16_t EthReadFrame(tEthFrame * EthFrame)
{
    u16 i;

    EthFrame->Len = MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].LEN;

    RxSrc = (u32 *) (EXT_ETH_BASE_ADDR + (MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].ADDR << 2));
    RxDst = (u16 *) EthFrame->Data;

    for (i = (EthFrame->Len / 2 + (EthFrame->Len & 0x01)); i > 0; i--) {
	*RxDst++ = *RxSrc++;
	RxSrc = (u32 *) (MDR_EXT_ETH_RX_BUF_BASE | ((u32) RxSrc & MDR_EXT_ETH_RX_BUF_SZ_MSK));
    }

    MDR_EXT_ETH->RXBF_HEAD = (MDR_EXT_ETH->RXBF_TAIL - 1) & MDR_EXT_ETH_TX_BUF_SZ16_MSK;

    return EthFrame->Len;
}

/**
 * Отправляем кадр Ethernet
 * Дескритор необходимо модифицировать последним:
 * Длина посылки->Начальный адрес в буфере->Дескриптор!
 */
static int8_t EthWriteFrame(tEthFrame * EthFrame)
{
    u16 i;

    if (MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].CTRL & EXT_ETH_DSC_TX_CTRL_RDY) {
	log_printf("miss tx\r\n");
	return ERR_MEM;
    }
    
    MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].LEN = EthFrame->Len;
    MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].ADDR = (((u32) pTrBuf) >> 2) & MDR_EXT_ETH_ADDR_MSK;

    TxDst = pTrBuf;
    TxSrc = (u16 *) EthFrame->Data;

    for (i = (EthFrame->Len / 2 + (EthFrame->Len & 0x01)); i > 0; i--) {
	*TxDst++ = *TxSrc++;
	TxDst = (u32 *) (MDR_EXT_ETH_TX_BUF_BASE | ((u32) TxDst & MDR_EXT_ETH_TX_BUF_SZ_MSK));
    }
    pTrBuf = TxDst;

    return ERR_OK;
}

/**
 * In this function, the hardware should be initialized.
 * Called from ethernetif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static void low_level_init(struct netif *netif)
{
    struct ethernetif *eth = netif->state;
    TaskHandle_t task = NULL;

    EthInit();

    eth->link = false;
    eth->phy_ok = true;

    /* set MAC hardware address length */
    netif->hwaddr_len = ETHARP_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] = MAC_ADDR0;
    netif->hwaddr[1] = MAC_ADDR1;
    netif->hwaddr[2] = MAC_ADDR2;
    netif->hwaddr[3] = MAC_ADDR3;
    netif->hwaddr[4] = MAC_ADDR4;
    netif->hwaddr[5] = MAC_ADDR5;

    /* initialize MAC address in ethernet MAC */
    MDR_EXT_ETH->MAC_ADDR[0] = (netif->hwaddr[1] << 8) | netif->hwaddr[0];
    MDR_EXT_ETH->MAC_ADDR[1] = (netif->hwaddr[3] << 8) | netif->hwaddr[2];
    MDR_EXT_ETH->MAC_ADDR[2] = (netif->hwaddr[5] << 8) | netif->hwaddr[4];

    /* maximum transfer unit */
    MDR_EXT_ETH->MIN_FRAME = 64;
    MDR_EXT_ETH->MAX_FRAME = 1518;

    netif->mtu = 1500;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

    s_pxNetIf = netif;

    /* Создаем счетный семафор для быстрых событий */
    if (s_xSemaphore == NULL) {
	s_xSemaphore = xSemaphoreCreateCounting(10, 0);
//	s_xSemaphore = xSemaphoreCreateBinary();
    }

    /* Создадим задачу, чтобы она опрашивала eth1 */
    xTaskCreate(eth5_if_task, "eth5 periodic", netifINTERFACE_TASK_STACK_SIZE, s_pxNetIf, netifINTERFACE_TASK_PRIORITY, &task);
    if (task == NULL) {
	log_printf("ERROR: Create eth%c periodic Task\r\n", IFNAME1);
	vTaskDelete(task);
//      configASSERT(task);
    }
    log_printf("SUCCESS: Create eth%c periodic Task. Mac addr: %s\r\n", IFNAME1, FULL_MAC);

    /* Phy: разрешаем Tx и Rx */
    MDR_EXT_ETH->PHY_CTRL |= (EXT_ETH_PHY_CTRL_RXEN | EXT_ETH_PHY_CTRL_TXEN);

    /* Печатаем содержимое регистров */
    eth_print_regs(MDR_EXT_ETH);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    struct pbuf *q;
    int framelen = 0;
    err_t Err;

    static xSemaphoreHandle xTxSemaphore = NULL;

    if (xTxSemaphore == NULL) {
	vSemaphoreCreateBinary(xTxSemaphore);
    }

    if (xSemaphoreTake(xTxSemaphore, netifGUARD_BLOCK_TIME)) {

#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE);	/* drop the padding word */
#endif

	for (q = p; q != NULL; q = q->next) {
	    /* Send the data from the pbuf to the interface, one pbuf at a
	       time. The size of the data in each pbuf is kept in the ->len
	       variable. */
	    memcpy(&EthFrame.Data[framelen], q->payload, q->len);
	    framelen += q->len;
	}
	/* The above memcpy() reduces the system performance, but 
	   it has to be done, as the RTE ethernet driver expects only
	   one and continuous packet data buffer. */
	EthFrame.Len = framelen;

	Err = EthWriteFrame(&EthFrame);

	/* Взводим дескриптор */
	eth5_prepare_tx_desc();

#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE);	/* reclaim the padding word */
#endif

	LINK_STATS_INC(link.xmit);
	xSemaphoreGive(xTxSemaphore);
    }

    return Err;
}

/**
 * Меняем номера дескрипторов.
 */
static void eth5_prepare_tx_desc(void)
{
    if (TxDescNum == (EXT_ETH_DSC_TX_NUM_MAX - 1)) {
	MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].CTRL |= (EXT_ETH_DSC_TX_CTRL_WRAP | EXT_ETH_DSC_TX_CTRL_RDY | EXT_ETH_DSC_RX_CTRL_IRQ_EN);
	TxDescNum = 0;
    } else {
	MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].CTRL |= EXT_ETH_DSC_TX_CTRL_RDY | EXT_ETH_DSC_RX_CTRL_IRQ_EN;
	TxDescNum++;
    }
}


/**
 * Should allocate a pbuf and transfer the bytes of the incoming
 * packet from the interface into the pbuf.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p, *q;
    int len, framelen;

    /* Obtain the size of the packet and put it into the "len" variable. */
    len = EthReadFrame(&EthFrame);

#if ETH_PAD_SIZE
    len += ETH_PAD_SIZE;	/* allow room for Ethernet padding */
#endif

    /* We allocate a pbuf chain of pbufs from the pool. */
    p = pbuf_alloc(PBUF_RAW, len, PBUF_POOL);

    if (p != NULL) {

#if ETH_PAD_SIZE
	pbuf_header(p, -ETH_PAD_SIZE);	/* drop the padding word */
#endif

	/* Copy the data to intermediate buffer. This is required because
	   the RTE ethernet driver copies all the data to one
	   continuous packet data buffer. */
	framelen = 0;
	/* We iterate over the pbuf chain until we have read the entire
	 * packet into the pbuf. */
	for (q = p; q != NULL; q = q->next) {
	    /* Read enough bytes to fill this pbuf in the chain. The
	     * available data in the pbuf is given by the q->len
	     * variable.
	     * This does not necessarily have to be a memcpy, you can also preallocate
	     * pbufs for a DMA-enabled MAC and after receiving truncate it to the
	     * actually received size. In this case, ensure the tot_len member of the
	     * pbuf is the sum of the chained pbuf len members.
	     */
	    memcpy(q->payload, &EthFrame.Data[framelen], q->len);
	    framelen += q->len;
	}

#if ETH_PAD_SIZE
	pbuf_header(p, ETH_PAD_SIZE);	/* reclaim the padding word */
#endif

	LINK_STATS_INC(link.recv);
    } else {
	//drop packet();
	LINK_STATS_INC(link.memerr);
	LINK_STATS_INC(link.drop);
    }

    return p;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t eth5_init(struct netif * netif)
{
    struct ethernetif *eth;

    LWIP_ASSERT("netif != NULL", (netif != NULL));

    eth = (struct ethernetif *) mem_malloc(sizeof(struct ethernetif));
    if (eth == NULL) {
	LWIP_DEBUGF(NETIF_DEBUG, ("ethernetif_init: out of memory\n"));
	return ERR_MEM;
    }
#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif				/* LWIP_NETIF_HOSTNAME */

    /*
     * Initialize the snmp variables and counters inside the struct netif.
     * The last argument should be replaced with your link speed, in units
     * of bits per second.
     */
    NETIF_INIT_SNMP(netif, snmp_ifType_ethernet_csmacd, LINK_SPEED_OF_NETIF_IN_BPS);

    netif->state = eth;
    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}


/**
 * This function should be called when a packet is ready to be read
 * from the interface. It uses the function low_level_input() that
 * should handle the actual reception of bytes from the network
 * interface. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
static void eth5_input(struct netif *netif)
{
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    /* move received packet into a new pbuf */
    p = low_level_input(netif);

    /* no packet could be read, silently ignore this */
    if (p == NULL) {
	log_printf("no packet could be read, silently ignore this\r\n");
	return;
    }

    /* points to packet payload, which starts with an Ethernet header */
    ethhdr = p->payload;

    switch (htons(ethhdr->type)) {
	/* IP or ARP packet? */
    case ETHTYPE_IP:
    case ETHTYPE_ARP:

	/* full packet send to tcpip_thread to process */
	if (netif->input(p, netif) != ERR_OK) {
	    log_printf("eth5_input: IP input error\r\n");
	    pbuf_free(p);
	    p = NULL;
	}
	break;

    default:
        log_printf("eth5_input: Not IP type: %04X\r\n", ethhdr->type);      
	pbuf_free(p);
	p = NULL;
	break;
    }
}

/**
 * Меняем номера дескрипторов.
 */
static void eth5_prepare_rx_desc(void)
{
    /* Меняем дескриптор */
    if (RxDescNum == (EXT_ETH_DSC_RX_NUM_MAX - 1)) {
	MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].CTRL |= EXT_ETH_DSC_RX_CTRL_WRAP | EXT_ETH_DSC_RX_CTRL_RDY | EXT_ETH_DSC_RX_CTRL_IRQ_EN;
	__NOP();
	RxDescNum = 0;
    } else {
	MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].CTRL |= EXT_ETH_DSC_RX_CTRL_RDY | EXT_ETH_DSC_RX_CTRL_IRQ_EN;
	__NOP();
	RxDescNum++;
    }
}


/**
 * Задача приема, ожидает семафора от обработчика прерываний
 */
void eth5_if_task(void *pvParameters)
{
    u16 desc;
    do {
	if (xSemaphoreTake(s_xSemaphore, 100) == pdTRUE) {


	    /* Другие флаги кроме успешного приема и вот этих */
	    if (rx_reg & 0x5840) {
		get_info();
	    }

	    /* Читаем данные пока есть данные приема. 
	     * у нас стоит ожидание при неготовности бита RDY 
             */
	    do {
	        rx_reg = MDR_EXT_ETH->INT_SRC;
		desc = MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].CTRL; 
		if ((desc & EXT_ETH_DSC_RX_CTRL_RDY) == 0) {
		    eth5_input(s_pxNetIf);
		    eth5_prepare_rx_desc();
		} else {
  		     /* смотреть неготовность дескриптора-может здесь проблема */
		    log_printf("descr %d isn't ready: %04X, INT_SRC: %04X\r\n", RxDescNum, desc, MDR_EXT_ETH->INT_SRC);
	        }
	    } while(rx_reg & (EXT_ETH_INT_RXF | EXT_ETH_INT_RXBF_FULL));
	}

    } while (true);
}

/** 
 * Сбросить прерывание чтением 
 * и послать семафор ожидающей задаче
 */
void eth5_irq_handler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    /* Прочитаем причину + Clear the interrupt flags */
    rx_reg = MDR_EXT_ETH->INT_SRC;

    led_toggle();

    /* Индикатор приема пакета и ошибок: Прием, переполнение или потеря пакета 
    *  Отправим сообщение при любом флаге */
    if (rx_reg & (EXT_ETH_INT_RXF | EXT_ETH_INT_RXBF_FULL | EXT_ETH_INT_RXE)) {
	xSemaphoreGiveFromISR(s_xSemaphore, &xHigherPriorityTaskWoken);
    }

    /* Переключить задачу */
    if (xHigherPriorityTaskWoken != pdFALSE) {
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    }
}


void get_info()
{
    log_printf("\r\nINT_SRC: 0x%04X\r\n", (u16) rx_reg);
    log_printf("RxDesc(%d): %04X\r\n", RxDescNum, (u16) MDR_EXT_ETH_DSC_RX_TBL->DSC_RX[RxDescNum].CTRL);
    log_printf("TxDesc(%d): %04X\r\n", TxDescNum, (u16) MDR_EXT_ETH_DSC_TX_TBL->DSC_TX[TxDescNum].CTRL);
}

/**
 * Прочитать MAC адрес
 */
void eth5_get_mac_addr(u8* addr)
{
    memcpy(addr, s_pxNetIf->hwaddr, 6);
}


/**
 * Записать MAC адрес
 */
void eth5_set_mac_addr(u8* addr)
{
    NVIC_DisableIRQ(MDR_EXT_IRQ_NUM);	/* Disable IRQ */

    /* set MAC hardware address */
    s_pxNetIf->hwaddr[0] = addr[0];
    s_pxNetIf->hwaddr[1] = addr[1];
    s_pxNetIf->hwaddr[2] = addr[2];
    s_pxNetIf->hwaddr[3] = addr[3];
    s_pxNetIf->hwaddr[4] = addr[4];
    s_pxNetIf->hwaddr[5] = addr[5];

    /* initialize MAC address in ethernet MAC */
    MDR_EXT_ETH->MAC_ADDR[0] = (s_pxNetIf->hwaddr[1] << 8) | s_pxNetIf->hwaddr[0];
    MDR_EXT_ETH->MAC_ADDR[1] = (s_pxNetIf->hwaddr[3] << 8) | s_pxNetIf->hwaddr[2];
    MDR_EXT_ETH->MAC_ADDR[2] = (s_pxNetIf->hwaddr[5] << 8) | s_pxNetIf->hwaddr[4];

    rx_reg = MDR_EXT_ETH->INT_SRC;

    NVIC_EnableIRQ(MDR_EXT_IRQ_NUM);	/* Enable IRQ */
}

