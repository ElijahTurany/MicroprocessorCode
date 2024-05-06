/*ADC.c*/

#include "adc.h"
 //replace x with correct bit positions
#define GPIOAEN			(1U<<2)
#define ASCR_ASC4		(1U<<4)
#define ADCEN           (1U<<13)
#define CFGR_CONT		(0U<<13)
#define SEQ_LEN_1		0
#define ISR_EOC			(1U<<2)


/*PA4 can be used for the ADC channel: ADC12_IN9; this means
 * ADC1_IN9 and ADC2_IN9 are multiplexed to PA4 */

void pa4_adc1_init(void)
{
	/*Configure ADC GPIO Pin*/
	/*Enable clock access to GPIOA*/
	RCC->AHB2ENR   |= GPIOAEN;  //error here

	/*Set PA4 to Analog Pin*/
	GPIOA->MODER	|=(1U<<2);   //error here
	GPIOA->MODER	|=(1U<<2);   //error here

	/*Connect GPIO analog switch to ADC input (ASCR register)*/
	/*Analog Switch Control Register (ASCR) to PA4*/
	GPIOA->ASCR     |= ASCR_ASC4;   //error here

	/*Configure ADC module*/
	RCC->AHB2ENR	|= ADCEN;

	/*Enable clock access to ADC_CCR module
	 * (ADC common control register)*/
	/*Set ADC common clock ; use system clock divided by 2*/
	ADC123_COMMON->CCR  &= ~(1U<<16);
	ADC123_COMMON->CCR  |=  (1U<<17);

	/*Set Single Conversion (ADC_CFGR)*/
	ADC1->CFGR		&=~ CFGR_CONT;		//error here

	/*Set sequence length (SQR1 register to define number of conversion)*/
	ADC1->SQR1		= SEQ_LEN_1;

	/*Set ADC channel */
	ADC1->SQR1		|=(1U<<6);   //replace x with correct bit
	ADC1->SQR1		&=~(1U<<7);
	ADC1->SQR1		&=~(1U<<8);
	ADC1->SQR1		|=(1U<<9);  //0b1001 indicates ADC channel 9 is used
	//ADC1->SQR1		&=~(1U<<10);

	/*Disable ADC deep power down (ADC_CR register)*/
	ADC1->CR	&=~(1U<<29);

	/*Enable ADC internal regulator (unique to STM32L series uC)*/
	ADC1->CR	|=(1U<<28);

	/*Start ADC Calibration (ADCAL)*/
	ADC1->CR	|=(1U<<31);

	/*Enable the ADC*/
	ADC1->CR	|=(1U<<0);
}

void start_conversion(void)
{
	/*ADC_CR register*/
	ADC1->CR	|=(1U<<2);
}

uint32_t adc_read(void)
{
	/*Wait for end of conversion flag (ADC_ISR at bit 2)*/
	while (!(ADC1-> ISR  & ISR_EOC)){}   //error here

	/*Clear EOC flag by writing 1 to bit 2*/
	ADC1-> ISR  &=~ ISR_EOC;  //error here

	/*Read results*/
	return (ADC1->DR);
}
