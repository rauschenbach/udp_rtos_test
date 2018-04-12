#ifndef _ETH_COMMON_H_
#define _ETH_COMMON_H_

/* Описания внешних ethernet различаются адресами */

#include	<stdint.h>
#include	"globdefs.h"
#include	"netif.h"
#include	"main.h"
#include	"err.h"

/* Количество дескрипторов пакетов приёмника <1-32> */
#define EXT_ETH_DSC_RX_NUM_MAX			16

/*  Количество дескрипторов пакетов передатчика <1-32>  */
#define EXT_ETH_DSC_TX_NUM_MAX			16


/* MDR_EXT_ETH_TypeDef structure */
typedef struct {
    __IO uint32_t MAC_CTRL;
    __IO uint32_t MIN_FRAME;
    __IO uint32_t MAX_FRAME;
    __IO uint32_t COLL_CONF;
    __IO uint32_t IPGTx;
    __IO uint32_t MAC_ADDR[3];
    __IO uint32_t HASH[4];
    __IO uint32_t INT_MSK;
    __IO uint32_t INT_SRC;
    __IO uint32_t PHY_CTRL;
    __IO uint32_t PHY_STAT;
    __IO uint32_t RXBF_HEAD;
    __IO uint32_t RXBF_TAIL;
    uint32_t RESERVED0[2];
    __IO uint32_t STAT_RX_ALL;
    __IO uint32_t STAT_RX_OK;
    __IO uint32_t STAT_RX_OVF;
    __IO uint32_t STAT_RX_LOST;
    __IO uint32_t STAT_TX_ALL;
    __IO uint32_t STAT_TX_OK;
    uint32_t RESERVED1[5];
    __IO uint32_t GCTRL;
} MDR_EXT_ETH_TypeDef;


/*-- MAC_CTRL Register ------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_MAC_CTRL_LB_EN_Pos					0
#define EXT_ETH_MAC_CTRL_BIG_ENDIAN_Pos					1
#define EXT_ETH_MAC_CTRL_HALFD_EN_Pos					2
#define EXT_ETH_MAC_CTRL_BCKOF_DIS_Pos					3
#define EXT_ETH_MAC_CTRL_ERR_FRAME_EN_Pos				4
#define EXT_ETH_MAC_CTRL_SHRT_FRAME_EN_Pos			5
#define EXT_ETH_MAC_CTRL_LONG_FRAME_EN_Pos			6
#define EXT_ETH_MAC_CTRL_CTRL_FRAME_EN_Pos			7
#define EXT_ETH_MAC_CTRL_MCA_EN_Pos					8
#define EXT_ETH_MAC_CTRL_BCA_EN_Pos					9
#define EXT_ETH_MAC_CTRL_PRO_EN_Pos					10
#define EXT_ETH_MAC_CTRL_PAUSE_EN_Pos					11
#define EXT_ETH_MAC_CTRL_DSCR_SCAN_EN_Pos				12
#define EXT_ETH_MAC_CTRL_RX_RST_Pos					14
#define EXT_ETH_MAC_CTRL_TX_RST_Pos					15

/* Bit field masks: */
#define EXT_ETH_MAC_CTRL_LB_EN				((uint16_t)0x0001)
#define EXT_ETH_MAC_CTRL_BIG_ENDIAN			((uint16_t)0x0002)
#define EXT_ETH_MAC_CTRL_HALFD_EN			((uint16_t)0x0004)
#define EXT_ETH_MAC_CTRL_BCKOF_DIS			((uint16_t)0x0008)
#define EXT_ETH_MAC_CTRL_ERR_FRAME_EN			((uint16_t)0x0010)
#define EXT_ETH_MAC_CTRL_SHRT_FRAME_EN			((uint16_t)0x0020)
#define EXT_ETH_MAC_CTRL_LONG_FRAME_EN			((uint16_t)0x0040)
#define EXT_ETH_MAC_CTRL_CTRL_FRAME_EN			((uint16_t)0x0080)
#define EXT_ETH_MAC_CTRL_MCA_EN				((uint16_t)0x0100)
#define EXT_ETH_MAC_CTRL_BCA_EN				((uint16_t)0x0200)
#define EXT_ETH_MAC_CTRL_PRO_EN				((uint16_t)0x0400)
#define EXT_ETH_MAC_CTRL_PAUSE_EN			((uint16_t)0x0800)
#define EXT_ETH_MAC_CTRL_DSCR_SCAN_EN			((uint16_t)0x1000)
#define EXT_ETH_MAC_CTRL_RX_RST				((uint16_t)0x4000)
#define EXT_ETH_MAC_CTRL_TX_RST				((uint16_t)0x8000)

/*-- COLL_CONF Register ------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_COLL_CONF_COLL_WND_Pos					0
#define EXT_ETH_COLL_CONF_RETRY_LIM_Pos					8

/* Bit field masks: */
#define EXT_ETH_COLL_CONF_COLL_WND_Msk			((uint16_t)0x00FF)
#define EXT_ETH_COLL_CONF_RETRY_LIM_Msk			((uint16_t)0x0F00)

/*-- INT_MSK/INT_SRC Registers -----------------------------------*/
/* Bit field positions: */
#define EXT_ETH_INT_TX_BUSY_Pos						0
#define EXT_ETH_INT_TXC_Pos						5
#define EXT_ETH_INT_TXE_Pos						6
#define EXT_ETH_INT_TXF_Pos						7
#define EXT_ETH_INT_RXS_Pos						9
#define EXT_ETH_INT_RXL_Pos						10
#define EXT_ETH_INT_RXD_NOT_READY_Pos					11
#define EXT_ETH_INT_RXBF_FULL_Pos					12
#define EXT_ETH_INT_RXC_Pos						13
#define EXT_ETH_INT_RXE_Pos						14
#define EXT_ETH_INT_RXF_Pos						15

/* Bit field masks: */
#define EXT_ETH_INT_TX_BUSY				((uint16_t)0x0001)
#define EXT_ETH_INT_TXC					((uint16_t)0x0020)
#define EXT_ETH_INT_TXE					((uint16_t)0x0040)
#define EXT_ETH_INT_TXF					((uint16_t)0x0080)
#define EXT_ETH_INT_RXS					((uint16_t)0x0200)
#define EXT_ETH_INT_RXL					((uint16_t)0x0400)
#define EXT_ETH_INT_RXD_NOT_READY			((uint16_t)0x0800)
#define EXT_ETH_INT_RXBF_FULL				((uint16_t)0x1000)
#define EXT_ETH_INT_RXC					((uint16_t)0x2000)
#define EXT_ETH_INT_RXE					((uint16_t)0x4000)
#define EXT_ETH_INT_RXF					((uint16_t)0x8000)

/*-- PHY_CTRL Register ------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_PHY_CTRL_LB_Pos					0
#define EXT_ETH_PHY_CTRL_DLB_Pos				1
#define EXT_ETH_PHY_CTRL_HALFD_Pos				2
#define EXT_ETH_PHY_CTRL_EARLY_DV_Pos				3
#define EXT_ETH_PHY_CTRL_DIR_Pos				4
#define EXT_ETH_PHY_CTRL_BASE_2_Pos				5
#define EXT_ETH_PHY_CTRL_LINK_PERIOD_Pos			6
#define EXT_ETH_PHY_CTRL_RXEN_Pos				12
#define EXT_ETH_PHY_CTRL_TXEN_Pos				13
#define EXT_ETH_PHY_CTRL_RST_Pos				15

/* Bit field masks: */
#define EXT_ETH_PHY_CTRL_LB				((uint16_t)0x0001)
#define EXT_ETH_PHY_CTRL_DLB				((uint16_t)0x0002)
#define EXT_ETH_PHY_CTRL_HALFD				((uint16_t)0x0004)
#define EXT_ETH_PHY_CTRL_EARLY_DV			((uint16_t)0x0008)
#define EXT_ETH_PHY_CTRL_DIR				((uint16_t)0x0010)
#define EXT_ETH_PHY_CTRL_BASE_2				((uint16_t)0x0020)
#define EXT_ETH_PHY_CTRL_LINK_PERIOD_Msk		((uint16_t)0x0FC0)
#define EXT_ETH_PHY_CTRL_RXEN				((uint16_t)0x1000)
#define EXT_ETH_PHY_CTRL_TXEN				((uint16_t)0x2000)
#define EXT_ETH_PHY_CTRL_RST				((uint16_t)0x8000)

/*-- PHY_STAT Register ------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_PHY_STAT_LINK_Pos					10

/* Bit field masks: */
#define EXT_ETH_PHY_STAT_LINK				((uint16_t)0x0400)

/*-- GCTRL Register ------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_GCTRL_ASYNC_MODE_Pos					12
#define EXT_ETH_GCTRL_SPI_RST_Pos					13
#define EXT_ETH_GCTRL_READ_CLR_STAT_Pos					14
#define EXT_ETH_GCTRL_GLBL_RST_Pos					15

/* Bit field masks: */
#define EXT_ETH_GCTRL_ASYNC_MODE			((uint16_t)0x1000)
#define EXT_ETH_GCTRL_SPI_RST				((uint16_t)0x2000)
#define EXT_ETH_GCTRL_READ_CLR_STAT			((uint16_t)0x4000)
#define EXT_ETH_GCTRL_GLBL_RST				((uint16_t)0x8000)


/* MDR_EXT_ETH_DSC_TypeDef structure */
typedef struct {
    uint32_t CTRL;
    uint32_t LEN;
    uint32_t RESERVED;
    uint32_t ADDR;
} MDR_EXT_ETH_DSC_TypeDef;


/*-- DSC_RX_CTRL ----------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_DSC_RX_CTRL_OR_Pos					0
#define EXT_ETH_DSC_RX_CTRL_SMB_ERR_Pos					2
#define EXT_ETH_DSC_RX_CTRL_CRC_ERR_Pos					3
#define EXT_ETH_DSC_RX_CTRL_EF_Pos					4
#define EXT_ETH_DSC_RX_CTRL_SF_Pos					5
#define EXT_ETH_DSC_RX_CTRL_LF_Pos					6
#define EXT_ETH_DSC_RX_CTRL_CF_Pos					7
#define EXT_ETH_DSC_RX_CTRL_UCA_Pos					8
#define EXT_ETH_DSC_RX_CTRL_BCA_Pos					9
#define EXT_ETH_DSC_RX_CTRL_MCA_Pos					10
#define EXT_ETH_DSC_RX_CTRL_IRQ_EN_Pos					13
#define EXT_ETH_DSC_RX_CTRL_WRAP_Pos					14
#define EXT_ETH_DSC_RX_CTRL_RDY_Pos					15

/* Bit field masks: */
#define EXT_ETH_DSC_RX_CTRL_OR				((uint16_t)0x0001)
#define EXT_ETH_DSC_RX_CTRL_SMB_ERR			((uint16_t)0x0004)
#define EXT_ETH_DSC_RX_CTRL_CRC_ERR			((uint16_t)0x0008)
#define EXT_ETH_DSC_RX_CTRL_EF				((uint16_t)0x0010)
#define EXT_ETH_DSC_RX_CTRL_SF				((uint16_t)0x0020)
#define EXT_ETH_DSC_RX_CTRL_LF				((uint16_t)0x0040)
#define EXT_ETH_DSC_RX_CTRL_CF				((uint16_t)0x0080)
#define EXT_ETH_DSC_RX_CTRL_UCA				((uint16_t)0x0100)
#define EXT_ETH_DSC_RX_CTRL_BCA				((uint16_t)0x0200)
#define EXT_ETH_DSC_RX_CTRL_MCA				((uint16_t)0x0400)
#define EXT_ETH_DSC_RX_CTRL_IRQ_EN			((uint16_t)0x2000)
#define EXT_ETH_DSC_RX_CTRL_WRAP			((uint16_t)0x4000)
#define EXT_ETH_DSC_RX_CTRL_RDY				((uint16_t)0x8000)

/*-- DSC_TX_CTRL ----------------------------------------------*/
/* Bit field positions: */
#define EXT_ETH_DSC_TX_CTRL_CS_Pos					0
#define EXT_ETH_DSC_TX_CTRL_LC_Pos					2
#define EXT_ETH_DSC_TX_CTRL_RL_Pos					3
#define EXT_ETH_DSC_TX_CTRL_RTRY_Pos					4
#define EXT_ETH_DSC_TX_CTRL_CRC_DIS_Pos					8
#define EXT_ETH_DSC_TX_CTRL_PAD_DIS_Pos					9
#define EXT_ETH_DSC_TX_CTRL_PRE_DIS_Pos					10
#define EXT_ETH_DSC_TX_CTRL_IRQ_EN_Pos					13
#define EXT_ETH_DSC_TX_CTRL_WRAP_Pos					14
#define EXT_ETH_DSC_TX_CTRL_RDY_Pos					15

/* Bit field masks: */
#define EXT_ETH_DSC_TX_CTRL_CS				((uint16_t)0x0001)
#define EXT_ETH_DSC_TX_CTRL_LC				((uint16_t)0x0004)
#define EXT_ETH_DSC_TX_CTRL_RL				((uint16_t)0x0008)
#define EXT_ETH_DSC_TX_CTRL_RTRY			((uint16_t)0x00F0)
#define EXT_ETH_DSC_TX_CTRL_CRC_DIS			((uint16_t)0x0100)
#define EXT_ETH_DSC_TX_CTRL_PAD_DIS			((uint16_t)0x0200)
#define EXT_ETH_DSC_TX_CTRL_PRE_DIS			((uint16_t)0x0400)
#define EXT_ETH_DSC_TX_CTRL_IRQ_EN			((uint16_t)0x2000)
#define EXT_ETH_DSC_TX_CTRL_WRAP			((uint16_t)0x4000)
#define EXT_ETH_DSC_TX_CTRL_RDY				((uint16_t)0x8000)

#define EXT_ETH_DSC_RX_NUM				32
#define EXT_ETH_DSC_TX_NUM				32


/* MDR_EXT_ETH_DSC_RX_TBL_TypeDef structure */
typedef struct {
    MDR_EXT_ETH_DSC_TypeDef DSC_RX[EXT_ETH_DSC_RX_NUM];
} MDR_EXT_ETH_DSC_RX_TBL_TypeDef;

/* MDR_EXT_ETH_DSC_TX_TBL_TypeDef structure */
typedef struct {
    MDR_EXT_ETH_DSC_TypeDef DSC_TX[EXT_ETH_DSC_TX_NUM];
} MDR_EXT_ETH_DSC_TX_TBL_TypeDef;



#define	MDR_EXT_ETH_ADDR_MSK			0x1FFF
#define	MDR_EXT_ETH_TX_BUF_SZ16_MSK		0x07FF

#define	MDR_EXT_ETH_RX_BUF_SZ			(4 * 0x0800)
#define	MDR_EXT_ETH_DSC_RX_TBL_SZ		(4 * 0x0080)
#define	MDR_EXT_ETH_TX_BUF_SZ			(4 * 0x0800)
#define	MDR_EXT_ETH_DSC_TX_TBL_SZ		(4 * 0x0080)
#define MDR_EXT_ETH_RX_BUF_SZ_MSK		(MDR_EXT_ETH_RX_BUF_SZ - 1)
#define MDR_EXT_ETH_TX_BUF_SZ_MSK		(MDR_EXT_ETH_TX_BUF_SZ - 1)


#define netifMTU                                (1500)
#define netifINTERFACE_TASK_STACK_SIZE		(configMINIMAL_STACK_SIZE)
#define netifINTERFACE_TASK_PRIORITY		(tskIDLE_PRIORITY + 3)
#define netifGUARD_BLOCK_TIME			(250)

/* The time to block waiting for input. */
#define emacBLOCK_TIME_WAITING_FOR_INPUT	(( portTickType ) 100)


/* Адреса Ethernet */
#define	ETH3_BASE_ADDR	    			0xA1800000
#define	ETH4_BASE_ADDR	    			0xA1400000
#define	ETH5_BASE_ADDR	    			0xA0C00000 


/**
 * Helper struct to hold private data used to operate your ethernet interface.
 * Keeping the ethernet address of the MAC in this struct is not necessary
 * as it is already kept in the struct netif.
 * But this is only an example, anyway...
 */
#if 10
struct ethernetif {
    bool link;			// Ethernet Link State
    bool phy_ok;		// Phy initialized successfully
    /* Add whatever per-interface state that is needed here. */
};
#else
typedef struct {
    bool link;			// Ethernet Link State
    bool phy_ok;		// Phy initialized successfully
    /* Add whatever per-interface state that is needed here. */
} ethernetif;

#endif

#define	ETH_FRAME_SIZE				1514
#define	LINK_SPEED_OF_NETIF_IN_BPS		10e6

/* Кадр ethernet */
typedef struct {
    unsigned short Len;
    unsigned char *Data;
} tEthFrame;


#define 	MAC_ADDR0   		FULL_MAC[0]
#define 	MAC_ADDR1   		FULL_MAC[1]
#define 	MAC_ADDR2   		FULL_MAC[2]
#define 	MAC_ADDR3   		FULL_MAC[3]
#define 	MAC_ADDR4   		FULL_MAC[4]
#define 	MAC_ADDR5   		FULL_MAC[5]

#define 	WAIT_WHILE_POLLING	10

err_t eth1_init(struct netif *netif);
void  eth1_poll(struct netif *netif);
void  eth1_check_link(struct netif *netif);
void  eth1_periodic_task(void *);
void  eth1_irq_handler(void);

err_t eth2_init(struct netif *netif);
void  eth2_poll(struct netif *netif);
void  eth2_check_link(struct netif *netif);
void  eth2_periodic_task(void *);
void  eth2_irq_handler(void);


err_t eth3_init(struct netif *netif);
void  eth3_poll(struct netif *netif);
void  eth3_check_link(struct netif *netif);
void  eth3_irq_handler(void);

err_t eth4_init(struct netif *netif);
void  eth4_poll(struct netif *netif);
void  eth4_check_link(struct netif *netif);
void  eth4_irq_handler(void);

err_t eth5_init(struct netif *netif);
void  eth5_poll(struct netif *netif);
void  eth5_check_link(struct netif *netif);
void  eth5_irq_handler(void);

void eth_print_regs(MDR_EXT_ETH_TypeDef *);
void eth_print_stat(MDR_EXT_ETH_TypeDef *);
void eth_spi_cs_off(MDR_EXT_ETH_TypeDef *);
void eth_irq_init(MDR_EXT_ETH_TypeDef *);

void eth_arp_timer_start(void);

#endif /* eth_common.h */
