/******************************************************************************
 * ������� �������� ���, �������� ����������� ����� �.�. ����� 
 * ��� ������� ������� ����� �� ������ ����� (01-01-1970)
 * ��� ������� � ��������� �����
 *****************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "userfunc.h"
#include "uart1.h"



/**
 * ��������������� ��� log_printf
 */
#if !defined 	IAR_LOG_PRINTF
int log_printf(const char *fmt, ...)
{
    static char buf[512];
    int ret = 0;
    va_list list;

    va_start(list, fmt);
    ret = vsnprintf(buf, sizeof(buf), fmt, list);
    va_end(list);

    if (ret < 0) {
	return ret;
    }


    for(int i = 0; i < ret; i++)
	    uart1_send_byte(buf[i]);
    return ret;
}
#endif


/**
 * ������ � ���������� ����
 */
void print_data_hex(void *data, int len)
{
    int i;
    uint8_t byte;
    

    for (i = 0; i < len; i++) {
	byte = *((uint8_t*)data + i);
	if (i % 8 == 0)
	    log_printf(" ");
	if (i % /*16*/32 == 0) {
	    log_printf("\r\n");
	}

	log_printf("%02X ", byte);
    }
    log_printf("\r\n");
}

/**
 * ���������� MAC ����� - 6 ����
 */
void print_mac_addr(c8* str, uint8_t* addr)
{
 log_printf("%s %02X-%02X-%02X-%02X-%02X-%02X\n", str,
	addr[0], addr[1], addr[2],
	addr[3], addr[4], addr[5]);
}


/**
 * ���������� IP ����� - 4 ����
 */
void print_ip_addr(c8* str, uint8_t* addr)
{  
  log_printf("%s %d.%d.%d.%d\n", str, addr[0], addr[1], addr[2], addr[3]);
}

