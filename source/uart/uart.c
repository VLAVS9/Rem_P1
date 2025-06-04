/*
 * uart.c
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <uart/uart.h>
#include <bits.h>
#include <gpio/gpio.h>
#include <fsl_uart.h>

uart_mail_box_t g_mail_box_uart_0;

static uart_config_t config;

// Callback general, estructurado como en GPIO
static void (*uart0_callback)(uint8_t data) = NULL;

void UART_callback_init(uart_name_t uart, void (*handler)(uint8_t data)) {

	if (UART_0 == uart) {
		uart0_callback = handler;
	}
}

void UART0_RX_TX_IRQHandler(void) {

	if ((kUART_RxDataRegFullFlag | kUART_RxOverrunFlag) & UART_GetStatusFlags(UART0)) {

		uint8_t data = UART_ReadByte(UART0);

		g_mail_box_uart_0.mail_box = data;
		g_mail_box_uart_0.flag     = true;

		if (uart0_callback) {
			uart0_callback(data);
		}
	}

	SDK_ISR_EXIT_BARRIER;
}

void UART0_init(void) {

	uint32_t uart0_clock;

	CLOCK_EnableClock(kCLOCK_PortB);

	UART0_port_init();

	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = UART0_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	uart0_clock = CLOCK_GetFreq(UART0_CLK_SRC);

	UART_Init(UART0, &config, uart0_clock);

	UART_EnableInterrupts(UART0, kUART_RxDataRegFullInterruptEnable | kUART_RxOverrunInterruptEnable);
}

uint8_t UART0_flag(void) {
	return g_mail_box_uart_0.flag;
}

uint8_t UART0_mail_box(void) {
	return g_mail_box_uart_0.mail_box;
}

void UART0_terminal(void) {
	UART_WriteBlocking(UART0, &g_mail_box_uart_0.mail_box, bit_1);
	g_mail_box_uart_0.flag = false;
}
