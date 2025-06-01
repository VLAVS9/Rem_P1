/*
 * ethernet/ethernet.h
 *      Authors:
 *      Gutiérrez Alvarez, Elián Ricardo
 *      Vargas Sánchez, Miguel Vladimir
 *      Muñoz Amézquita, André Emiliano
 */
// ethernet/ethernet.c
#include "ethernet/ethernet.h"
#include "ethernet/ethernet_cfg.h"
#include <stdlib.h>
#include <string.h>
#include "pin_mux.h"
#include "stdbool.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include "fsl_sysmpu.h"
#include <bits.h>

// Descriptores y buffers (alineados y no cacheables)
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rxBd[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t txBd[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t rxBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t txBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);

// Datos de la trama  04-7C-16-DF-73-D8
static const uint8_t destAddr[6] = {0x04,0x7C,0x16,0xDF,0x73,0xD8};
static const uint8_t srcAddr [6] = {0xD4,0xBE,0xD9,0x45,0x22,0x60};
static const char    nameStr[]  = "Vladimir Vargas Sanchez";
static uint8_t       frameBuf[ENET_FRAME_LEN + 14];

// Manejadores
static enet_handle_t gHandle;
static mdio_handle_t mdioHandle;
static phy_handle_t  phyHandle;
static uint32_t      sentCount;

// Inicializa hardware, PHY y ENET
void ethernet_init(void) {
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
// Agregamos 4 bytes de check parra el paquete
static uint32_t crc32_bitwise(const uint8_t *data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ 0xEDB88320;
            } else {
                crc >>= 1;
            }
        }
    }
    return ~crc;
}
// Construye trama de broadcast
void ethernet_buildFrame(uint8_t *string) {
    uint32_t payloadLen = ENET_FRAME_LEN - 14;
    size_t texto_len = strlen((char *)string);
    if (texto_len > payloadLen - 4) {
        texto_len = payloadLen - 4;  // Reservar 4 bytes para CRC32
    }

    // 1) Copiar MAC destino y origen
    memcpy(frameBuf + 0, destAddr, 6);
    memcpy(frameBuf + 6, srcAddr, 6);

    // 2) Escribir "Length" (payloadLen) en big-endian en bytes 12-13
    frameBuf[12] = (uint8_t)((payloadLen >> 8) & 0xFF);
    frameBuf[13] = (uint8_t)( payloadLen       & 0xFF);

    // 3) Copiar el texto plano en offset 14
    memcpy(frameBuf + 14, string, texto_len);

    // 4) Calcular CRC32 sobre SOLO los bytes del texto
    uint32_t crc = crc32_bitwise(frameBuf + 14, texto_len);

    // 5) Escribir CRC32 (little-endian) justo después del texto
    size_t crc_offset = 14 + texto_len;
    frameBuf[crc_offset + 0] = (uint8_t)( crc        & 0xFF);
    frameBuf[crc_offset + 1] = (uint8_t)((crc >> 8)  & 0xFF);
    frameBuf[crc_offset + 2] = (uint8_t)((crc >> 16) & 0xFF);
    frameBuf[crc_offset + 3] = (uint8_t)((crc >> 24) & 0xFF);

    // 6) Rellenar con 0xFF desde el final del CRC hasta completar payloadLen bytes
    size_t fill_start = crc_offset + 4;
    size_t fill_end   = 14 + payloadLen;  // equivale a ENET_FRAME_LEN
    for (size_t i = fill_start; i < fill_end; ++i) {
        frameBuf[i] = 0xFF;
    }
}
// Envía el paquete
void ethernet_send(void) {
    //if(sentCount >= ENET_TX_COUNT) return false;
    if(ENET_SendFrame(EXAMPLE_ENET, &gHandle, frameBuf, ENET_FRAME_LEN, 0, false, NULL) == kStatus_Success) {
        PRINTF("Frame transmitted\n");
    }

    //return false;
}

bool ethernet_receive(ethernet_frame_t *frame) {
    if (frame == NULL) return false;

    // Inicializar estructura
    frame->buffer = NULL;
    frame->length = 0;
    frame->payload_len = 0;

    // 1. Verificar si hay frames disponibles
    uint32_t length;
    status_t status = ENET_GetRxFrameSize(&gHandle, &length, 0);

    if (status != kStatus_Success || length == 0) {
        // PRINTF("No hay frames disponibles o error al obtener tamano (%d)\r\n", status); // Descomentar para depuración
        return false;
    }

    PRINTF("Se detectó un frame de tamaño: %u bytes.\r\n", length);

    // 2. Reservar memoria para el frame
    frame->buffer = (uint8_t *)malloc(length);
    if (frame->buffer == NULL) {
        PRINTF("Error: No se pudo asignar memoria para el frame\r\n");
        return false;
    }

    // 3. Leer el frame
    uint32_t timestamp;
    status = ENET_ReadFrame(EXAMPLE_ENET, &gHandle, frame->buffer, length, 0, &timestamp);

    if (status != kStatus_Success) {
        PRINTF("Error al leer el frame (%d).\r\n", status);
        free(frame->buffer);
        return false;
    }

    frame->length = length;

    // 4. Extraer longitud del payload (bytes 12-13)
    // Usando el formato Big Endian actual para depuración.
    // Recuerda que la práctica especifica Little Endian para "todos los datos que requieran mas de un bit"[cite: 2].
    frame->payload_len = (frame->buffer[12] << 8) | frame->buffer[13];

    PRINTF("Frame recibido - Longitud Total: %u, Longitud Payload (segun campo): %u\r\n", frame->length, frame->payload_len);

    // Imprimir el contenido completo del buffer recibido para depuración
    PRINTF("Contenido crudo del frame (primeros %d bytes):\r\n", frame->length > 64 ? 64 : frame->length);
    for(int i = 0; i < (frame->length > 64 ? 64 : frame->length); i++) {
        PRINTF("%02X ", frame->buffer[i]);
        if ((i + 1) % 16 == 0) { // Nueva línea cada 16 bytes
            PRINTF("\r\n");
        }
    }
    PRINTF("\r\n");

    return true;
}
