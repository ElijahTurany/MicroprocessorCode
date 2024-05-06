/*Access peripheral of the L476RG u controller*/
#include "stm32l476xx.h"
#include "fpu.h"
#include "uart.h"
#include "adc.h"
#include <stdio.h>
#include <string.h>

//#define GPIOAEN		(1U<<0)
//#define PIN5		(1U<<5)
//#define LED_PIN		PIN5

void pseudo_delay(void);
int sensor_value;


int main()
{

	/*Enable FPU*/
	debug_uart_init();
	pa4_adc1_init();
	pseudo_delay();
	start_conversion();

	while(1){
		sensor_value = adc_read();
		printf("Sensor value: %d\n\r", sensor_value);
		pseudo_delay();
	}
}

void pseudo_delay(void){
	int x;
	for(int i = 0; i < 16000; i++) {
		x++;
	}
}



