/*Access peripheral of the L476RG u controller*/


#include <BT_SPP_lib.h>
#include "stm32l476xx.h"
#include <stdio.h>//include a standard io header here

#define TX_BUFF_SIZE 4

extern portType debug_port;
extern portType bluetooth_port;

char data_buff[TX_BUFF_SIZE] = "Hi \n\r";

int main()
{
	debug_uart_init();
	bluetooth_uart_init();
	circular_buffer_init();

	while(1)
	{
		buffer_send_string(data_buff, bluetooth_port);
		systick_delay_ms(1000);
	}

}
