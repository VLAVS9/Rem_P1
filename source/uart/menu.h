/*
 * menu.h
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#ifndef MENU_H_
#define MENU_H_

#include <stdint.h>

#define ESC     0x1B
#define RETURN  0x0D

extern uint8_t* msg_text;

void menu_mostrar(void);
void menu_interaction(void);

#endif /* MENU_H_ */
