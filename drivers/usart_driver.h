/*
* usart_driver.h
*
* Created: 27-08-2022 18:24:42
*  Author: fernando
*/


#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#define NUMBER_OF_REGISTERS (8)
#define USART_0 1
#define USART_1 0
#define SERIAL_BUFFER_SIZE 100

#include <avr/io.h>
#include <avr/interrupt.h> // Contains all interrupt vectors
#include <string.h>
#include "ring_buffer.h"

typedef struct
{
	volatile uint8_t *usart_registers[NUMBER_OF_REGISTERS];
	ring_buffer rb;
	uint8_t serial_buffer[SERIAL_BUFFER_SIZE];
}USART_t;

USART_t usart[2];

void usart_init(USART_t *u, uint8_t r);
void usart_config(USART_t *u, uint16_t baudrate, uint8_t parity , uint8_t usart_mode, uint8_t data_bit , uint8_t stop_bit);
void usart_tx(USART_t *u , uint8_t data);
void usart_rx(void);
void usart_tx_frame(USART_t *u, uint8_t * frame);

uint16_t usart_available(USART_t *u);
uint8_t usart_read(USART_t *u);
void usart_flush(void);
void usart_receive(USART_t *u);



#endif /* USART_DRIVER_H_ */