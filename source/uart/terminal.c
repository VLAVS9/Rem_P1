/*
 * terminal.c
 *
 *  Created on: Oct 19, 2024
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */

#include <uart/terminal.h>
#include <stdio.h>
#include <string.h>
#include <fsl_uart.h>

/* VT100: Limpia pantalla */
uint8_t clear[] = "\033[2J";

/* VT100: Fondo negro, texto amarillo */
uint8_t background_text[] = "\033[40;33m";

/* Menú principal */
uint8_t menu_titulo[] = "--- Selecciona un mensaje a enviar ---\r\n";
uint8_t menu_instruccion[] = "Selecciona un número (1-16): ";

/* Inicializa terminal */
void terminal_init(void) {
    UART_WriteBlocking(UART0, clear, sizeof(clear));
    UART_WriteBlocking(UART0, background_text, sizeof(background_text));
}

/* ----- Mensajes ----- */
uint8_t mensaje_1[]   = "No todo lo que es oro reluce...";
uint8_t mensaje_2[]   = "Es peligroso...";
uint8_t mensaje_3[]   = "Aún en la oscuridad...";
uint8_t mensaje_4[]   = "Un mago nunca llega tarde...";
uint8_t mensaje_5[]   = "¿Qué es la vida?";
uint8_t mensaje_6[]   = "Aún hay esperanza...";
uint8_t mensaje_7[]   = "No temas a la oscuridad...";
uint8_t mensaje_8[]   = "El mundo esta cambiando...";
uint8_t mensaje_9[]   = "Hasta los más pequeños...";
uint8_t mensaje_10[]  = "Las raíces profundas...";
uint8_t mensaje_11[]  = "No digas que el sol se ha puesto...";
uint8_t mensaje_12[]  = "No se puede...";
uint8_t mensaje_13[]  = "El coraje se encuentra...";
uint8_t mensaje_14[]  = "Y sobre todo...";
uint8_t mensaje_15[]  = "No todos los tesoros...";
uint8_t mensaje_16[]  = "De las cenizas, un fuego...";

uint8_t *mensajes[16] = {
    mensaje_1, mensaje_2, mensaje_3, mensaje_4,
    mensaje_5, mensaje_6, mensaje_7, mensaje_8,
    mensaje_9, mensaje_10, mensaje_11, mensaje_12,
    mensaje_13, mensaje_14, mensaje_15, mensaje_16
};

uint16_t mensaje_lens[16] = {
    sizeof(mensaje_1),  sizeof(mensaje_2),  sizeof(mensaje_3),  sizeof(mensaje_4),
    sizeof(mensaje_5),  sizeof(mensaje_6),  sizeof(mensaje_7),  sizeof(mensaje_8),
    sizeof(mensaje_9),  sizeof(mensaje_10), sizeof(mensaje_11), sizeof(mensaje_12),
    sizeof(mensaje_13), sizeof(mensaje_14), sizeof(mensaje_15), sizeof(mensaje_16)
};

/* ----- Respuestas ----- */
uint8_t respuesta_1[]  = "...Ni todos los que vagan están perdidos.\r\n";
uint8_t respuesta_2[]  = "...brilla una luz.\r\n";
uint8_t respuesta_3[]  = "Nada más que un breve caminar a la luz del sol.\r\n";
uint8_t respuesta_4[]  = "...pues en ella se esconden las estrellas.\r\n";
uint8_t respuesta_5[]  = "...pueden cambiar el curso del futuro.\r\n";
uint8_t respuesta_6[]  = "...si aún te queda la luna.\r\n";
uint8_t respuesta_7[]  = "...en los lugares más inesperados.\r\n";
uint8_t respuesta_8[]  = "...son oro y plata.\r\n";
uint8_t respuesta_9[]  = "...cruzar tu puerta.\r\n";
uint8_t respuesta_10[] = "...ni pronto, Frodo Bolsón. Llega precisamente cuando se lo propone.\r\n";
uint8_t respuesta_11[] = "...mientras la Compañía permanezca fiel.\r\n";
uint8_t respuesta_12[] = "...Siento que algo se avecina.\r\n";
uint8_t respuesta_13[] = "...no alcanzan las heladas.\r\n";
uint8_t respuesta_14[] = "...pasar.\r\n";
uint8_t respuesta_15[] = "...cuidado con el Anillo.\r\n";
uint8_t respuesta_16[] = "...se despertará.\r\n";

uint8_t *respuestas[16] = {
    respuesta_1, respuesta_2, respuesta_3, respuesta_4,
    respuesta_5, respuesta_6, respuesta_7, respuesta_8,
    respuesta_9, respuesta_10, respuesta_11, respuesta_12,
    respuesta_13, respuesta_14, respuesta_15, respuesta_16
};

uint16_t respuesta_lens[16] = {
    sizeof(respuesta_1),  sizeof(respuesta_2),  sizeof(respuesta_3),  sizeof(respuesta_4),
    sizeof(respuesta_5),  sizeof(respuesta_6),  sizeof(respuesta_7),  sizeof(respuesta_8),
    sizeof(respuesta_9),  sizeof(respuesta_10), sizeof(respuesta_11), sizeof(respuesta_12),
    sizeof(respuesta_13), sizeof(respuesta_14), sizeof(respuesta_15), sizeof(respuesta_16)
};

/* Posiciona el cursor en x,y en consola VT100 */
void set_vt100_position(uint8_t *buffer, uint8_t x, uint8_t y) {
    buffer[0] = '\033';
    buffer[1] = '[';
    buffer[2] = '0' +  y / 100;
    buffer[3] = '0' + (y % 100) / 10;
    buffer[4] = '0' +  y % 10;
    buffer[5] = ';';
    buffer[6] = '0' +  x / 100;
    buffer[7] = '0' + (x % 100) / 10;
    buffer[8] = '0' +  x % 10;
    buffer[9] = 'H';
    buffer[10] = '\0';
}

/* Imprime el menú principal en consola con numeración */
void terminal_print_menu(void) {
    uint8_t g_vt100_xy[20];
    uint8_t x = 10;
    uint8_t y = 10;

    set_vt100_position(g_vt100_xy, x, y);
    UART_WriteBlocking(UART0, g_vt100_xy, sizeof(g_vt100_xy));
    UART_WriteBlocking(UART0, menu_titulo, sizeof(menu_titulo));
    y++;

    for (uint8_t i = 0; i < 16; i++) {
        set_vt100_position(g_vt100_xy, x, y++);
        UART_WriteBlocking(UART0, g_vt100_xy, sizeof(g_vt100_xy));

        char enumerado[6];
        snprintf(enumerado, sizeof(enumerado), "%2u) ", i + 1);
        UART_WriteBlocking(UART0, (uint8_t *)enumerado, strlen(enumerado));

        UART_WriteBlocking(UART0, mensajes[i], mensaje_lens[i]);
    }

    set_vt100_position(g_vt100_xy, x, y);
    UART_WriteBlocking(UART0, g_vt100_xy, sizeof(g_vt100_xy));
    UART_WriteBlocking(UART0, menu_instruccion, sizeof(menu_instruccion));
}

/* Imprime un mensaje individual */
void terminal_print_mensaje(uint8_t index) {
    uint8_t g_vt100_xy[20];
    uint8_t x = 10;
    uint8_t y = 30;

    if (index < 16) {
        set_vt100_position(g_vt100_xy, x, y);
        UART_WriteBlocking(UART0, g_vt100_xy, sizeof(g_vt100_xy));

        char enumerado[6];
        snprintf(enumerado, sizeof(enumerado), "%2u) ", index + 1);
        UART_WriteBlocking(UART0, (uint8_t *)enumerado, strlen(enumerado));

        UART_WriteBlocking(UART0, mensajes[index], mensaje_lens[index]);

        ethernet_buildFrame(mensajes[index]);
        ethernet_send();
    }


}

/* Imprime una respuesta individual */
void terminal_print_respuesta(uint8_t *string) {
    uint8_t g_vt100_xy[20];
    uint8_t x = 10;
    uint8_t y = 32;


        set_vt100_position(g_vt100_xy, x, y);
        UART_WriteBlocking(UART0, g_vt100_xy, sizeof(g_vt100_xy));
        UART_WriteBlocking(UART0, string, strlen(string));

}

uint8_t* terminal_get_mensaje(uint8_t index) {
    if (index >= 1 && index <= 16) {
        return mensajes[index - 1];
    }
    return NULL;
}

uint8_t* terminal_get_respuesta(uint8_t index) {
    if (index >= 1 && index <= 16) {
        return respuestas[index - 1];
    }
    return NULL;
}
