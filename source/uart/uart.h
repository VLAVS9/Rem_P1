/*
 * uart.h
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

#define UART0_CLK_FREQ	CLOCK_GetFreq(UART0_CLK_SRC)
#define UART0_BAUDRATE	115200

typedef enum {
	UART_0
} uart_name_t;

typedef struct {
	uint8_t flag;		// Flag to indicate that there is new data.
	uint8_t mail_box;	// It contains the received data.
} uart_mail_box_t;

// Inicializa el callback para una UART específica
void UART_callback_init(uart_name_t uart, void (*handler)(uint8_t data));

void UART0_init(void);
uint8_t UART0_flag(void);
uint8_t UART0_mail_box(void);
void UART0_terminal(void);

#endif /* UART_H_ */
