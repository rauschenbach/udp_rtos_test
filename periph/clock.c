#include "clock.h"


/**
 * Установить частоту CPU 40 МГц
 */
void set_cpu_clock(void)
{
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);	/* Enable HSE clock oscillator */

    do {
	/* Good HSE clock */
	if (RST_CLK_HSEstatus() == SUCCESS) {

	    /* Select HSE clock as CPU_PLL input clock source. Set PLL multiplier to 5 */
	    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul5); // 40          


	    /* Enable CPU_PLL */
	    RST_CLK_CPU_PLLcmd(ENABLE);

	    /* Wait for PLL ready status */
	    if (RST_CLK_HSEstatus() == SUCCESS) {	/* Good CPU PLL */

		/* Set CPU_C3_prescaler to 1 */
		RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

		/* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
		RST_CLK_CPU_PLLuse(ENABLE);

		/* Core clock устанавливаем чтобы была равна 72 */
		RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

      		/* Enable clock of EEPROM_CNTRL */
		RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, ENABLE);

		/* С запасом - это до 100 МГц 3 цикла ожидания для EEPROM */
		EEPROM_SetLatency(EEPROM_Latency_3);

		/* Disable clock of EEPROM_CNTRL */
		RST_CLK_PCLKcmd(RST_CLK_PCLK_EEPROM, DISABLE);

#if 0
		tmp = MDR_BKP->REG_0E & 0xFFFFFFC0;
		tmp |= (7 << 3) | 7;	//Set fields LOW and Select RI to 7 (because clock of MCU core more 80 MHz)
		MDR_BKP->REG_0E = tmp;
#endif

		/* Вызываем функцию чтобы обновить глобальные переменные типа SystemCoreClock */
		SystemCoreClockUpdate();

	    } else {
		break;
	    }
	}
	/* Генератор 25 МГц  */
	RST_CLK_HSE2config(RST_CLK_HSE2_ON);

	if (RST_CLK_HSE2status() == SUCCESS) {
	    ;
	}


    } while (0);
}
