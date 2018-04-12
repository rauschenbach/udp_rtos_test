#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

#define SYS_LIGHTWEIGHT_PROT    0

#define ETHARP_TRUST_IP_MAC     0
#define IP_REASSEMBLY           0
#define IP_FRAG                 0
#define ARP_QUEUEING            0
#define TCP_LISTEN_BACKLOG      0

/**
 * NO_SYS==1: Provides VERY minimal functionality. Otherwise,
 * use lwIP facilities.
 */
#define LWIP_DHCP			0
#define NO_SYS                  	0
#define MEMP_MEM_MALLOC         	1
#define MEM_ALIGNMENT           	4
#define MEM_SIZE                	(16 * 1024)
#define MEMP_NUM_PBUF           	100
#define MEMP_NUM_UDP_PCB        	10
#define MEMP_NUM_TCP_PCB        	5
#define MEMP_NUM_TCP_PCB_LISTEN 	5
#define MEMP_NUM_TCP_SEG        	20
#define MEMP_NUM_SYS_TIMEOUT    	10
#define PBUF_POOL_SIZE          	20

/* Определить правильный размер!!! */
#define PBUF_POOL_BUFSIZE       	512 /*(4096)*/


#define LWIP_TCP                	0	/* Нет TCP */
#define TCP_TTL                 	255
#define TCP_QUEUE_OOSEQ         	0

#define ETH_PAD_SIZE                	2
#define LWIP_CHKSUM_ALGORITHM       	2

#define LWIP_NETIF_LINK_CALLBACK	1

/* TCP Maximum segment size. */
#define TCP_MSS                 	(1500 - 40)	  /* TCP_MSS = (Ethernet MTU - IP header size - TCP header size) */

/* TCP sender buffer space (bytes). */
#define TCP_SND_BUF             	(2 * TCP_MSS)

/*  TCP_SND_QUEUELEN: TCP sender buffer space (pbufs). This must be at least
  as much as (2 * TCP_SND_BUF/TCP_MSS) for things to work. */
#define TCP_SND_QUEUELEN        	(2 * TCP_SND_BUF/TCP_MSS)

/* TCP receive window. */
#define TCP_WND                 	(2 * TCP_MSS)


#define LWIP_AUTOIP 			0
#define LWIP_DHCP_AUTOIP_COOP 		0	


/* ---------- ICMP options ---------- */
#define LWIP_ICMP               	1


/* ---------- UDP options ---------- */
#define LWIP_UDP                	1
#define UDP_TTL                 	255
#define LWIP_STATS 			1
#define LWIP_PROVIDE_ERRNO 		1


/* контрольную сумму считает карта */
#define CHECKSUM_BY_HARDWARE            1

#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_CHECK_IP               1
#define CHECKSUM_CHECK_UDP              1
#define CHECKSUM_CHECK_TCP              1

#define LWIP_NETCONN                    1
#define LWIP_SOCKET                     1


#define TCPIP_THREAD_STACKSIZE          2000
#define TCPIP_MBOX_SIZE                 5
#define DEFAULT_UDP_RECVMBOX_SIZE       2000
#define DEFAULT_TCP_RECVMBOX_SIZE       2000
#define DEFAULT_ACCEPTMBOX_SIZE         2000
#define DEFAULT_THREAD_STACKSIZE        500
#define TCPIP_THREAD_PRIO               (configMAX_PRIORITIES - 2)


#endif /* __LWIPOPTS_H__ */
