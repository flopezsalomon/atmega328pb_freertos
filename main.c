/*
* atmega328pb_so_freertos.c
*
* Created: 19-05-2024 17:23:56
* Author : Fernando Lopez Salomon
* Description : A simple example how to create two tasks with FREERTOS to handler GPIO and USART components.
*/

#include "FreeRTOS.h"
#include "task.h"
#include "application.h"
#include "hardware.h"
#include "gpio_driver.h"
#include "usart_driver.h"
#include <avr/io.h>

port_t status_led;

// Blinker Task
void blinkLED(void* parameter)
{
	while(1)
	{
		gpio_toogle(&status_led);   // Toggle the status LED
		vTaskDelay(500);			// Wait for 500 ms
	}
}

void printMsg(void* parameter)
{
	while(1)
	{
		usart_tx_frame(&usart[USART_0],"Hello World\n");
		vTaskDelay(1000);			// Wait for 1 sec
	}
}

int main(void)
{
	gpio_init_pin(&status_led,PORT_STATUS_LED, STATUS_LED, OUTPUT, OFFSET_PORT_LED); // Initialize status LED pin
	gpio_config_pin(&status_led);
	gpio_write_pin(&status_led, LOW);  // Set status LED to LOW initially
	
	usart_init(&usart[USART_0],USART0_BASE_REG);
	usart_config(&usart[USART_0],BAUDRATE,PARITY_MODE,MODE,DATA_BIT,STOP_BIT); // 9600, 8N1
	
	// Create Tasks
	xTaskCreate(blinkLED, "blink", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
	xTaskCreate(printMsg, "print", configMINIMAL_STACK_SIZE, NULL, 3, NULL);

	// Start scheduler
	vTaskStartScheduler();
	
	return 0;
}

// Idle task
void vApplicationIdleHook(void){
	// This runs while no other task runs
}

