/*Access peripheral of the L476RG u controller*/

#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
//include a string header here
//include a standard io header here

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5


int main()
{
	/*Enable FPU*/
	fpu_enable();

    /*initialize UART*/
	debug_uart_init();
	//timebase_init();
	RCC->AHB2ENR |= GPIOAEN;
	GPIOA-> MODER |= (1U<<10);
	GPIOA-> MODER &= ~(1U<<11);
	while(1)
	{
	  systickDelayMs(1000);
      printf("A second  passed!\n\r");
      GPIOA->ODR ^=LED_PIN;

	}
}
