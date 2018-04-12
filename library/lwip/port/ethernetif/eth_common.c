/************************************************************************************
 *
 * 	������� eth 5600��1�
 *  	Copyright (c) rauschenbach 
 *      <sokareis@mail.ru>
 *
 ***********************************************************************************/
#include "eth_common.h"
#include "main.h"

static void arp_timer(void *);

/**
 *  ��������� �������� 
 */
void eth_print_regs(MDR_EXT_ETH_TypeDef * eth)
{
    log_printf("-------------------------------------\r\n");
    log_printf("MAC_CTRL %04X\n", (u16) eth->MAC_CTRL);
    log_printf("MIN_FRAME %04X\n", (u16) eth->MIN_FRAME);
    log_printf("MAX_FRAME %04X\n", (u16) eth->MAX_FRAME);
    log_printf("COLL_CONF %04X\n", (u16) eth->COLL_CONF);
    log_printf("IPGTx %04X\n", (u16) eth->IPGTx);
    log_printf("MAC_ADDR[0] %04X\n", (u16) eth->MAC_ADDR[0]);
    log_printf("MAC_ADDR[1] %04X\n", (u16) eth->MAC_ADDR[1]);
    log_printf("MAC_ADDR[2] %04X\n", (u16) eth->MAC_ADDR[2]);
    log_printf("INT_MSK - %04X\n", (u16) eth->INT_MSK);
    log_printf("INT_SRC - %04X\n", (u16) eth->INT_SRC);
    log_printf("PHY_CTRL - %04X\n", (u16) eth->PHY_CTRL);
    log_printf("PHY_STAT - %04X\n", (u16) eth->PHY_STAT);
    log_printf("RXBF_HEAD - %04X\n", (u16) eth->RXBF_HEAD);
    log_printf("RXBF_TAIL- %04X\n", (u16) eth->RXBF_TAIL);
    log_printf("STAT_RX_ALL - %04X\n", (u16) eth->STAT_RX_ALL);
    log_printf("STAT_RX_OK - %04X\n", (u16) eth->STAT_RX_OK);
    log_printf("STAT_RX_OVF - %04X\n", (u16) eth->STAT_RX_OVF);
    log_printf("STAT_RX_LOST - %04X\n", (u16) eth->STAT_RX_LOST);
    log_printf("STAT_TX_ALL - %04X\n", (u16) eth->STAT_TX_ALL);
    log_printf("STAT_TX_OK - %04X\n", (u16) eth->STAT_TX_OK);
    log_printf("GCTRL - %04X\n", (u16) eth->GCTRL);
}

/**
 * ������ CS. ������ SPI � Z ���������
 * ������� CS ���� ���������� �����. ����
 */
void eth_spi_cs_off(MDR_EXT_ETH_TypeDef * eth)
{
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTD;

    /* ������ CS �� ������  */
    if (eth == (MDR_EXT_ETH_TypeDef *) (ETH3_BASE_ADDR + 4 * 0x1FC0)) {
	MDR_PORTD->OE |= PORT_Pin_0;	/* ����� CS3 */
	MDR_PORTD->FUNC &= 0xFFFFFFFC;	/* ������� - ����  */
	MDR_PORTD->ANALOG |= 0x00000001;	/* �������� ����� */
	MDR_PORTD->PWR |= 0x02;	/* ������� ����� */
	MDR_PORTD->SETTX = PORT_Pin_0;	/* ������ � 1-�� */
    } else if (eth == (MDR_EXT_ETH_TypeDef *) (ETH4_BASE_ADDR + 4 * 0x1FC0)) {
	MDR_PORTD->OE |= PORT_Pin_1;	/* ����� CS4 */
	MDR_PORTD->FUNC &= 0xFFFFFFF3;	/* ������� - ����  */
	MDR_PORTD->ANALOG |= 0x00000002;	/* �������� ����� */
	MDR_PORTD->PWR |= 0x08;	/* ������� ����� */
	MDR_PORTD->SETTX = PORT_Pin_1;	/* ������ � 1-�� */
    } else if (eth == (MDR_EXT_ETH_TypeDef *) (ETH5_BASE_ADDR + 4 * 0x1FC0)) {
	MDR_PORTD->OE |= PORT_Pin_5;	/* ����� CS4 */
	MDR_PORTD->FUNC &= 0xFFFFF3FF;	/* ������� - ����  */
	MDR_PORTD->ANALOG |= 0x00000020;	/* �������� ����� */
	MDR_PORTD->PWR |= 0xFBFF;	/* ������� ����� */
	MDR_PORTD->SETTX = PORT_Pin_5;	/* ������ � 1-�� */
    } else {
	PORT_InitTypeDef init;
	init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_5;
	init.PORT_FUNC = PORT_FUNC_PORT;
	init.PORT_OE = PORT_OE_OUT;
	init.PORT_MODE = PORT_MODE_DIGITAL;
	init.PORT_SPEED = PORT_SPEED_FAST;
	PORT_Init(MDR_PORTD, &init);
	PORT_SetBits(MDR_PORTD, PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_5);
    }
}

/**
 * ������ ��� ARP ����
 */
static void arp_timer(void *arg)
{
    etharp_tmr();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

/**
 * ARP ������ ��� ���� ����������
 * ��������� ������ ��� arp ����
 */
void eth_arp_timer_start(void)
{
    etharp_init();
    sys_timeout(ARP_TMR_INTERVAL, arp_timer, NULL);
}

void eth_print_stat(MDR_EXT_ETH_TypeDef * eth)
{
    log_printf("-------------------------------------\r\n");
    log_printf("STAT_RX_ALL %d\n", (u16) eth->STAT_RX_ALL);
    log_printf("STAT_RX_OK %d\n", (u16) eth->STAT_RX_OK);
    log_printf("STAT_RX_OVF %d\n", (u16) eth->STAT_RX_OVF);
    log_printf("STAT_RX_LOST %d\n", (u16) eth->STAT_RX_LOST);
    log_printf("STAT_TX_ALL %d\n", (u16) eth->STAT_TX_ALL);
    log_printf("STAT_TX_OK %d\n", (u16) eth->STAT_TX_OK);
}

/**
 * ������������� IRQ5, IRQ6 � IRQ7 �� ����� PC5..PC6..PC7
 * for input EXT_INT1 EXT_INT2 EXT_INT3  
 * ��� 3-� ������� ethernet
 */
void eth_irq_init(MDR_EXT_ETH_TypeDef * eth)
{
    PORT_InitTypeDef port;
    u32 num;

    PORT_StructInit(&port);

    /* ������ CS �� ������  */
    if (eth == (MDR_EXT_ETH_TypeDef *) (ETH3_BASE_ADDR + 4 * 0x1FC0)) {
	port.PORT_Pin = PORT_Pin_5;
	num = EXT_INT1_IRQn;
    } else if (eth == (MDR_EXT_ETH_TypeDef *) (ETH4_BASE_ADDR + 4 * 0x1FC0)) {
	port.PORT_Pin = PORT_Pin_6;
	num = EXT_INT2_IRQn;
    } else {		
	port.PORT_Pin = PORT_Pin_7;
	num = EXT_INT3_IRQn;
    }

    port.PORT_FUNC = PORT_FUNC_MAIN;
    port.PORT_OE = PORT_OE_IN;
    port.PORT_SPEED = PORT_SPEED_MAXFAST;
    port.PORT_MODE = PORT_MODE_DIGITAL;
    port.PORT_PULL_UP = PORT_PULL_UP_OFF;
    port.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;	/* �������� � GND */
    PORT_Init(MDR_PORTC, &port);

    eth->INT_MSK = 0x8000 | 0x5840;	/* ���������� ����� IRQ �� ����������� ������ ����� � ������� */

    NVIC_EnableIRQ((IRQn_Type)num);	/* Enable IRQ */
}

