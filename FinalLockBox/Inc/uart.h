#ifndef __UART_H__
#define __UART_H__

#include "stm32l476xx.h"

int __io_putchar(int ch);
void debug_uart_init(void);
void uart_write(int ch);
uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
#endif


