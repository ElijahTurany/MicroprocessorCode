
#include "stm32l476xx.h"
//#include "bsp.h"
#include "fpu.h"
#include <string.h>
#include <stdio.h>
#include "gen_bluetooth_driver.h"
#include "timebase.h"


int main()
{
	fpu_enable();
	debug_uart_init();

	while(1) {
		systickDelayMs(1000);
	 	printf("Test\n\r");
	}
}
