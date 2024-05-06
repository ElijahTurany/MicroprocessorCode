/*Access peripheral of the L476RG u controller*/
#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
#include "bsp.h"
#include "timebase.h"
#include <stdio.h>
#include <string.h>
#include "exti.h"
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

	led_init();

	pa1_exti_init();

	while(1)
	{}

}

