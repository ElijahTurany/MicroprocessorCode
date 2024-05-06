
#include "stm32l476xx.h"

void GPIO_Init(void)
{
	/*Four pins to control the stepper motor:
	 * PC 5, PC 6, PC 8, and PC 9*/
	#define PIN1 5
	#define PIN2 6
	#define PIN3 8
	#define PIN4 9

	#define MPORT GPIOC
	#define DELAY  15000 //60000

	/* Enable GPIOs clock */
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOCEN;

	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	MPORT->MODER &= ~(3<<(2*PIN1) | 3<<(2*PIN2) | 3<<(2*PIN3) | 3<<(2*PIN4));  //clear PC 5,6,8,9 GPIO pins
	MPORT->MODER  |=  1<<(2*PIN1) | 1<<(2*PIN2) | 1<<(2*PIN3) | 1<<(2*PIN4);   //  set Output(01)

	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	MPORT->OSPEEDR &= ~(3<<(2*PIN1) | 3<<(2*PIN2) | 3<<(2*PIN3) | 3<<(2*PIN4));   //clear PC 5,6,8,9 GPIO pins
	MPORT->OSPEEDR |=   2<<(2*PIN1) | 2<<(2*PIN2) | 2<<(2*PIN3) | 2<<(2*PIN4);  // Set Fast speed

	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	MPORT->OTYPER &= ~(1<<PIN1 | 1<<PIN2 | 1<<PIN3 | 1<<PIN4);       // Set Push-pull

	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	MPORT->PUPDR  &= ~(3<<(2*PIN1) | 3<<(2*PIN2) | 3<<(2*PIN3) | 3<<(2*PIN4));  // No pull-up, no pull-down
}

void Half_Stepping_Clockwise(void)
{

unsigned char HalfStep[8] = {0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01, 0x09};

	int i, j;
	int k;
	uint32_t output;
	for (k = 0; k < 512; k++) {
	for(i = 0; i < 8; i++){
			for( j = 0; j < DELAY*3; j++); // delay
			output = MPORT->ODR;
			output &= ~( 1<< PIN1 | 1 << PIN2 | 1 << PIN3 | 1<< PIN4 );
			output |= ((HalfStep[i] & 0x8) >> 3) << PIN1;
			output |= ((HalfStep[i] & 0x4) >> 2) << PIN2;
			output |= ((HalfStep[i] & 0x2) >> 1) << PIN3;
			output |= ( HalfStep[i] & 0x1) << PIN4;
			MPORT->ODR = output;
		}
	}
}

void Half_Stepping_CounterClockwise(void)
{

unsigned char HalfStep[8] = {0x09, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0C, 0x08};

	int i, j;
	int k;
	uint32_t output;
	for (k = 0; k < 512; k++) {
	for(i = 0; i < 8; i++){
			for(j = 0; j < DELAY; j++); // delay
			output = MPORT->ODR;
			output &= ~( 1<< PIN1 | 1 << PIN2 | 1 << PIN3 | 1<< PIN4 );
			output |= ((HalfStep[i] & 0x8) >> 3) << PIN4;
			output |= ((HalfStep[i] & 0x4) >> 2) << PIN3;
			output |= ((HalfStep[i] & 0x2) >> 1) << PIN2;
			output |= ((HalfStep[i] & 0x1) >> 0) << PIN1;
			MPORT->ODR = output;
	}
  }
}

void Full_Stepping_CounterClockwise(void)
{

unsigned char FullStep[4] = {0x08, 0x04, 0x02, 0x01};

	int i, j;
	int k;
	uint32_t output;
	for (k = 0; k < 512; k++) {
	for(i = 0; i < 4; i++){
			for(j = 0; j < DELAY; j++); // delay
			output = MPORT->ODR;
			output &= ~( 1<< PIN1 | 1 << PIN2 | 1 << PIN3 | 1<< PIN4 );
			output |= ((FullStep[i] & 0x8) >> 3) << PIN1;
			output |= ((FullStep[i] & 0x4) >> 2) << PIN2;
			output |= ((FullStep[i] & 0x2) >> 1) << PIN3;
			output |= ( FullStep[i] & 0x1) << PIN4;
			MPORT->ODR = output;
		}
	}
}
