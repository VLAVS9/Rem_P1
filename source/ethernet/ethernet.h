// ethernet/ethernet.h
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>
#include "fsl_enet.h"
#include "fsl_phy.h"

typedef struct {
    uint8_t *buffer;    // Buffer con los datos recibidos
    uint32_t length;    // Longitud del frame
    uint16_t payload_len; // Longitud del payload (extraído del campo Length)
} ethernet_frame_t;


void ethernet_init(void);
void ethernet_send(void);
void ethernet_buildFrame(uint8_t *string);
bool ethernet_receive(ethernet_frame_t *frame);  // Cambiado para usar la estructura


#endif // ETHERNET_H
