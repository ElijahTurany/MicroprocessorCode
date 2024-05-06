#include "lcd.h"
#include "stm32l476xx.h"

//Control lines declaration, data lines are declared in lcd.h
#define REGSELECT 8
#define ENABLE 9

//Initializes LCD pins
void lcdInit(void) {
	RCC->AHB2ENR |=   RCC_AHB2ENR_GPIOBEN;

	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER &= ~(3U<<(2*REGSELECT) | 3U<<(2*ENABLE));  //clear PC 5,6,8,9 GPIO pins
	GPIOB->MODER  |=  (1U<<(2*REGSELECT) | 1U<<(2*ENABLE));  //  set Output(01)
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*REGSELECT) | 3U<<(2*ENABLE));   //clear PC 5,6,8,9 GPIO pins
	GPIOB->OSPEEDR |=  (2U<<(2*REGSELECT) | 2U<<(2*ENABLE));  // Set Fast speed
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR  &= ~(3U<<(2*REGSELECT) | 3U<<(2*ENABLE));


	// GPIO Mode: Input(00), Output(01), AlterFunc(10), Analog(11, reset)
	GPIOB->MODER &= ~(3U<<(2*DB0) | 3U<<(2*DB1) | 3U<<(2*DB2) | 3U<<(2*DB3) | 3U<<(2*DB4) | 3U<<(2*DB5) | 3U<<(2*DB6) | 3U<<(2*DB7));  //clear PC 5,6,8,9 GPIO pins
	GPIOB->MODER  |=  (1U<<(2*DB0) | 1U<<(2*DB1) | 1U<<(2*DB2) | 1U<<(2*DB3) | 1U<<(2*DB4) | 1U<<(2*DB5) | 1U<<(2*DB6) | 1U<<(2*DB7));  //  set Output(01)
	// GPIO Speed: Low speed (00), Medium speed (01), Fast speed (10), High speed (11)
	GPIOB->OSPEEDR &= ~(3U<<(2*DB0) | 3U<<(2*DB1) | 3U<<(2*DB2) | 3U<<(2*DB3) | 3U<<(2*DB4) | 3U<<(2*DB5) | 3U<<(2*DB6) | 3U<<(2*DB7));   //clear PC 5,6,8,9 GPIO pins
	GPIOB->OSPEEDR |=  (2U<<(2*DB0) | 2U<<(2*DB1) | 2U<<(2*DB2) | 2U<<(2*DB3) | 2U<<(2*DB4) | 2U<<(2*DB5) | 2U<<(2*DB6) | 2U<<(2*DB7));  // Set Fast speed
	// GPIO Output Type: Output push-pull (0, reset), Output open drain (1)
	// GPIO Push-Pull: No pull-up, pull-down (00), Pull-up (01), Pull-down (10), Reserved (11)
	GPIOB->PUPDR  &= ~(3U<<(2*DB0) | 3U<<(2*DB1) | 3U<<(2*DB2) | 3U<<(2*DB3) | 3U<<(2*DB4) | 3U<<(2*DB5) | 3U<<(2*DB6) | 3U<<(2*DB7));

}

//Function used to add delay between commands
void delayMs(int n){
	int i, j;
	for(i = 0 ; i < n; i++) {
		for(j = 0; j < 800; j++) {}
	}
}

//Writes to data lines given an input for each line
void writeToBus(int b7, int b6, int b5, int b4, int b3, int b2, int b1, int b0) {
	GPIOB->ODR &=~ ((1<<DB0) | (1<<DB1) | (1<<DB2) | (1<<DB3) | (1<<DB4) | (1<<DB5) | (1<<DB6) | (1<<DB7));
	GPIOB->ODR |= ((b0<<DB0) | (b1<<DB1) | (b2<<DB2) | (b3<<DB3) | (b4<<DB4) | (b5<<DB5) | (b6<<DB6) | (b7<<DB7));
}

//Enable signal used to send commands and data, data is pushed after a high-to-low signal
void enableSignal(void) {
	GPIOB->ODR |= (1<<ENABLE);
	delayMs(1);
	GPIOB->ODR &=~ (1<<ENABLE);
}

//Sends a command given input for each data line
void sendCommand(int b7, int b6, int b5, int b4, int b3, int b2, int b1, int b0) {
	GPIOB->ODR &=~ (1<<REGSELECT);
	writeToBus(b7,b6,b5,b4,b3,b2,b1,b0);
	enableSignal();
	//Delay to ensure adequate space between commands
	delayMs(10);
}

//Sends a character given an 8bit value passed into it
void sendData(int val) {
	GPIOB->ODR |= (1<<REGSELECT);
	GPIOB->ODR &=~ ((1<<DB0) | (1<<DB1) | (1<<DB2) | (1<<DB3) | (1<<DB4) | (1<<DB5) | (1<<DB6) | (1<<DB7));
	GPIOB->ODR |= val;
	enableSignal();
	//Delay to ensure adequate space between data transmissions
	delayMs(10);
}

//Sends a string of 8 bit character values sequentially
void sendString(int string[], int size) {
	for (int i = 0; i < size; i++) {
		sendData(string[i]);
	}
}

void lcdSetup(void) {
	//Function set
	//0,0,1,8bitMode,2lineDisplay,5x8,x,x
	sendCommand(0,0,1,1,1,0,0,0);
	//Display on
	//0,0,0,0,1,displayOn,cursorOff,cursorBlinkOff
	sendCommand(0,0,0,0,1,1,0,0);
	//Clear Screen
	sendCommand(0,0,0,0,0,0,0,1);
	//Set cursor position
	//1,line1,position0
	sendCommand(1,0,0,0,0,0,0,0);
}

void clearScreen(void) {
	//Clear Screen
	sendCommand(0,0,0,0,0,0,0,1);
	//line1
	sendCommand(1,0,0,0,0,0,0,0);
}

void lineOne(void) {
	//1,line1,position0
	sendCommand(1,0,0,0,0,0,0,0);
}

void lineTwo(void) {
	//1,line2,position0
	sendCommand(1,1,0,0,0,0,0,0);
}

//Given ASCII code from keypad, display appropriate LCD char
//(*) and (#) and not displayed
void displayInput(int scanVal) {
	switch(scanVal) {
		case 48:
			sendData(char0);
			break;
		case 49:
			sendData(char1);
			break;
		case 50:
			sendData(char2);
			break;
		case 51:
			sendData(char3);
			break;
		case 52:
			sendData(char4);
			break;
		case 53:
			sendData(char5);
			break;
		case 54:
			sendData(char6);
			break;
		case 55:
			sendData(char7);
			break;
		case 56:
			sendData(char8);
			break;
		case 57:
			sendData(char9);
			break;
		case 65:
			sendData(charA);
			break;
		case 66:
			sendData(charB);
			break;
		case 67:
			sendData(charC);
			break;
		case 68:
			sendData(charD);
			break;
	}
}
