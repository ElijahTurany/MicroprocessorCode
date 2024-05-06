#include "bsp.h"
#include "stm32l476xx.h"
#define GPIOAEN (1U<<0)
#define GPIOCEN (1U<<2)
#define PIN5 (1U<<5)
#define LED_PIN PIN5
#define PIN13 (1U<<13)
#define BTN_PIN PIN13
void led_init(void)
{
/*Enable clock access to GPIOA*/
	//GPIOA->AHB2ENR |=GPIOAEN;
/*Set PA5 mode to output mode*/
	GPIOA->MODER |=  (1U<<10);
	GPIOA->MODER &= ~(1U<<11);
}
void led_on(void)
{
/*set PA5 high*/
	GPIOA->ODR |= (1U<<10);

}
void led_off(void)
{
/*set PA5 low*/
	GPIOA->ODR &=~(1U<<10);
}
void button_init(void)
{
/*Enable clock access to PORTC*/
	RCC->AHB2ENR |=GPIOCEN;
/*Set PC13 as an input pin*/
	GPIOC->MODER &= ~(3U<<26);
	GPIOC->MODER &= ~(3U<<27);
}
bool get_btn_state(void)
{
/*check if button is pressed and button is active low*/
if(GPIOC->IDR & BTN_PIN)
{
return false; //because of active low button
}
else
{
return true;
}
}
