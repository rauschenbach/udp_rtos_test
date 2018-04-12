#ifndef _UART1_H
#define _UART1_H

#include "main.h"


#define XCHG_BUF_LEN		2048 + 40
#define OSI_WAIT_FOREVER 	(0xFFFFFFFF)
#define COMM_RESET_TIMEOUT	1000 /* 1 ������� */


/* ������ ������  */
typedef enum {
    RX_OK, 		/* ��� � ������� */
    RX_TIMEOUT,         /* ������� */
    RX_CRC_ERR,		/* ������ ����������� ����� */
    RX_FAIL,            /* ������ ������ */
} rx_status_en;



void uart1_init(int);
void uart1_send_byte(char);

#endif /* uart1.h  */
