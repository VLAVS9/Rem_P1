/*
 * terminal.h
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#ifndef TERMINAL_H_
#define TERMINAL_H_

#include <stdint.h>

void terminal_init(void);
void terminal_print_menu(void);
void terminal_print_mensaje(uint8_t index);
void terminal_print_respuesta(uint8_t index);

#endif /* TERMINAL_H_ */
