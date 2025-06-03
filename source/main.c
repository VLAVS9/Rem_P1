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
#include <uart/menu.h>
#include <uart/uart.h>

#include <ethernet/ethernet.h>
#include <fsl_debug_console.h>
#include <bits.h>

uint8_t active  = TRUE;
uint8_t messagesCounter  = bit_0;
uint8_t numberOfMessages = 10;

/*uint8_t frase1[]   = "No todo lo que es oro reluce...";
uint8_t frase2[]   = "Es peligroso...";
uint8_t frase3[]   = "Aún en la oscuridad...";
uint8_t frase4[]   = "Un mago nunca llega tarde...";
uint8_t frase5[]   = "¿Qué es la vida?";
uint8_t frase6[]   = "Aún hay esperanza...";
uint8_t frase7[]   = "No temas a la oscuridad...";
uint8_t frase8[]   = "El mundo esta cambiando...";
uint8_t frase9[]   = "Hasta los más pequeños...";
uint8_t frase10[]  = "Las raíces profundas...";
uint8_t frase11[]  = "No digas que el sol se ha puesto...";
uint8_t frase12[]  = "No se puede...";
uint8_t frase13[]  = "El coraje se encuentra...";
uint8_t frase14[]  = "Y sobre todo...";
uint8_t frase15[]  = "No todos los tesoros...";
uint8_t frase16[]  = "De las cenizas, un fuego...";*/

int main(void) {

	// Inicialización de periféricos
	GPIO_init();
	LED_init();
	SW_init();
	UART0_init();

	//ethernet_init();
	//ethernet_buildFrame(frase16);

    // Configuración de prioridades de interrupción
    NVIC_set_basepri_threshold(PRIORITY_2);
    NVIC_enable_interrupt_and_priotity(UART0_IRQ, PRIORITY_1);
    NVIC_global_enable_interrupts;

    while(TRUE == active) {

    	menu_mostrar();  // Control central del menú interactivo

		/*if(messagesCounter >= numberOfMessages){
			PRINTF("Done sending %d frames\r\n", numberOfMessages);
			active = FALSE;
		}else {
			ethernet_send();
			messagesCounter ++;
		}

		ethernet_frame_t received_frame = {0};

		if (ethernet_receive(&received_frame)) {

			if (received_frame.length >= 14) {
				uint8_t *payload_start = received_frame.buffer + 14;
				size_t actual_payload_len = received_frame.payload_len;
				size_t print_len = (actual_payload_len < (received_frame.length - 14)) ? actual_payload_len : (received_frame.length - 14);

				PRINTF("Mensaje: \"");
				for (size_t i = 0; i < print_len; i++) {
					if (payload_start[i] >= 32 && payload_start[i] <= 126) { // Caracteres ASCII imprimibles
						PRINTF("%c", payload_start[i]);
					} else {
						PRINTF("."); // Imprimir un punto para caracteres no imprimibles
					}
				}
				PRINTF("\"\r\n");
			}
			free(received_frame.buffer); //liberar la memoria
		}*/
    }
    return 0;
}
