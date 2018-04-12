/************************************************************************************
 *
 * 	Echo server lwip+freertos+udp test
 *  	Copyright (c) rauschenbach 
 *      sokareis@mail.ru
 *
 ***********************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uart1.h"
#include "clock.h"
#include "main.h"


const char *str0 = "Milandr  1986VE3 uController";

static void prvSetupHardware(void);

int main(void)
{
    prvSetupHardware();

    eth_create_task();    
    vTaskStartScheduler();
    for (;;);
}

/**
 * Настройка часов, уартов и пр.
 */
static void prvSetupHardware(void)
{
    /* 40 МГц eth5 должен работать!  */
    set_cpu_clock();

    /* Светодиод */
    led_init();
    led_on();
    led_off();
    
    /* Create tcp_ip stack thread */
    tcpip_init(NULL, NULL);    

     /* Задачу для ARP */
    eth_arp_timer_start();
}


/**
 * Переполнение стека
 */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName)
{
    /* Check pcTaskName for the name of the offending task, or pxCurrentTCB
       if pcTaskName has itself been corrupted. */
    (void) pxTask;
    (void) pcTaskName;
    for (;;);
}


/**
 * will only be called if
 * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
 * function that will get called if a call to pvPortMalloc() fails.
 * pvPortMalloc() is called internally by the kernel whenever a task, queue,
 * timer or semaphore is created.  It is also called by various parts of the
 * demo application.  If heap_1.c or heap_2.c are used, then the size of the
 * heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
 * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
 * to query the size of free heap space that remains (although it does not
 *  provide information on how the remaining heap might be fragmented). 
*/
void vApplicationMallocFailedHook(void)
{
    taskDISABLE_INTERRUPTS();
    for (;;);
}


/**
 * Idle Task Function
 * Можно ставить слипы
 */
void vApplicationIdleHook(void)
{

}

/**
  * This function will be called by each tick interrupt if
  * configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
  *     added here, but the tick hook is called from an interrupt context, so
  *     code must not attempt to block, and only the interrupt safe FreeRTOS API
  *     functions can be used (those that end in FromISR()). 
  */
void vApplicationTickHook(void)
{
    systick_func();
}

