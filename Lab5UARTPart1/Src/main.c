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
	//GPIO-> MODER |= (1U<<10);
	//GPIO-> MODER &= ~(1U<<11);
	while(1)
	{
      printf("Hello from STM32\n\r");
      for(int i = 0; i < 9000; i++){}
	}
}
