/**
* @file usart_driver.c
* @brief Source file for USART driver on ATmega328PB.
*
* This file implements the functions declared in usart_driver.h for handling USART
* communication. This includes initializations, configurations, and data handling
* through the USART peripheral.
*
* @date 27-08-2022
* @author Fernando Lopez Salomon
*/

#include "usart_driver.h"

void usart_init(USART_t *u, uint8_t r)
{
	for(uint8_t i=0; i< NUMBER_OF_REGISTERS; i++){
		u->usart_registers[i] = (volatile uint8_t*)(r + i);
	}
}

void usart_config(USART_t *u, uint16_t baudrate, uint8_t parity, uint8_t usart_mode, uint8_t data_bit, uint8_t stop_bit)
{
	*(u->usart_registers[5]) = baudrate >> 8; // Set Baud Rate
	*(u->usart_registers[4]) = baudrate;
	
	*(u->usart_registers[2]) = usart_mode | parity | stop_bit | data_bit;		// Set Frame Format
	*(u->usart_registers[1]) = (1<<RXEN1) | (1<<TXEN1); 						// Enable Receiver and Transmitter
	*(u->usart_registers[1]) |= (1 << RXCIE1);									// Enable the USART Receive Complete interrupt (USART_RXC)
	
	rb_init(&u->rb,SERIAL_BUFFER_SIZE,u->serial_buffer);
}

void usart_tx(USART_t *u, uint8_t data)
{
	while (!(*(u->usart_registers[0]) & (1<<UDRE0)));	 // Wait for empty transmit buffer
	*(u->usart_registers[6]) = data;					 // Put data into buffer, sends the data
}

void usart_rx(void)
{
	
}

uint16_t usart_available(USART_t *u)
{
	return rb_full_count(&u->rb);
}

uint8_t usart_read(USART_t *u)
{
	return rb_remove(&u->rb);
}

void usart_flush(void)
{
	
}

void usart_tx_frame(USART_t *u, uint8_t * frame)
{
	for (uint8_t j = 0; j < strlen(frame); j++){
		usart_tx(u,*(frame + j));
	}
}

void usart_receive(USART_t *u)
{
	rb_push_insert(&u->rb, (uint8_t)*(u->usart_registers[6]));
}

ISR(USART0_RX_vect)
{
	usart_receive(&usart[USART_0]);
}

ISR(USART1_RX_vect)
{
	usart_receive(&usart[USART_1]);
}

