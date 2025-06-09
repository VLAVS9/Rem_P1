/*
 * menu.c
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <uart/menu.h>
#include <bits.h>
#include <uart/terminal.h>
#include <uart/uart.h>
#include <fsl_uart.h>

/* Estados globales */
uint8_t selection_state   = TRUE;
uint8_t menu_state        = bit_0;
uint8_t interaction_state = bit_0;
uint8_t return_state      = FALSE;

/* Variables auxiliares */
uint8_t tecla   = bit_0;
uint8_t numero  = bit_0;

uint8_t msg_num   = bit_0;
uint8_t *msg_text = NULL;

/*
 * Muestra el menú solo si selection_state está activo.
 * Esta función es llamada continuamente desde el while(1) en main,
 * pero solo ejecuta contenido si hay cambio de estado.
 */
void menu_mostrar(void) {
    if (TRUE == selection_state) {

        terminal_init();

        switch (menu_state) {
            case bit_0:
                terminal_print_menu();
                interaction_state = bit_0;
                break;

            case bit_1:
            	terminal_print_mensaje(msg_num);
            	terminal_print_menu();


            	menu_state        = bit_0;
                interaction_state = bit_0;
                break;

            default:
                break;
        }

        selection_state = FALSE;
    }

    menu_interaction();

}

/*
 * Lógica de interacción con el menú.
 * Lee caracteres de UART, interpreta ESC, RETURN y números válidos (3-10),
 * y controla el flujo del menú basado en estos eventos.
 */
void menu_interaction(void) {
    if ((bit_0 == interaction_state) && (TRUE == UART0_flag())) {

        tecla = UART0_mail_box();  // Captura la tecla presionada.

        if (ESC == tecla) {
            // Volver al menú principal.
            selection_state   = TRUE;
            menu_state        = bit_0;
            interaction_state = bit_0;
            return_state      = FALSE;
        }
        else if ((('0' <= tecla) && ('9' >= tecla)) && (bit_0 == numero) && (FALSE == return_state)) {
            // Primer dígito capturado.
            numero        = tecla - '0';
            return_state  = TRUE;
        }
        else if ((('0' <= tecla) && ('9' >= tecla)) && (bit_0 != numero) && (TRUE == return_state)) {
            // Segundo dígito capturado.
            numero        = numero * 10 + (tecla - '0');
            return_state  = TRUE;
        }
        else if ((RETURN == tecla) && (TRUE == return_state)) {
            if ((1 <= numero) && (16 >= numero)) {
                // Número válido, continuar con siguiente menú.
            	msg_num  = numero - 1;
            	msg_text = terminal_get_mensaje(msg_num);

                selection_state   = TRUE;
                menu_state        = bit_1;
                interaction_state = bit_1;
                return_state      = FALSE;
            } else {
                // Número fuera de rango, volver al menú principal.
                selection_state   = TRUE;
                menu_state        = bit_0;
                interaction_state = bit_0;
                return_state      = FALSE;
            }

            numero = bit_0; // Reinicia número tras procesarlo.
        }
        else {
            // Cualquier otra tecla no válida, reiniciar menú.
            selection_state   = TRUE;
            menu_state        = bit_0;
            interaction_state = bit_0;
            return_state      = FALSE;
        }

        UART0_terminal(); // Limpia UART.
    }
}
