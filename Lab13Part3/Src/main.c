
#include "stm32l476xx.h"
//#include "bsp.h"
#include "fpu.h"
#include <string.h>
#include <stdio.h>
#include "gen_bluetooth_driver.h"
#include "timebase.h"
#include "circular_buffer.h"


int main()
{
	fpu_enable();
	debug_uart_init();
	bluetooth_uart_init();
	circular_buffer_init();

	while(1) {
		buffer_send_string(data_buff,bluetooth_port);
		systickDelayMs(1000);
	 	//printf("Test\n\r");
	}
}
