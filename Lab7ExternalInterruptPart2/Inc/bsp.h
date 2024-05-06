#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h>
#include <stdbool.h>

/*Include all function prototypes from bsp.c*/
/*such as servo pin initialization, timer1 
and timer 5 initalization*/


void led_init(void);
void tim2_init(void);
void TIM2_IRQHandler(void);
void led_toggle();
#endif
