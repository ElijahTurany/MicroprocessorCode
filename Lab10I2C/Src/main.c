/*Access peripheral of the L476RG u controller*/
#include "stm32l476xx.h"
//#include "PLL.h"
#include "I2C.h"
#include <stdio.h>
#include <string.h>
#include "SysClock.h"
//#include "SysClock.h"

uint8_t Data_Receive[6];

void I2C_GPIO_Init(void){

	RCC->AHB2ENR  |=  RCC_AHB2ENR_GPIOBEN;

	// GPIO Mode: Input(00, reset), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER   &= ~( 3U<<(2*6) | 3U<<(2*7) );  // Clear Mode
	GPIOB->MODER   |=    2U<<(2*6) | 2U<<(2*7);    // Alternative Functionl

	GPIOB->AFR[0]  |= 0x44000000;   // Alternative Function 4 = I2C1

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR |=    3U<<(2*6) | 3U<<(2*7);    // High Speed

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR   &= ~( 3U<<(2*6) | 3U<<(2*7) );
	GPIOB->PUPDR   |=    1U<<(2*6) | 1U<<(2*7) ;   // Pull-up

	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	GPIOB->OTYPER  |= 1U<<6 | 1U<<7;  // Open Drain
}

int main(void) {
	System_Clock_Init();
	I2C_GPIO_Init();

	I2C_Initialization(I2C1);
	uint8_t slaveAddress;
	while(1) {
		slaveAddress = 0x48<<1;

		I2C_ReceiveData(I2C1,slaveAddress, Data_Receive, 1);
		for(int i = 0; i < 50000; i++); // Short delay
	}
}


