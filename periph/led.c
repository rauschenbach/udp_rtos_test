#include "led.h"

/* Светодиод - пока на время отладки */
void led_init(void)
{
    PORT_InitTypeDef port;

    port.PORT_Pin = PORT_Pin_3;
    port.PORT_FUNC = PORT_FUNC_PORT;
    port.PORT_OE = PORT_OE_OUT;
    port.PORT_SPEED = PORT_SPEED_MAXFAST;
    port.PORT_MODE = PORT_MODE_DIGITAL;
    port.PORT_PULL_UP = PORT_PULL_UP_OFF;
    port.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    PORT_Init(MDR_PORTD, &port);
}

void led_on(void)
{
	MDR_PORTD->SETTX = PORT_Pin_3;
}

void led_off(void)
{
	MDR_PORTD->CLRTX = PORT_Pin_3;
}

/* Если установлен - сбросим  */
void led_toggle(void)
{
  if (MDR_PORTD->RXTX & PORT_Pin_3) {
	MDR_PORTD->CLRTX = PORT_Pin_3;
  } else {
	MDR_PORTD->SETTX = PORT_Pin_3;
  }
}

