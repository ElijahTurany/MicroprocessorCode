#include "exti.h"
#include "stm32l476xx.h"

#define GPIOAEN (1U<<0)

#define SYSCFGEN (1U<<0)
#define EXTI_MR_MR1 (1U<<1)
#define EXTI_TR1 (1U<<1)
#define EXTI_PR1 (1U<<1)

void pa1_exti_init(void) {

	__disable_irq();
	RCC->APB2ENR |= GPIOAEN;
	RCC->APB2ENR |= SYSCFGEN;

	GPIOA->MODER &=~ (1U<<2);
	GPIOA->MODER &=~ (1U<<3);
	GPIOA->PUPDR &=~ (1U<<2);
	GPIOA->PUPDR |= (1U<<3);

	SYSCFG->EXTICR[0] = 0;

	EXTI->IMR1 |= EXTI_MR_MR1;
	EXTI->RTSR1 |= EXTI_TR1;
	EXTI->FTSR1 |= EXTI_TR1;

	NVIC_EnableIRQ(EXTI1_IRQn);

	__enable_irq();
}

uint32_t g_exti1_counter;
void exti1_callback(void) {
	g_exti1_counter++;
}

void EXTI1_IRQHandler(void) {
	exti1_callback();

	EXTI->PR1 |= EXTI_PR1;
}
