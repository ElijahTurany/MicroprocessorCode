#include "gen_bluetooth_driver.h"
#include <stdint.h>

#define GPIOAEN (1U<<0)
#define UART2EN (1U<<17)
#define UART1EN (1U<<14)

#define CR1_TE (1U<<3)
#define CR1_UE (1U<<0)
#define CR1_RE (1U<<2)
#define ISR_TXE (1U<<7)
#define ISR_RXNE (1U<<5)

#define SYS_FREQ 4000000
#define APB1_CLK SYS_FREQ

#define DBG_UART_BAUDRATE 9600

static void uart_write(int ch);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);

static void uart_write(int ch) {
	while(!(USART2->ISR & ISR_TXE)){}
	USART2->TDR = (ch &0xFF);
}

int __io_putchar(int ch) {
	uart_write(ch);
	return ch;
}

void debug_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB2ENR |=GPIOAEN;   //typo here

	/*Set the mode of PA2 (USART2_TX) to AF mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set the mode of PA3 (USART2_TX) to AF mode*/
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/*Set AF type to AF7(UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<8);  //typo here
    GPIOA->AFR[0] |=(1U<<9);  //typo here
    GPIOA->AFR[0] |=(1U<<10);  //typo here
    GPIOA->AFR[0] &= ~(1U<<11);  //typo here

    GPIOA->AFR[0] |=(1U<<12);  //typo here
    GPIOA->AFR[0] |=(1U<<13);  //typo here
    GPIOA->AFR[0] |=(1U<<14);  //typo here
    GPIOA->AFR[0] &= ~(1U<<15);  //typo here

    /*Enable clock access to UART2EN in APB1ENR*/
    RCC->APB1ENR1 |= UART2EN;   //typo here
    /*Enable UART Module (USART Enable)*/
    USART2->CR1 &=~ CR1_UE; /*bit 0*/

    USART2->BRR = compute_uart_bd(APB1_CLK,DBG_UART_BAUDRATE);

    USART2->CR1 = (CR1_TE | CR1_RE);

    NVIC_EnableIRQ(USART2_IRQn);

    USART2->CR1 |= CR1_UE;
}

void bluetooth_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB2ENR |=GPIOAEN;   //typo here

	/*Set the mode of PA9 (USART2_TX) to AF mode*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set the mode of PA10 (USART2_TX) to AF mode*/
	GPIOA->MODER &=~(1U<<20);
	GPIOA->MODER |= (1U<<21);

	/*Set AF type to AF7(UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<4);  //typo here
    GPIOA->AFR[0] |=(1U<<5);  //typo here
    GPIOA->AFR[0] |=(1U<<6);  //typo here
    GPIOA->AFR[0] &= ~(1U<<7);  //typo here

    GPIOA->AFR[0] |=(1U<<8);  //typo here
    GPIOA->AFR[0] |=(1U<<9);  //typo here
    GPIOA->AFR[0] |=(1U<<10);  //typo here
    GPIOA->AFR[0] &= ~(1U<<11);  //typo here

    /*Enable clock access to UART2EN in APB1ENR*/
    RCC->APB2ENR |= UART1EN;   //typo here
    /*Enable UART Module (USART Enable)*/
    USART1->CR1 &=~ CR1_UE; /*bit 0*/

    USART1->BRR = compute_uart_bd(APB1_CLK,DBG_UART_BAUDRATE);

    USART1->CR1 = (CR1_TE | CR1_RE);

    NVIC_EnableIRQ(USART1_IRQn);

    USART1->CR1 |= CR1_UE;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2-> BRR = compute_uart_bd(periph_clk,baudrate);
}

