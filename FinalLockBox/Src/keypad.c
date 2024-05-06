#include "keypad.h"
#include <stddef.h>

//Row and column declarations
#define COL1 1
#define COL2 2
#define COL3 3
#define COL4 4

#define ROW1 5
#define ROW2 6
#define ROW3 7
#define ROW4 8

uint8_t columns[4] = {COL1, COL2, COL3, COL4};
uint8_t rows[4] = {ROW1, ROW2, ROW3, ROW4};

//2D array of keypad values
int keypadVals[4][4] = {{49,50,51,65},
						{52,53,54,66},
						{55,56,57,67},
						{42,48,35,68}};

//Keypad pin initialization
void keypadInit(void) {
	/* Enable GPIOs clock */
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	//Rows
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3U<<(2*COL1) | 3U<<(2*COL2) | 3U<<(2*COL3) | 3U<<(2*COL4));  //clear PC 5,6,8,9 GPIO pins
	GPIOC->MODER  |=  1U<<(2*COL1) | 1U<<(2*COL2) | 1U<<(2*COL3) | 1U<<(2*COL4);   //  set Output(01)
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3U<<(2*COL1) | 3U<<(2*COL2) | 3U<<(2*COL3) | 3U<<(2*COL4));   //clear PC 5,6,8,9 GPIO pins
	GPIOC->OSPEEDR |=   2U<<(2*COL1) | 2U<<(2*COL2) | 2U<<(2*COL3) | 2U<<(2*COL4);  // Set Fast speed
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	GPIOC->OTYPER &= ~(1U<<COL1 | 1U<<COL2 | 1U<<COL3 | 1U<<COL4);       // Set Push-pull
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3U<<(2*COL1) | 3U<<(2*COL2) | 3U<<(2*COL3) | 3U<<(2*COL4));  // No pull-up, no pull-down

	//Columns
	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOC->MODER &= ~(3U<<(2*ROW1) | 3U<<(2*ROW2) | 3U<<(2*ROW3) | 3U<<(2*ROW4));  //clear PC 5,6,8,9 GPIO pins
	//GPIOC->MODER  |=  1<<(2*ROW1) | 1<<(2*ROW2) | 1<<(2*ROW3) | 1<<(2*ROW4);   //  set Output(01)
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOC->OSPEEDR &= ~(3U<<(2*ROW1) | 3U<<(2*ROW2) | 3U<<(2*ROW3) | 3U<<(2*ROW4));   //clear PC 5,6,8,9 GPIO pins
	GPIOC->OSPEEDR |=   2U<<(2*ROW1) | 2U<<(2*ROW2) | 2U<<(2*ROW3) | 2U<<(2*ROW4);  // Set Fast speed
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	GPIOC->OTYPER &= ~(1U<<ROW1 | 1U<<ROW2 | 1U<<ROW3 | 1U<<ROW4);       // Set Push-pull
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOC->PUPDR  &= ~(3U<<(2*ROW1) | 3U<<(2*ROW2) | 3U<<(2*ROW3) | 3U<<(2*ROW4));  //  pull-down
	GPIOC->PUPDR  |= (2U<<(2*ROW1) | 2U<<(2*ROW2) | 2U<<(2*ROW3) | 2U<<(2*ROW4));
}

int keypadScan(void) {
	//Writes high to all columns
	GPIOC->ODR |= (1<<COL1) | (1<<COL2) | (1<<COL3) | (1<<COL4);
	for(int i = 0; i < 4; i++) {
		//Finds the row the key is in
		if(GPIOC->IDR & (1<<rows[i])) {
			//Loops through every column, writing low to it and high to the rest
			for(int j = 0; j < 4; j++) {
				GPIOC->ODR |=(1<<COL1) | (1<<COL2) | (1<<COL3) | (1<<COL4);
				GPIOC->ODR &=~ (1<<columns[j]);
				//Returns the key when found
				if(!(GPIOC->IDR & (1<<rows[i]))) {
					return keypadVals[i][j];
				}
			}
		}
	}
	return 0;
}
