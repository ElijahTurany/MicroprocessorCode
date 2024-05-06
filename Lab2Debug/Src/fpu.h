#ifndef __FPU_H__
#define __FPU_H__
void fpu_enable(void);
#endif
/*fpu.c*/
#include "stm32l476xx.h"
void fpu_enable(void)
{
/*Enable floating point unit: Enable CP10 and CP11 for full access*/
SCB->CPACR |=(1<20);
SCB->CPACR |=(1<21);
SCB->CPACR |=(1<22);
SCB->CPACR |=(1<23);
}
