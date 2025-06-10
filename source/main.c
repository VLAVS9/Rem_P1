/*
 * main.h
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <stdio.h>
#include <gpio/gpio.h>
#include <nvic/nvic.h>
#include <pit/pit.h>
#include <uart/menu.h>
#include <uart/uart.h>

#include <ethernet/ethernet.h>
#include <fsl_debug_console.h>
#include <bits.h>

uint8_t active  = TRUE;
uint8_t messagesCounter  = bit_0;
uint8_t numberOfMessages = 10;



int main(void) {

	// Inicialización de periféricos
	ethernet_init();
	GPIO_init();
	LED_init();
	SW_init();
	PIT_enable();
	PIT0_init();
	UART0_init();

    // Inicialización de callbacks
	PIT_callback_init(PIT_0, menu_mostrar);

    // Configuración de prioridades de interrupción
    NVIC_set_basepri_threshold(PRIORITY_2);
    NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_1);
    NVIC_global_enable_interrupts;

    menu_mostrar();

    while(1){
    	uint32_t len;
    	        uint8_t *respuesta = ethernet_receive(&len);
    	        if (respuesta) {

    	            free(respuesta);
    	            break;
    	        }
    }

    return 0;
}
