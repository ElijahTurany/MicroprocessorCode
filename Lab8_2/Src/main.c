/*Access peripheral of the L476RG u controller*/
#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
#include <stdio.h>
#include <string.h>

int main()
{

	/*Enable FPU*/
	uart2_tx_init();
	void pa4_adc1_init();
	pseudo_delay();
	start_conversion();

	while(1){
		sensor_value = adc_read();
		printf("Sensor value: %d\n\r", sensor_value);
	}
}

void pseudo_delay(void){
	int x;
	for(int i = 0; i < 8000; i++) {
		x++;
	}
}

