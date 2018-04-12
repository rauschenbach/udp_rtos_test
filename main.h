#ifndef _MAIN_H
#define _MAIN_H

#include <MDR32F9Qx_usb_handlers.h>
#include <MDR32F9Qx_rst_clk.h>
#include <MDR32F9Qx_config.h>
#include <MDR32F9Qx_eeprom.h>
#include <MDR32F9Qx_config.h>
#include <MDR32F9Qx_board.h>
#include <MDR32F9Qx_timer.h>
#include <MDR32F9Qx_uart.h>
#include <MDR32F9Qx_port.h>
#include <MDR1986VE3_it.h>
#include <MDR32F9Qx_ssp.h>
#include <MDR32F9Qx_eth.h>
#include <MDR1986VE3.h>
#include "led.h"

#include "eth_common.h"
#include "tcpip.h"
#include "userfunc.h"
#include "systick.h"
#include "eth_task.h"

#include <FreeRTOSConfig.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <clock.h>
#include <task.h>

/* Task priorities. */
#define 	COMM_TASK_PRIORITY		(tskIDLE_PRIORITY + 2) 
#define 	GEN_QUEUE_TASK_PRIORITY		(tskIDLE_PRIORITY)
#define 	TEST_TASK_PRIORITY		(tskIDLE_PRIORITY + 1)
#define 	LED_TASK_PRIORITY		(tskIDLE_PRIORITY + 2)


#define 	ETH_TASK_PRIORITY		(tskIDLE_PRIORITY + 1)


#include "tcp.h"
#include "dhcp.h"
#include "init.h"
#include "netif.h"
#include "etharp.h"



#define USE_DHCP       /* enable DHCP, if disabled static address is used */




u32_t sys_now(void);

#endif /* main.h */
