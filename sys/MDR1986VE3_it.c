#include "MDR32F9Qx_config.h"
#include <MDR32F9Qx_uart.h>
#include <MDR32F9Qx_adc.h>
#include <MDR32F9Qx_bkp.h>
#include <MDR32F9Qx_port.h>
#include <MDR1986VE3_it.h>
#include <stdio.h>
#include "eth_common.h"

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
    unsigned int contr_reg;
    contr_reg = __get_CONTROL();
    if (contr_reg & 2) {
#if defined ( __CMCARM__ )
	__ASM MRS R0, PSP;
#else
	__ASM("MRS R0, PSP");
#endif
    } else {
#if defined ( __CMCARM__ )
	__ASM MRS R0, MSP;
#else
	__ASM("MRS R0, MSP");
#endif
    }
    //top of stack is in R0. It is passed to C-function.
#if defined ( __CMCARM__ )
    __ASM BL(Hard_fault_handler_c);
#else
    __ASM("BL (Hard_fault_handler_c)");
#endif

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1);
}


void Hard_fault_handler_c(unsigned int *hardfault_args)
{
    unsigned int stacked_r0;
    unsigned int stacked_r1;
    unsigned int stacked_r2;
    unsigned int stacked_r3;
    unsigned int stacked_r12;
    unsigned int stacked_lr;
    unsigned int stacked_pc;
    unsigned int stacked_psr;

    stacked_r0 = ((unsigned long) hardfault_args[0]);
    stacked_r1 = ((unsigned long) hardfault_args[1]);
    stacked_r2 = ((unsigned long) hardfault_args[2]);
    stacked_r3 = ((unsigned long) hardfault_args[3]);

    stacked_r12 = ((unsigned long) hardfault_args[4]);
    stacked_lr = ((unsigned long) hardfault_args[5]);
    stacked_pc = ((unsigned long) hardfault_args[6]);
    stacked_psr = ((unsigned long) hardfault_args[7]);

    log_printf("[Hard fault handler]\r\n");
    log_printf("R0 = 0x%x\r\n", stacked_r0);
    log_printf("R1 = 0x%x\r\n", stacked_r1);
    log_printf("R2 = 0x%x\r\n", stacked_r2);
    log_printf("R3 = 0x%x\r\n", stacked_r3);
    log_printf("R12 = 0x%x\r\n", stacked_r12);
    log_printf("LR = 0x%x\r\n", stacked_lr);
    log_printf("PC = 0x%x\r\n", stacked_pc);
    log_printf("PSR = 0x%x\r\n", stacked_psr);

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1);
}

/*******************************************************************************
* Function Name  : ADC_IRQHandler
* Description    : This function handles ADC global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ADC_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : BACKUP_IRQHandler
* Description    : This function handles BACKUP global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BKP_IRQHandler(void)
{
}

/*******************************************************************************
 * Function Name  : EXT_INT3_IRQHandler
 * Переход из 1 в 0 на стороне Eth микросхемы
 * на процесор идет через ключ (из 0 в 1-цу)
 ******************************************************************************/
void EXT_INT3_IRQHandler(void)
{
    eth5_irq_handler();
}

/*******************************************************************************
 * Function Name  : UART1_IRQHandler
 ******************************************************************************/
void UART1_IRQHandler(void)
{
}
