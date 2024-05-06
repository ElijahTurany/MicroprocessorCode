
#include "stm32l476xx.h"
//#include "bsp.h"
#include "RTC.h"
#include "UART.h"
#include "timebase.h"
#include <string.h>
#include <stdio.h>

#define RTC_BUFF_LEN		30

uint8_t time_buffer[RTC_BUFF_LEN] = {0};
uint8_t date_buffer[RTC_BUFF_LEN] = {0};

void display_rtc_calendar(void);

uint32_t prev_time, curr_time;

int main()
{
	debug_uart_init();
	rtc_init();
	timebase_init();
	//LED_Init();

	/*Get current time*/
	prev_time = rtc_date_get_minute();

	while(1)
	{
	 	display_rtc_calendar();

	}
}

void display_rtc_calendar(void)
{
	/*Display time format : hh :mm : ss*/

	sprintf((char*)time_buffer,"%.2d:%.2d:%.2d", rtc_convert_bcd2bin(rtc_date_get_hour()),
			rtc_convert_bcd2bin(rtc_date_get_minute()),
			rtc_convert_bcd2bin(rtc_date_get_second()));

	printf(" Time: %.2d:%.2d:%.2d\n\r", rtc_convert_bcd2bin(rtc_date_get_hour()),
				rtc_convert_bcd2bin(rtc_date_get_minute()),
				rtc_convert_bcd2bin(rtc_date_get_second()));

	delay(1);
	sprintf((char*)date_buffer,"%.2d:%.2d:%.2d", rtc_convert_bcd2bin(rtc_date_get_month()),
				rtc_convert_bcd2bin(rtc_date_get_day()),
				2000 + rtc_convert_bcd2bin(rtc_date_get_year()));

	printf(" Day_Year: %.2d:%.2d:%.2d\n\r", rtc_convert_bcd2bin(rtc_date_get_month()),
					rtc_convert_bcd2bin(rtc_date_get_day()),
					rtc_convert_bcd2bin(rtc_date_get_year()));
}
