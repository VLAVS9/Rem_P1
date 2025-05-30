/*
 * main.h
 *
 *      Author: Vladimir Elian Andre
 */

#include "ethernet/ethernet.h"
#include "fsl_debug_console.h"

uint8_t active  = 1;
uint8_t counter  = 0;
uint8_t frase1[]  = "No todo lo que es oro reluce";

int main(void) {
    ENET_InitDriver();
    //ENET_BuildFrame();
    ENET_BuildFrame_frase(frase1);

    while(active) {
		if(counter >= 5){
			PRINTF("Done sending %d frames\r\n", 5);
			active = 0;
		}else {
			ENET_SendNext();
			counter++;
		}
    }
    return 0;
}
