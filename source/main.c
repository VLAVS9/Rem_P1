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
    }
    return 0;
}
