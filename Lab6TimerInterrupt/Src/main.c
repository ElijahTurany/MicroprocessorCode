/*Access peripheral of the L476RG u controller*/

#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
#include "bsp.h"
#include "timebase.h"
#include <stdio.h>
#include <string.h>
//include a string header here
//include a standard io header here

#define GPIOAEN		(1U<<0)
#define PIN5		(1U<<5)
#define LED_PIN		PIN5


int main()
{

	RCC->AHB2ENR |= GPIOAEN;
	/*Enable FPU*/
	fpu_enable();

    /*initialize UART*/
	debug_uart_init();
	//timebase_init();
	timebase_init();

	led_init();
	tim2_init();

	TIM2_IRQHandler();


	while(1)
	{

      //printf("*");

	}


}

