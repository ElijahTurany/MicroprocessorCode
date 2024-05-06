#include <BT_SPP_lib.h>
#include <stdio.h>
#include<string.h>

/** STM32------------------------> BLUETOOTH Module
 * 3.3V-------------------------->	 	VCC
 * GND---------------------------> 		GND
 * PA9--(UART1_TX)--D8-----------> 		RX
 * PA10-(UART1_RX)--D2-----------> 		TX
 **/

extern portType debug_port;

extern portType bluetooth_port;

#define BT_port bluetooth_port


