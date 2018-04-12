/**
  ******************************************************************************
  * @file    system_MDR1986VE3.c
  * @author  Andrey Sidorov
  * @version V1.4.0
  * @date    27/02/2013
  * @brief   CMSIS Cortex-M1 Device Peripheral Access Layer System Source File.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 Milandr</center></h2>
  ******************************************************************************
  * FILE system_MDR1986VE3.c
  */


/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @addtogroup MDR1986VE3 MDR1986VE3
 *  @{
 */

/** @defgroup MDR1986VE3_System_ARM MDR1986VE3 System ARM
  * @{
  */

/** @addtogroup System_Private_Includes System Private Includes
  * @{
  */

#include "MDR1986VE3.h"
#include "MDR32F9Qx_config.h"

#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"

					      /** @} *//* End of group System_Private_Includes */

/** @defgroup __MDR1986VE3_System_Private_Variables MDR1986VE3 System Private Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
uint32_t SystemCoreClock = (uint32_t) 8000000;	/*!< System Clock Frequency (Core Clock)
						 *   default value */

					      /** @} *//* End of group __MDR1986VE3_System_Private_Variables */

/** @defgroup __MDR1986VE3_System_Private_Functions MDR1986VE3 System Private Functions
  * @{
  */

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   None
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
    uint32_t cpu_c1_freq, cpu_c2_freq, cpu_c3_freq;
    uint32_t pll_mul;

    /* Compute CPU_CLK frequency */

    /* Determine CPU_C1 frequency */
    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t) 0x00000002) ==
	(uint32_t) 0x00000002) {
	cpu_c1_freq = HSE_Value;
    } else {
	cpu_c1_freq = HSI_Value;
    }

    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t) 0x00000001) ==
	(uint32_t) 0x00000001) {
	cpu_c1_freq /= 2;
    }

    /* Determine CPU_C2 frequency */
    cpu_c2_freq = cpu_c1_freq;

    if ((MDR_RST_CLK->CPU_CLOCK & (uint32_t) 0x00000004) ==
	(uint32_t) 0x00000004) {
	/* Determine CPU PLL output frequency */
	pll_mul = ((MDR_RST_CLK->PLL_CONTROL >> 8) & (uint32_t) 0x0F) + 1;
	cpu_c2_freq *= pll_mul;
    }

    /*Select CPU_CLK from HSI, CPU_C3, LSE, LSI cases */
    switch ((MDR_RST_CLK->CPU_CLOCK >> 8) & (uint32_t) 0x03) {
	uint32_t tmp;
    case 0:
	/* HSI */
	SystemCoreClock = HSI_Value;
	break;
    case 1:
	/* CPU_C3 */
	/* Determine CPU_C3 frequency */
	tmp = MDR_RST_CLK->CPU_CLOCK >> 4 & (uint32_t) 0x0F;
	if (tmp & (uint32_t) 0x8) {
	    tmp &= (uint32_t) 0x7;
	    cpu_c3_freq = cpu_c2_freq / ((uint32_t) 2 << tmp);
	} else {
	    cpu_c3_freq = cpu_c2_freq;
	}
	SystemCoreClock = cpu_c3_freq;
	break;
    case 2:
	/* LSE */
	SystemCoreClock = LSE_Value;
	break;
    default:			/* case 3 */
	/* LSI */
	SystemCoreClock = LSI_Value;
	break;
    }
}

/**
  * @brief  Setup the microcontroller system
  *         RST clock configuration to the default reset state
  *         Setup SystemCoreClock variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit(void)
{
    PORT_InitTypeDef init;

    /* Reset the RST clock configuration to the default reset state */

    /* Reset all clock but RST_CLK & BKP_CLK bits */
    MDR_RST_CLK->PER_CLOCK = (uint32_t) 0x8000010;
    MDR_RST_CLK->PER2_CLOCK &= (uint32_t) 0x00000000;

    /* Reset CPU_CLOCK bits */
    MDR_RST_CLK->CPU_CLOCK &= (uint32_t) 0x00000000;

    /* Reset PLL_CONTROL bits */
    MDR_RST_CLK->PLL_CONTROL &= (uint32_t) 0x00000000;

    /* Reset HSEON and HSEBYP bits */
    MDR_RST_CLK->HS_CONTROL &= (uint32_t) 0x00000000;

    /* Reset USB_CLOCK bits */
    MDR_RST_CLK->USB_CLOCK &= (uint32_t) 0x00000000;

    /* Reset ADC_MCO_CLOCK bits */
    MDR_RST_CLK->ADC_MCO_CLOCK &= (uint32_t) 0x00000000;

    SystemCoreClockUpdate();

#if 1
    /* Разрешим тактирование всех портов:  C, D, E, F */
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTC;
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTD;
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTE;
    MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTF;

    /* протактируем  PORTG, PORTH */
    MDR_RST_CLK->PER2_CLOCK |= RST_CLK_PCLK2_PORTG;
    MDR_RST_CLK->PER2_CLOCK |= RST_CLK_PCLK2_PORTH;

    PORT_StructInit(&init);
    
    /* Шина адреса ADDR2...ADDR12 на выход
     * PORTF пины PF5...PF15 
     * Альтернативная функция для выводов */
    init.PORT_Pin = PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 | PORT_Pin_8 | 
		    PORT_Pin_9 | PORT_Pin_10 | PORT_Pin_11 | PORT_Pin_12 |
		    PORT_Pin_13 | PORT_Pin_14 | PORT_Pin_15;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTF, &init);


    /* Шина адреса ADDR13 на выход
     * PORTD пин PD15 
     * Альтернативная функция для вывода */
    init.PORT_Pin = PORT_Pin_15;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTD, &init);


    /* Шина адреса ADDR14...ADDR21 на выход
     * PORTE пины PE0...PE7 
     * CS3 CS4 CS5 - адреса  ADDR22 ADDR23 ADDR24 - для микросхем ETH
     * Альтернативная функция для вывода */
    init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 |
		    PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 |
		    PORT_Pin_8 | PORT_Pin_9 | PORT_Pin_10;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTE, &init);


    /* Шина адреса A30...A31 на выход
     * PORTC пины PC13...PC14 
     * Альтернативная функция для вывода */
    init.PORT_Pin = PORT_Pin_13 | PORT_Pin_14;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTC, &init);


    /* Шина данных D0..D15 входы-выход
     * PORTH пины PH0...PH15
     * Переопределенная функция для вывода */
    init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | 
		    PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 | 
		    PORT_Pin_8 | PORT_Pin_9 | PORT_Pin_10 | PORT_Pin_11 | 
		    PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14 | PORT_Pin_15;
    init.PORT_FUNC = PORT_FUNC_OVERRID;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTH, &init);

   
    /* Шина данных D16..D31 входы-выход
     * PORTG пины PG0...PG15
     * Альтернативная функция для вывода */
    init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | 
		    PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 | 
		    PORT_Pin_8 | PORT_Pin_9 | PORT_Pin_10 | PORT_Pin_11 | 
		    PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14 | PORT_Pin_15;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTG, &init);
    

//---------------->

    /* PORTC - основн. ноги PC0 и PC1 -  ~WR и ~RD */
    init.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
    init.PORT_FUNC = PORT_FUNC_MAIN;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTC, &init);


    /* управление. PORTC пины 9..12 для управления шиной - альтерн ноги ~BE0...~BE3 */
    init.PORT_Pin =  PORT_Pin_9 | PORT_Pin_10 | PORT_Pin_11 | PORT_Pin_12;
    init.PORT_FUNC = PORT_FUNC_ALTER;
    init.PORT_MODE = PORT_MODE_DIGITAL;
    init.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTC, &init);

    /* Настройки параметров внешней памяти */
    MDR_EBC->CONTROL = 0x3002; /* wait state = 3 + RAM ON. При 72 МГц. */
    MDR_EBC->MEM_REGION[2] = 0x1205; /* RAM Cycles3 - Регион XRAM */
    MDR_EBC->MEM_REGION[3] = 0x1205; /* RAM Cycles3 - Регион ETH */
//    MDR_EBC->MEM_REGION[3] = 0x2421; /* RAM Cycles4 - Регион ETH */
#endif
}
