/*Access peripheral of the L476RG u controller*/

#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>
//include a string header here
//include a standard io header here

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5
char key;

int main()
{
	/*Enable FPU*/
	fpu_enable();

    /*initialize UART*/

	RCC->AHB2ENR |= GPIOAEN;

	uart2_rxtx_init();

	GPIOA-> MODER |= (1U<<10);
	GPIOA-> MODER &= ~(1U<<11);
	while(1)
	{
      key = uart2_read(USART2);

      if (key == '1') {
    	  GPIOA->ODR |= LED_PIN;
      }
      else {
    	  GPIOA->ODR &=~LED_PIN;
      }
      //for(int i = 0; i < 9000; i++){}
      printf("%c\n\r", key);
	}
}
