#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUGGER_H__


#include <stdint.h>
#include "stm32l476xx.h"

typedef uint8_t portType;


#define UART_BUFFER_SIZE		20
#define INIT_VAL				 0

typedef struct
{
	unsigned char buffer[UART_BUFFER_SIZE];
	volatile uint32_t head;
	volatile uint32_t tail;
}circular_buffer;

void circular_buffer_init(void);
void buffer_clear(portType uart);
int buffer_peek(portType uart);
int buffer_read(portType uart);
void buffer_write(unsigned char c, portType uart);
int is_data(portType uart);
int8_t is_response(char *str);
void get_strs(uint8_t number_of_chars, char *dest_buffer);
void buffer_send_string(const char *s, portType uart);


#endif
