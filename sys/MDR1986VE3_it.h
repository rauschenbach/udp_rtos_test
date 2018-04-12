/**
  ******************************************************************************
  * @file    MDR1986VE3_it.h
  * @author  Milandr Application Team
  * @version V3.0.0
  * @date    08.09.2013
  * @brief   This file contains all the prototypes of interrupt handler functions
  ******************************************************************************
  * <br><br>
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Milandr SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 Milandr</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32F9Qx_H
#define __MDR32F9Qx_H

/* Includes ------------------------------------------------------------------*/
#include <MDR32F9Qx_config.h>
#include "types.h"

/** @addtogroup __MDR1986VE3_Eval_Demo MDR1986VE3 Demonstration Example
  * @{
  */

/** @defgroup Interrupt_Service_Routines Interrupt Service Routines
  * @{
  */

/** @defgroup Interrupt_Service_Exported_Types Interrupt Service Exported Types
  * @{
  */
/** @} */ /* End of group Interrupt_Service_Exported_Types */

/** @defgroup Interrupt_Service_Exported_Macros Interrupt Service Exported Macros
  * @{
  */

/* UART event mask as of interest by CDC PSTN line state notifications */
#define UART_LINE_STATE_EVENTS		(UART_IT_OE | UART_IT_BE | UART_IT_PE | UART_IT_FE | \
																	 UART_IT_DSR | UART_IT_DCD | UART_IT_RI)


void UART2_IRQHandler(void);
void SysTick_Handler(void);


#endif /* __MDR32F9Qx_H */

