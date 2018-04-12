/**
  ******************************************************************************
  * @file    types.h
  * @author  Phyton Application Team
  * @version V2.0.0
  * @date    09.09.2010
  * @brief   This file contains some auxiliary type definitions.
  ******************************************************************************
  * <br><br>
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 Phyton</center></h2>
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TYPES_H
#define __TYPES_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>


#define HWREG(x)                                                              \
        (*((volatile unsigned /*long*/int *)(x)))
#define HWREGH(x)                                                             \
        (*((volatile unsigned short *)(x)))
#define HWREGB(x)                                                             \
        (*((volatile unsigned char *)(x)))
#define HWREGBITW(x, b)                                                       \
        HWREG(((unsigned long)(x) & 0xF0000000) | 0x02000000 |                \
              (((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITH(x, b)                                                       \
        HWREGH(((unsigned long)(x) & 0xF0000000) | 0x02000000 |               \
               (((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITB(x, b)                                                       \
        HWREGB(((unsigned long)(x) & 0xF0000000) | 0x02000000 |               \
               (((unsigned long)(x) & 0x000FFFFF) << 5) | ((b) << 2))


#define         ALL_PORT_PINS   (PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | \
                                 PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6 | PORT_Pin_7 | \
                                 PORT_Pin_8 | PORT_Pin_9 | PORT_Pin_10 | PORT_Pin_11 |\
				 PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14 | PORT_Pin_15)


#define         ALL_PORTS_CLK   (RST_CLK_PCLK_PORTA | RST_CLK_PCLK_PORTB | \
                                  RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTD | \
                                  RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTF)




typedef unsigned char const    ucint8_t;
typedef volatile unsigned char vuint8_t;
typedef volatile unsigned long vuint32_t;

//typedef enum {FALSE = 0, TRUE = !FALSE} bool;

#endif /* __TYPES_H */
