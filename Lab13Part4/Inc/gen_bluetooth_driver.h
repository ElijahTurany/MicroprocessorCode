#ifndef GEN_BLUETOOTH_DRIVER_H_
#define GEN_BLUETOOTH_DRIVER_H_

#include "stm32l476xx.h"
void debug_uart_init(void);
void bluetooth_uart_init(void);
void systick_delay_ms(uint32_t delay);
#endif /* GEN_BLUETOOTH_DRIVER_H_ */
