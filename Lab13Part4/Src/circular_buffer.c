#include "circular_buffer.h"
#include "stm32l476xx.h"
#include <string.h>
#define CR1_RXNEIE		(1U<<5) //see usart's RX Interrupt enable
#define CR1_TXEIE		(1U<<7)

#define ISR_RXNE		(1U<<5)
#define ISR_TXE   		(1U<<7)

portType debug_port     = 0;
portType bluetooth_port = 1;


/*define Buffer for debug UART*/
circular_buffer rx_buffer2={{INIT_VAL},INIT_VAL,INIT_VAL};
circular_buffer tx_buffer2={{INIT_VAL},INIT_VAL,INIT_VAL};

/*define Buffer for BLUETOOTH UART*/
/*define rx and tx buffer with buffer size, head and tail number*/
circular_buffer rx_buffer1={{INIT_VAL}, INIT_VAL, INIT_VAL};   //missing code here
circular_buffer tx_buffer1={{INIT_VAL}, INIT_VAL, INIT_VAL};    //missing code here

/*define rx and tx buffer pointers for both debug and BLUETOOTH UART */
circular_buffer * _rx_buffer1;        //missing code here
circular_buffer * _tx_buffer1;		  //missing code here
circular_buffer * _rx_buffer2;
circular_buffer * _tx_buffer2;

/*initialize buffer */
void circular_buffer_init(void)
{
	/*Initialize buffer with de-referencing pointers (&)*/
	_rx_buffer1 = &rx_buffer1;        //missing code here
	_tx_buffer1 = &tx_buffer1;         //missing code here
	_rx_buffer2 = &rx_buffer2;
	_tx_buffer2 = &tx_buffer2;

	/*Initialize UART RX Not Empty Interrupt for USART1 and USART2*/
	USART1->CR1 |= CR1_RXNEIE;                                   //missing code here
	USART2->CR1 |= CR1_RXNEIE;									//missing code here
}


static void buff_store_char(unsigned char c, circular_buffer * buffer)
{
	int loc =(uint32_t) (buffer->head + 1)%UART_BUFFER_SIZE;
	/*Check if no overflow will occur*/
	if( loc != buffer->tail)
	{
		/*Store character*/
		buffer->buffer[buffer->head] = c;

		/*Update head*/
		buffer->head = loc;
	}
}

/*define buffer_clear function to clear buffer contents for BLUETOOTH UART*/
void buffer_clear(portType uart)
{
	if(uart == debug_port)   //missing a port type here
	{
		/*Set the content to '\0'*/
		memset(_rx_buffer1->buffer,'\0', UART_BUFFER_SIZE);
		_rx_buffer1->head = 0;
	}

	if(uart == debug_port)
		{
			/*Set the content to '\0'*/
			/*fill in the implementation when needed*/
		}
}

int buffer_peek(portType uart)
{
	if(uart == bluetooth_port)
	{
			if(_rx_buffer1->head == _rx_buffer1->tail)
			{
				return -1;
			}
			else
			{
				return _rx_buffer1->buffer[_rx_buffer1->tail];
			}
	}

	if(uart == debug_port)
		{
			if(_rx_buffer2->head == _rx_buffer2->tail)
			{
				/*Return code here to indicate buffer has no new data*/
				return -1;
			}
			else
			{
				return _rx_buffer2->buffer[_rx_buffer2->tail];
			}
		}
	else
	{
		return -1;
	}
}

int buffer_read(portType uart)
{
	if (uart == bluetooth_port)
	{
			if(_rx_buffer1->head == _rx_buffer1->tail)
			{
				return -1;
			}
			else
			{
				unsigned c = _rx_buffer1->buffer[_rx_buffer1->tail];
				_rx_buffer1->tail = (uint32_t)(_rx_buffer1->tail +1) % UART_BUFFER_SIZE;
				return c;
			}
	}

	else if (uart == debug_port)
		{
		if(_rx_buffer2->head == _rx_buffer2->tail)
				{
					/*buffer has no new data*/
					return -1;
				}
		else
				{   /*read new data at tail position into a variable called'c'*/
					unsigned c = _rx_buffer2->buffer[_rx_buffer2->tail];                    //error here
                    /*compute new tail position*/
					_rx_buffer2->tail = (uint32_t)(_rx_buffer2->tail+1) % UART_BUFFER_SIZE;  //error here
					return c;
				}
		}

	else
	{
		return -1;
	}
}

void buffer_write(unsigned char c, portType uart)
{
	if (uart == bluetooth_port)
	{
		int loc = (uint32_t) (_tx_buffer1->head + 1) % UART_BUFFER_SIZE;
		while(loc == _tx_buffer1->tail){}
		_tx_buffer1->buffer[_tx_buffer1->head] = c;
		_tx_buffer1->head = loc;

		USART1->CR1 |= CR1_TXEIE;
	}

	if (uart == debug_port)
		{
			int loc =(uint32_t)(_tx_buffer1->head + 1) % UART_BUFFER_SIZE;  //error here with a wrong write buffer position
			while(loc == _tx_buffer2->tail){}
			_tx_buffer2->buffer[_tx_buffer2->head] =c;
			_tx_buffer2->head = loc;

			/*Enable UART Transmit Interrupt*/
			USART2->CR1 |=CR1_TXEIE;
		}
}

int is_data(portType uart)
{
	if (uart == bluetooth_port)
	{
			return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer1->head - _rx_buffer1->tail)%UART_BUFFER_SIZE;
	}

	else if (uart == debug_port)
		{
			return (uint16_t)(UART_BUFFER_SIZE + _rx_buffer2->head - _rx_buffer2->tail)%UART_BUFFER_SIZE; //typo here
		}
	else
	{
		return -1;
	}
}

static void get_first_char(char *str)
{
	/*Make sure there is data in buffer*/

	while(!is_data(bluetooth_port)){}
	while(buffer_peek(bluetooth_port) !=str[0])
		{
			_rx_buffer1->tail = (_rx_buffer1->tail + 1)%UART_BUFFER_SIZE;
			while(!is_data(bluetooth_port)){}
		}
}

int8_t is_response(char *str)
{
	int curr_pos = 0;
	int len = strlen(str);
	while(curr_pos != len)
	{
		curr_pos = 0;
		get_first_char(str);

		while(buffer_peek(bluetooth_port) ==str[curr_pos])
		{
			curr_pos++;
			buffer_read(bluetooth_port);

			if(curr_pos == len)
			{
				/*Success*/
				return 1;
			}
			while(!is_data(bluetooth_port)){}
		}

	}
		return -1;
}

void get_strs(uint8_t number_of_chars, char *dest_buffer)
{
	for(int index = 0; index <number_of_chars; index++)
	{
		for(int i = 0; i< 9000; i++)
		{
			dest_buffer[index] = buffer_read(bluetooth_port);
		}
	}
}

void buffer_send_string(const char *s, portType uart)
{
	while(*s != '\0')
	{
		buffer_write(*s++, uart);
	}
}


/*UART and BLUETOOTH PORTs are used for interrupt mode for transmit
 * and receive operations instead of polling mode; and are executed 
 for every character being received/transmitted;
 * Therefore, the interrupts are executed in a callback function respectively */

void bluetooth_uart_callback(void)
{
	/*Check if RXNE (RX not empty)is raised and if RXNEIE is enabled*/
	if(((USART1->ISR & ISR_RXNE) !=0 ) && ((USART1->CR1 & CR1_RXNEIE) !=0))
		{
		  	  unsigned char c = USART1 ->RDR;
		  	  buff_store_char(c, _rx_buffer1);
		}

	/*Check if TXEIE is enabled*/
	if(((USART1->ISR & ISR_TXE) !=0 ) && ((USART1->CR1 & CR1_TXEIE) !=0))
	{
		if(tx_buffer1.head == tx_buffer1.tail)
		{
			  USART1->CR1 &=~ CR1_TXEIE;
		}
	    else
		{
			/*Get character from buffer*/
	    	unsigned char c = tx_buffer1.buffer[tx_buffer1.tail];
	    	/*update position*/
	    	tx_buffer1.tail = (tx_buffer1.tail + 1)%UART_BUFFER_SIZE;
	    	/*Transmit character*/
	    	USART1-> TDR = c;
		}
	}
}

void debug_uart_callback(void)
{
	/*Check if RXNE (RX not empty)is raised and if RXNEIE is enabled*/
	if(((USART2->ISR & ISR_RXNE) !=0 ) && ((USART2->CR1 & CR1_RXNEIE) !=0))
		{
		  	  unsigned char c = USART2 ->RDR;
		  	  buff_store_char(c, _rx_buffer2);
		}

	/*Check if TXEIE is enabled*/
	if(((USART2->ISR & ISR_TXE) !=0 ) && ((USART2->CR1 & CR1_TXEIE) !=0))
	{
		if(tx_buffer2.head == tx_buffer2.tail)
		{
			  USART2->CR1 &=~ CR1_TXEIE;
		}
	    else
		{
			/*Get character from buffer*/
	    	unsigned char c = tx_buffer2.buffer[tx_buffer2.tail];
	    	/*update position*/
	    	tx_buffer2.tail = (tx_buffer2.tail + 1)%UART_BUFFER_SIZE;
	    	/*Transmit character*/
	    	USART2-> TDR = c;
		}
	}
}

void USART1_IRQHandler(void)
{
	bluetooth_uart_callback();
}

void USART2_IRQHandler(void)
{
	debug_uart_callback();
}
