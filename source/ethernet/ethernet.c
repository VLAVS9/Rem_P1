/*
 * ethernet/ethernet.h
 *
 *      Author: Vladimir Elian Andre
 */
// ethernet/ethernet.c
#include "ethernet/ethernet.h"
#include <stdlib.h>
#include <string.h>
#include "pin_mux.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_sysmpu.h"

// Descriptores y buffers (alineados y no cacheables)
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rxBd[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t txBd[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t rxBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t txBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);

// Datos de la trama
static const uint8_t destAddr[6] = {0x36,0xCD,0x5C,0x2B,0x70,0x40};
static const uint8_t srcAddr [6] = {0xD4,0xBE,0xD9,0x45,0x22,0x60};
static const char    nameStr[]  = "Vladimir Vargas Sanchez";
static uint8_t       frameBuf[ENET_FRAME_LEN + 14];

// Manejadores
static enet_handle_t gHandle;
static mdio_handle_t mdioHandle;
static phy_handle_t  phyHandle;
static uint32_t      sentCount;

// Inicializa hardware, PHY y ENET
void ENET_InitDriver(void) {
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    PRINTF("\r\nENET example start.\r\n");

    SYSMPU_Enable(SYSMPU, false);
    CLOCK_SetRmii0Clock(1U);

    // Config ENET
    enet_config_t config;
    ENET_GetDefaultConfig(&config);
#ifndef EXAMPLE_PHY_INTERFACE_RGMII
    config.miiMode = kENET_RmiiMode;
#else
    config.miiMode = kENET_RgmiiMode;
#endif

    // Config MDIO + PHY
    phy_config_t phyCfg = { .phyAddr = EXAMPLE_PHY_ADDRESS, .autoNeg = true };
    mdioHandle.ops               = &EXAMPLE_MDIO_OPS;
    mdioHandle.resource.base     = EXAMPLE_ENET;
    mdioHandle.resource.csrClock_Hz = EXAMPLE_CLOCK_FREQ;
    phyHandle.phyAddr            = EXAMPLE_PHY_ADDRESS;
    phyHandle.mdioHandle         = &mdioHandle;
    phyHandle.ops                = &EXAMPLE_PHY_OPS;

    // Auto-negociación
    PHY_Init(&phyHandle, &phyCfg);
    bool link = false, autoN = false;
    for(uint32_t i=PHY_AUTONEGO_TIMEOUT_COUNT; i; --i) {
        PHY_GetAutoNegotiationStatus(&phyHandle, &autoN);
        PHY_GetLinkStatus(&phyHandle, &link);
        if(autoN && link) break;
    }
    PRINTF(link ? "PHY link up\r\n" : "PHY link failed\r\n");

    // Ajuste velocidad/duplex
    phy_speed_t speed;
    phy_duplex_t dup;
    PHY_GetLinkSpeedDuplex(&phyHandle, &speed, &dup);
    config.miiSpeed  = (enet_mii_speed_t)speed;
    config.miiDuplex = (enet_mii_duplex_t)dup;

    // Buffers
    enet_buffer_config_t bufCfg[] = {{
        ENET_RXBD_NUM, ENET_TXBD_NUM,
        SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT),
        SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT),
        rxBd, txBd, &rxBuff[0][0], &txBuff[0][0],
        true, true, NULL
    }};

    // Init ENET
    ENET_Init(EXAMPLE_ENET, &gHandle, &config, bufCfg, (uint8_t*)srcAddr, EXAMPLE_CLOCK_FREQ);
    ENET_ActiveRead(EXAMPLE_ENET);
    sentCount = 0;
}

// Construye trama de broadcast
void ENET_BuildFrame(void) {
    uint32_t payloadLen = ENET_FRAME_LEN - 14;
    memcpy(frameBuf, destAddr, 6);
    memcpy(frameBuf+6, srcAddr, 6);
    frameBuf[12] = (payloadLen >> 8) & 0xFF;
    frameBuf[13] = payloadLen & 0xFF;
    memcpy(frameBuf+14, nameStr, strlen(nameStr));
    for(uint32_t i=14+strlen(nameStr); i<ENET_FRAME_LEN+14; ++i) {
        frameBuf[i] = 0xFF;
    }
}

void ENET_BuildFrame_frase(uint8_t *string) {
    uint32_t payloadLen = ENET_FRAME_LEN - 14;
    memcpy(frameBuf, destAddr, 6);
    memcpy(frameBuf+6, srcAddr, 6);
    frameBuf[12] = (payloadLen >> 8) & 0xFF;
    frameBuf[13] = payloadLen & 0xFF;
    memcpy(frameBuf+14, string, strlen(string));
    for(uint32_t i=14+strlen(string); i<ENET_FRAME_LEN+14; ++i) {
        frameBuf[i] = 0xFF;
    }
}

// Envía el siguiente paquete
bool ENET_SendNext(void) {
    //if(sentCount >= ENET_TX_COUNT) return false;
    if(ENET_SendFrame(EXAMPLE_ENET, &gHandle, frameBuf, ENET_FRAME_LEN, 0, false, NULL) == kStatus_Success) {
        PRINTF("Frame transmitted\n");
    }

    //return false;
}
