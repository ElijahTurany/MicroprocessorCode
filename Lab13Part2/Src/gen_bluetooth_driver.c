
/*This file is gen-bluetooth_Criver.c*/
#include "gen_bluetooth_driver.h"
#include <stdint.h>

#define GPIOAEN			(1U<<0)
#define UART1EN			(1U<<14)
#define UART2EN			(1U<<17)

/*bit 3 is TE*/
#define CR1_TE			(1U<<3)
//Bit 0 is UE
#define CR1_UE			(1U<<0)
//bit 2 is RE
#define CR1_RE			(1U<<2)
//bit 7 in USART_ISR
#define ISR_TXE			(1U<<7)


#define SYS_FREQ		4000000
#define APB1_CLK		SYS_FREQ

#define SYSTICK_LOAD_VAL 	SYS_FREQ
#define CTRL_ENABLE		(1U<<0)
#define CTRL_COUNTFLAG 	(1U<<16)
#define CTRL_CLKSRC	(1U<<2)

#define DBG_UART_BAUDRATE	9600

static void uart_write(int ch);
static uint16_t	compute_uart_bd(uint32_t periph_clk, uint32_t baudrate);

static void uart_write(int ch){
		//make sure transmitter data register is empty
		while(!(USART2->ISR & ISR_TXE)){}
		//write to transmit data register
		USART2->TDR = (ch & 0xFF);
}

int __io_putchar(int ch){
	uart_write(ch);
	return ch;
}

void debug_uart_init(void)
{
	//enable clock access to GPIOA
	RCC->AHB2ENR |= GPIOAEN;

	//set the mode of PA2 (USART2_TX) to AF mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |=(1U<<5);

	//set the mode of PA3 (USART2_RX) to AF mode
	GPIOA->MODER &=~ (1U<<6);
	GPIOA->MODER |= (1U<<7);

	//Input(00), Output (01), AlterFUnc(10), Analog(11)
	//set PA2 = USART2_TX and PA.3 = USART2_RX

	//Set AF Type to AF& (UART2_TX) for PA2
	//Set AFRL to AFRO[0] instead
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	//Set AF Type to AF& (UART2_TX) for PA2
	//Set AFRL to AFRO[0] instead
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);

	//Enable clock access for UART2 in APB1ENR register
	RCC->APB1ENR1 |= UART2EN;
	//RCC->APB1ENR1 |= RCC_APB1ENR1_USART2EN; //Enable USART_2

	//Disable UART Module (USART Enable)
	USART2->CR1 &=~CR1_UE;

	//configure UART baud rate
	//uart_set_baudrate(APB1_CLK, DBG_UART_BAUTRDADE;
	USART2-> BRR = compute_uart_bd(APB1_CLK, DBG_UART_BAUDRATE);

	//configure transfer direction (Transmit Enable)
	USART2->CR1 = (CR1_TE | CR1_RE);

	//enable interrupt in the NVIC
	NVIC_EnableIRQ(USART2_IRQn);

	//Enable uart moduel (USART Enable)
	USART2->CR1 |= CR1_UE;
}



void bluetooth_uart_init(void)
{
	//enable clock access to GPIOA
	RCC->AHB2ENR |= GPIOAEN;

	//set the mode of PA0 (USART1_TX) to AF mode
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	//Set the mode of PA10 (USART1_RX) to AF mode
	GPIOA->MODER &=~(1U<<20);
	GPIOA->MODER |=(1U<<21);

	//Set AF type to AF7*UART1_TX) for PA9
	//Set AFRH to AFR[1] instead
	GPIOA->AFR[1] |=(1U<<4);
	GPIOA->AFR[1] |=(1U<<5);
	GPIOA->AFR[1] |=(1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	//Set AF type to AF7*UART1_RX) for PA10
	//Set AFRH to AFR[1] instead
	GPIOA->AFR[1] |=(1U<<8);
	GPIOA->AFR[1] |=(1U<<9);
	GPIOA->AFR[1] |=(1U<<10);
	GPIOA->AFR[1] &=~(1U<<11);

	//Enable clock access for UART1 in APB2ENR regiester
	RCC->APB2ENR |= UART1EN;

	//Disable UART Moudle (USART1)
	USART1->CR1 &=~CR1_UE;

	//configure uart baud rate
	USART1-> BRR = compute_uart_bd(APB1_CLK, DBG_UART_BAUDRATE);

	//configure transfer direction (Transmit enable)
	USART1->CR1 = (CR1_TE | CR1_RE);

	//Enable interrupt in the NVIC
	NVIC_EnableIRQ(USART1_IRQn);

	//Enable UART MOdule (USART Enable)
	USART1->CR1 |= CR1_UE;
}

static uint16_t compute_uart_bd(uint32_t periph_clk, uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}

void systick_delay_ms(uint32_t delay)
{
	//Reload systick with number of clocks per millisecond
	SysTick->LOAD = SYSTICK_LOAD_VAL/1000;

	//clear systick value register
	SysTick->VAL = 0;

	//enable systick and select to use an internal clock source
	SysTick->CTRL = CTRL_CLKSRC | CTRL_ENABLE;

	for ( int i = 0; i < delay; i++)
	{
		//wait until the COUNTFLAG is set
		while((SysTick->CTRL & CTRL_COUNTFLAG) ==0){}
	}

	SysTick->CTRL = 0;
}
