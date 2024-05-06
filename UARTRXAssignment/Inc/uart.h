#ifndef __UART_H__
#define __UART_H__

#include "stm32l476xx.h"
void debug_uart_init(void);
void uart2_rxtx_init(void);
char uart2_read(USART_TypeDef *USARTx);
#endif


