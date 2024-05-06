#include "exti.h"
#include "stm32l476xx.h"
#include "bsp.h"

#define EXTI_PIN 13

uint32_t g_exti1_counter;

void pc13_exti_init(void) {

	__disable_irq();

	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	GPIOC->MODER &=~ (3U<<(2*EXTI_PIN));
	GPIOC->PUPDR &=~ (3U<<(2*EXTI_PIN));

	SYSCFG->EXTICR[3] &=~ SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |=  SYSCFG_EXTICR4_EXTI13_PC;

	EXTI->IMR1 |= EXTI_IMR1_IM13;

	EXTI->FTSR1 |= EXTI_FTSR1_FT13;

	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	__enable_irq();

}

void exti15_10_callback(void) {
	g_exti1_counter++;
	led_toggle();
}

void EXTI15_10_IRQHandler(void) {
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	exti15_10_callback();
	EXTI->PR1 |= EXTI_PR1_PIF13;
}
