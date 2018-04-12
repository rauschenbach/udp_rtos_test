#include <stdarg.h>
#include <stdio.h>
#include "uart1.h"


/**
 * Запустить порт UART0
 */ 
void uart1_init(int baud)
{
    PORT_InitTypeDef PortInit;
    UART_InitTypeDef UART_InitStructure;

    /* Enables clock on PORTC */
  //  RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);


    /* Fill PortInit structure */
    PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
    PortInit.PORT_PD = PORT_PD_DRIVER;
    PortInit.PORT_GFEN = PORT_GFEN_OFF;
    PortInit.PORT_FUNC = PORT_FUNC_MAIN;
    PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE = PORT_MODE_DIGITAL;

    /* Configure PORTC pins 3 (UART0_TX) as output */
    PortInit.PORT_OE = PORT_OE_OUT;
    PortInit.PORT_Pin = PORT_Pin_3;
    PORT_Init(MDR_PORTC, &PortInit);


    /* Configure PORTC pins 4 (UART0_RX) as input */
    PortInit.PORT_OE = PORT_OE_IN;
    PortInit.PORT_Pin = PORT_Pin_4;
    PORT_Init(MDR_PORTC, &PortInit);

    /* Enables the CPU_CLK clock on UART2 */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);

    /* Set the HCLK division factor = 1 for UART2 */
    UART_BRGInit(MDR_UART1, UART_HCLKdiv8);


    /* Initialize UART_InitStructure */
    UART_InitStructure.UART_BaudRate = baud;
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    UART_InitStructure.UART_StopBits = UART_StopBits1; // 2 стоп бита!
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE;

    /* Configure UART1 parameters */
    UART_Init(MDR_UART1, &UART_InitStructure);

     /* Enable sender and receiver interrupts */
    UART_ITConfig (MDR_UART1,/* UART_IT_TX | */UART_IT_RX, ENABLE);
    NVIC_EnableIRQ(UART1_IRQn);    

    /* Enables UART1 peripheral */
    UART_Cmd(MDR_UART1, ENABLE);
}


/**
 * Send Data to UART2 
 */
void uart1_send_byte(char byte)
{
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_TXFE) != SET);
    UART_SendData(MDR_UART1, byte);
}
