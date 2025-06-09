/*
 * ethernet_cfg.h
 *
 *  Created on: 3 jun. 2025
 *      Author: miguelvladimirvargassanchez
 */

#ifndef ETHERNET_ETHERNET_CFG_H
#define ETHERNET_ETHERNET_CFG_H

#include <stdint.h>

/* Ajusta ENET_FRAME_LEN al tamaño de payload deseado (ej: 64 bytes) */
#define ENET_FRAME_LEN  64

/* Cantidad de descriptores, buffers, etc. (igual que en tu proyecto original) */
#define ENET_RXBD_NUM       4
#define ENET_TXBD_NUM       4
#define APP_ENET_BUFF_ALIGNMENT   16
#define ENET_FRAME_MAX_FRAMELEN   1536

/* Phy address y clock (igual que en tu proyecto original) */
#define EXAMPLE_PHY_ADDRESS       0x02
#define EXAMPLE_CLOCK_FREQ        CLOCK_GetFreq(kCLOCK_CoreSysClk)

#endif // ETHERNET_ETHERNET_CFG_H
