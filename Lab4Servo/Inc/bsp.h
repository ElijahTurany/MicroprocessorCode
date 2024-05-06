#ifndef __BSP_H__
#define __BSP_H__

#include <stdint.h>
#include <stdbool.h>

/*Include all function prototypes from bsp.c*/
/*such as servo pin initialization, timer1 
and timer 5 initalization*/

void Servo_Move_Left_90_Degree();
void Servo_Move_Center();
void Servo_Move_Right_90_Degree();
void led_dimmer();
void delayMs(int n);
void led_init(void);
void led_on(void);
void led_off(void);
void button_init(void);
bool get_btn_state(void);
void SERVO_Pin_Init(void);
void TIM2_CH1_Init(void);
void TIM5_CH1_Init(void);
#endif
