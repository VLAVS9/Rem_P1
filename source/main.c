/*
 * main.h
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <ethernet/ethernet.h>
#include <fsl_debug_console.h>
#include <bits.h>

uint8_t active  = TRUE;
uint8_t messagesCounter  = bit_0;
uint8_t numberOfMessages = 10;

uint8_t frase1[]  = "No todo lo que es oro reluce...";
uint8_t frase2[]  = "No temas a la oscuridad...";

int main(void) {
	ethernet_init();
	ethernet_buildFrame(frase1);

    while(TRUE == active) {
		if(messagesCounter >= numberOfMessages){
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

		                // Imprimimos los caracteres ASCII directamente.
		                // Aseguramos que no excedemos el tamaño real del buffer para evitar lecturas fuera de límites.
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
		        }
		        // Opcional: Pequeña espera para no consumir CPU en exceso
		        // for(volatile int i=0; i<10000; i++); // Descomentar si el loop es demasiado rápido

    }
    return 0;
}
