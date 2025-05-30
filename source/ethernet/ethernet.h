// ethernet/ethernet.h
#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>
#include "fsl_enet.h"
#include "fsl_phy.h"

// Configuración ENET/PHY
#define EXAMPLE_ENET            ENET
#define EXAMPLE_PHY_ADDRESS     0x00U
#define EXAMPLE_MDIO_OPS        enet_ops
#define EXAMPLE_PHY_OPS         phyksz8081_ops
#define EXAMPLE_CLOCK_FREQ      CLOCK_GetFreq(kCLOCK_CoreSysClk)

#ifndef APP_ENET_BUFF_ALIGNMENT
#define APP_ENET_BUFF_ALIGNMENT ENET_BUFF_ALIGNMENT
#endif
#ifndef PHY_AUTONEGO_TIMEOUT_COUNT
#define PHY_AUTONEGO_TIMEOUT_COUNT 100000U
#endif
#ifndef PHY_STABILITY_DELAY_US
#define PHY_STABILITY_DELAY_US    0U
#endif

#define ENET_RXBD_NUM           4
#define ENET_TXBD_NUM           4
#define ENET_FRAME_LEN          64
#define ENET_TX_COUNT           15

// Prototipos
void ENET_InitDriver(void);
void ENET_BuildFrame(void);
bool ENET_SendNext(void);
void ENET_BuildFrame_frase(uint8_t *string);

#endif // ETHERNET_H
