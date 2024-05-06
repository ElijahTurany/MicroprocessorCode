#include <BT_SPP_lib.h>
#include <stdio.h>
#include <string.h>

/** STM32------------------> BLUETOOTH Module
*	3.3V ------------------>  	VCC
* 	GND--------------------> 	GND
*	PA9--(UART1_TX)--D8---->	RX
*	PA10-(UART1_Rx)--D2---->	TX
**/

/*Add an extern portType debug_port */

//Add and Exern Bluetooth_port

#define BT_port bluetooth_port
