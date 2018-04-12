/************************************************************************************
 *
 * 	Echo server lwip+freertos+udp test
 *  	Copyright (c) rauschenbach 
 *      sokareis@mail.ru
 *
 ***********************************************************************************/
#include <stdio.h>
#include <lwip/api.h>
#include <lwip/opt.h>
#include <lwip/arch.h>
#include <lwip/api.h>
#include <lwip/inet.h>
#include <lwip/sockets.h>

#include "globdefs.h"
#include "eth_task.h"
#include "main.h"
#include "tcpip.h"
#include "eth_common.h"
#include "userfunc.h"

/* Static IP ADDRESS */
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   0
#define IP_ADDR3   102

/* NETMASK */
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/* Gateway Address */
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   0
#define GW_ADDR3   1


#define 	MAX_DHCP_TRIES 		5
#define 	TASK_NAME		"eth5 task"


static struct netif g_netif;
static struct ip_addr ipaddr;
static struct ip_addr netmask;
static struct ip_addr gw;

static void eth5_task(void *p);
static void lwip_dhcp_task(void *p);

/**
 *  Инициализация IP стека и всех интерфесов
 */
int eth_create_task(void)
{
    TaskHandle_t task = NULL;
    
    /* Static address used */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
    netif_set_addr(&g_netif, &ipaddr, &netmask, &gw);
    log_printf("ETH5 static IP address:  %d.%d.%d.%d\n", IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);

    /* Add your network interface to the netif_list. */
    netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, &eth5_init, /*&ethernet_input */ &tcpip_input);
    netif_set_up(&g_netif);
    xTaskCreate(eth5_task, TASK_NAME, configMINIMAL_STACK_SIZE, NULL, ETH_TASK_PRIORITY, &task);
    if (task == NULL) {
	log_printf("ERROR: Create %s\r\n", TASK_NAME);
	vTaskDelete(task);
	configASSERT(task);
    }
    log_printf("SUCCESS: Create %s\r\n", TASK_NAME);
    return 0;
}


/**
 * Задача
 */
void eth5_task(void *p)
{
#define ECHO_PORT   7
    static char buf[1024];
    int sockfd, n;
    struct sockaddr_in local;
    u32 addr_len = sizeof(struct sockaddr);

    /* Создаем сокет TCP с этими настойками */
    local.sin_port = htons(ECHO_PORT);
    local.sin_family = PF_INET;
    local.sin_addr.s_addr = ipaddr.addr;

    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) {
	log_printf("create socket failed\r\n");
	return;
    }
    log_printf("create socket OK\r\n");

    /* bind to port at any interface */
    if (bind(sockfd, (struct sockaddr *) &local, addr_len) < 0) {
	log_printf("error bind\r\n");
    }
    log_printf("bind OK\r\n");

    /* received data to buffer */
    while (1) {
	n = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *) &local, (u32_t *) & addr_len);
	if (n > 0) {
	    buf[n] = 0;

	    log_printf("%d bytes recv from (%s) : %s\r\n", n, inet_ntoa(local.sin_addr), buf);
	    if (sendto(sockfd, buf, n, 0, (struct sockaddr *) &local, addr_len) < 0) {
		log_printf("Error sendto\r\n");
	    }
	}
	vTaskDelay(5);
    }
}
