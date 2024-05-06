#include "timebase.h"
#include "stm32l476xx.h"

/*Define Time_base and interrupt registers*/
#define CTRL_ENABLE		(1U<<0)
#define CTRL_TICKINT	(1U<<1)
#define CTRL_CLCKSRC	(1U<<2)
#define CTRL_COUNTFLAG	(1U<<16)

/*we use the default start up clock at 4MHz to create 1 second tick*/
/*Please note this 1 second tick is not a real-time clock tick*/
#define ONE_SEC_LOAD    4000000
#define SYSTICK_LOAD_VAL 	4000/*Error here*/
#define MAX_DELAY		0xFFFFFFFF              /*Error here*/

#define TICK_FREQ  		1

volatile uint32_t g_curr_tick;
volatile uint32_t g_curr_tick_p;
volatile uint32_t tick_freq = 1;

void systickDelayMs(uint32_t delay) {
	SysTick-> LOAD = SYSTICK_LOAD_VAL;

	SysTick-> VAL = 0;

	SysTick-> CTRL = CTRL_CLCKSRC | CTRL_ENABLE;

	for(int i = 0; i < delay; i++) {
		while((SysTick->CTRL & CTRL_COUNTFLAG)==0){}
	}
	SysTick-> CTRL = 0;
}

/* delay in second*/
//void delay(uint32_t delay)
//{
//	uint32_t tickstart = get_tick();
//	uint32_t wait = delay;
//
//	if(wait < MAX_DELAY)
//	{
//		wait += (uint32_t)TICK_FREQ;
//	}
//	while((get_tick() - tickstart) < wait){} /*loop stuck here when true*/
//}
//
//
//uint32_t get_tick(void)
//{
//	__disable_irq();
//	g_curr_tick_p = g_curr_tick;
//	__enable_irq();
//	return g_curr_tick_p;
//}
//
//static void tick_increment(void)
//{
//	g_curr_tick += TICK_FREQ;
//}
//
void timebase_init(void){

    /* Disable a global interrupt */
	__disable_irq();

	/*load a value according to the clock cycle as we need a second tick from the clock*/
	SysTick->LOAD = ONE_SEC_LOAD-1; /*(count from zero from the system clock)*/

	  /*Clear systick current value register */
	SysTick ->VAL = 0;
	/*Select an Internal clock source */
	SysTick ->CTRL = CTRL_CLCKSRC;

	/*Enable an Interrupt */
	SysTick->CTRL |=CTRL_TICKINT;

	/*Enable systick */
	SysTick->CTRL |= CTRL_ENABLE;

	/* Enable a global interrupt */
	__enable_irq();

}
//
//void SysTick_Handler(void)
//{
//	tick_increment();
//}
