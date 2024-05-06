/*Access peripheral of the L476RG u controller*/
/*Place a generic file path ${ProjDirPath}*/

/*
 */

#include "stm32l476xx.h"
#include "bsp.h"

int main() {

	  // Default system clock 4 MHz
	    led_init();
		SERVO_Pin_Init();
		TIM2_CH1_Init(); // Timer to control LED, signal period = 10ms
		TIM5_CH1_Init(); // Timer to control Servo motor, signal period = 20ms

		/*Servo_Move_Right_90_Degree();
		delayMs(1000);
		Servo_Move_Left_90_Degree();
		delayMs(1000);
		Servo_Move_Center();*/

		led_dimmer();
}
