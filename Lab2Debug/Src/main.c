/*Access peripheral of the L476RG u controller*/
/*Place a generic file path ${ProjDirPath}*/

#include "stm32l476xx.h"
#include "bsp.h"
#include <stdio.h>
#define PIN13 (1U<<13)
#define BTN_PIN PIN13
uint8_t btn_state;

int sum;

//int add(int num1, int num2) {
//	int temp = 0;
//	temp = num1+num2;
//	return temp;
//}

int main(void)
{

	button_init();
	//int total;
	//for(int i = 0; i < 10; i++) {
		//total += i;
	//}

	//sum = add(10,20);

	 while(1)
	 {
		 if(GPIOC->IDR & BTN_PIN)
		 {
			 btn_state = 0; //because of active low
		 }
		 else
		 {
			 btn_state = 1 ;
		 }
		 for (int i = 0 ; i < 90000 ; i++){}
	 }
}




