/*Access peripheral of the L476RG u controller*/
/*Place a generic file path ${ProjDirPath}*/

/*
 * This program drives a stepper motor using the half-step mode
 * The ULA2803A IC is a transistor array
 * capable of driving a stepper motor up
 * to 5V/500mA continuous current.
 * The enable pins of the half-bridge (inside ULA2803A IC) are connected to Port C
 * (PC.5, PC.6, PC8, and PC.9)
 * The four output pins of the motor driver device are connected to
 * the terminals marked IN1, IN2, IN3, and IN4.  Use 5V on the
 * board to power up the stepper driver board
 */

#include "stm32l476xx.h"
#include "fpu.h"
#include "gpio.h"
#include "PLL.h"

#define PIN1 5
#define PIN2 6
#define PIN3 8
#define PIN4 9

#define MPORT GPIOC

#define DELAY  150000 

int main()
{
	/*Enable FPU*/
	fpu_enable();

	/*Initialize GPIO*/
	GPIO_Init();

	/*Initialize System Clock to 80 MHz*/
	System_Clock_Init();

	/*Four pins to control the stepper motor: PC 5, PC 6,
	 * PC 8, and PC 9*/
	Half_Stepping_Clockwise();
	Half_Stepping_CounterClockwise();
	


}


