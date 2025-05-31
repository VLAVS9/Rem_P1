// ethernet/ethernet.h
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>
#include "fsl_enet.h"
#include "fsl_phy.h"

void ethernet_init(void);
void ethernet_send(void);
void ethernet_buildFrame(uint8_t *string);

#endif // ETHERNET_H
