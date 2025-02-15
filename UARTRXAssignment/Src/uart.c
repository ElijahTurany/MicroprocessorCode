/*uart.c*/
#include <stdint.h>
#include "uart.h"

#define GPIOAEN		(1U<<0)//definem a bit position here to enable GPIOA
#define UART2EN		(1U<<17)//define a bit position to enable UART2, the MCU's peripheral

#define DBG_UART_BAUDRATE		 115200//define the baud rate to be 115200 here

/*Startup clock: after reset, the u-controller restarts by default with an internal
clock (MSI) for 4MHz; see section 3.11 Clocks and startup on the reference manual*/

#define SYS_FREQ				4000000//define a clock freq. here

#define APB1_CLK				SYS_FREQ
/*use the reference manual to find bit position for the following registers*/
/*USART_CR1 Transmit Enable*/
#define CR1_TE					(1U<<3) /*bit 3 is TE (Transmit Enable)*/
/*USART_CR1 USART Enable*/
#define CR1_UE                  (1U<<0) /*bit ? is UE (USART Enable)*/
/*USART_CR1 ISR TXE (Transmit Data Register Empty)*/
#define ISR_TXE					(1U<<7)  /*bit ? in USART_ISR*/

#define CR1_RE (1U<<2)
#define ISR_RNXE (1U<<5)

static void uart_write(int ch);
static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);
static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate);
static void uart2_write(USART_TypeDef *USARTx, uint8_t ch);

int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}

void uart2_rxtx_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB2ENR |=GPIOAEN;

	/*Set the mode of PA2 (USART2_TX) to AF mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set AF type to AF7(UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<8);
    GPIOA->AFR[0] |=(1U<<9);
    GPIOA->AFR[0] |=(1U<<10);
    GPIOA->AFR[0] &= ~(1U<<11);

    GPIOA->MODER &= ~(1U<<6);
    GPIOA->MODER |= (1U<<7);

    	/*Set AF type to AF7(UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<12);
    GPIOA->AFR[0] |=(1U<<13);
    GPIOA->AFR[0] |=(1U<<14);
    GPIOA->AFR[0] &= ~(1U<<15);
    /*Enable clock access to UART2EN in APB1ENR*/
    RCC->APB1ENR1 |=	UART2EN;

	/*Configure UART baud rate */
    uart_set_baudrate(APB1_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer direction  (Transmit Enable) */
    USART2->CR1 = CR1_TE | CR1_RE;/* bit 3*/

	/*Enable UART Module (USART Enable)*/
    USART2->CR1 |= CR1_UE; /*bit 0*/
}

char uart2_read(USART_TypeDef *USARTx) {
	while(!(USART2->ISR & ISR_RNXE)){}

	return USARTx-> RDR;
}

void debug_uart_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB2ENR |=GPIOAEN;

	/*Set the mode of PA2 (USART2_TX) to AF mode*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*Set AF type to AF7(UART2_TX)*/
    GPIOA->AFR[0] |=(1U<<8);  //typo here
    GPIOA->AFR[0] |=(1U<<9);  //typo here
    GPIOA->AFR[0] |=(1U<<10);  //typo here
    GPIOA->AFR[0] &= ~(1U<<11);  //typo here

    /*Enable clock access to UART2EN in APB1ENR*/
    RCC->APB1ENR1 |=	UART2EN;   //typo here

	/*Configure UART baud rate */
    uart_set_baudrate(APB1_CLK,DBG_UART_BAUDRATE);

	/*Configure transfer direction  (Transmit Enable) */
    USART2->CR1 = CR1_TE; //| CR1_RE/* bit 3*/

	/*Enable UART Module (USART Enable)*/
    USART2->CR1 |= CR1_UE; /*bit 0*/
}

static void uart_write(int ch){
	/*make sure transmitter data register is empty*/
		while(!(USART2->ISR & ISR_TXE)){}  //typo here
	    /*Write to transmit data register*/
		USART2->TDR = (ch & 0xFF);   //typo here
}

static void uart2_write(USART_TypeDef *USARTx, uint8_t ch){
	/*make sure transmitter data register is empty*/
		while(!(USARTx->ISR & ISR_TXE)){}  //typo here
	    /*Write to transmit data register*/
		USART2->TDR = (ch & 0xFF);   //typo here
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

static void uart_set_baudrate(uint32_t periph_clk, uint32_t baudrate)
{
	USART2-> BRR = compute_uart_bd(periph_clk,baudrate);
}
