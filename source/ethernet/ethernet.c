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

#ifndef AES_BLOCKLEN
#define AES_BLOCKLEN 16
#endif

// Descriptores y buffers (alineados y no cacheables)
AT_NONCACHEABLE_SECTION_ALIGN(enet_rx_bd_struct_t rxBd[ENET_RXBD_NUM], ENET_BUFF_ALIGNMENT);
AT_NONCACHEABLE_SECTION_ALIGN(enet_tx_bd_struct_t txBd[ENET_TXBD_NUM], ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t rxBuff[ENET_RXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);
SDK_ALIGN(uint8_t txBuff[ENET_TXBD_NUM][SDK_SIZEALIGN(ENET_FRAME_MAX_FRAMELEN, APP_ENET_BUFF_ALIGNMENT)], APP_ENET_BUFF_ALIGNMENT);

// Datos de la trama  04-7C-16-DF-73-D8
//macbook en 11 00:90:9a:9d:a3:4d
static const uint8_t destAddr[6] = {0x00,0x90,0x9a,0x9d,0xa3,0x4d};
static const uint8_t srcAddr [6] = {0xD4,0xBE,0xD9,0x45,0x22,0x60};
static const char    nameStr[]  = "Vladimir Vargas Sanchez";
static uint8_t       frameBuf[ENET_FRAME_LEN + 14];

// claves aes
static const uint8_t aes_key[16] = {
    'M','y','1','6','b','y','t','e','K','e','y','0','0','0','0','0'
};
static const uint8_t aes_iv[16] = {
    'M','y','1','6','b','y','t','e','I','V','0','0','0','0','0','0'
};
// Manejadores
static enet_handle_t gHandle;
static mdio_handle_t mdioHandle;
static phy_handle_t  phyHandle;
static uint32_t      sentCount;

// Inicializa hardware enet
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
// Construye trama de broadcast
void ethernet_buildFrame(uint8_t *string)
{
    // 1) Determinar longitud del texto plano
    size_t data_len = strlen((char *)string);

    // 2) Calcular longitud del ciphertext (múltiplo de 16)
    size_t padded_len = ((data_len / AES_BLOCKLEN) + 1) * AES_BLOCKLEN;

    // 3) Reservar buffer temporal para ciphertext
    uint8_t *cipher = (uint8_t *)malloc(padded_len);
    if (!cipher) {
        return;  // sin memoria, abortar
    }

    // 4) Cifrar con AES-CBC+PKCS#7
    aes_encrypt(string, data_len, aes_key, aes_iv, cipher, &padded_len);

    // 5) Construir encabezado Ethernet en frameBuf[0..13]
    memcpy(frameBuf + 0, destAddr, 6);
    memcpy(frameBuf + 6, srcAddr, 6);
    frameBuf[12] = (uint8_t)((ENET_FRAME_LEN >> 8) & 0xFF);
    frameBuf[13] = (uint8_t)( ENET_FRAME_LEN       & 0xFF);

    // 6) Copiar ciphertext en offset 14
    memcpy(frameBuf + 14, cipher, padded_len);

    // 7) Rellenar con 0xFF hasta completar ENET_FRAME_LEN bytes de payload
    for (size_t i = 14 + padded_len; i < 14 + ENET_FRAME_LEN; i++) {
        frameBuf[i] = 0xFF;
    }

    free(cipher);
}

// Envía el paquete
void ethernet_send(void) {

    if(ENET_SendFrame(EXAMPLE_ENET, &gHandle, frameBuf, ENET_FRAME_LEN, 0, false, NULL) == kStatus_Success) {
        PRINTF("Frame transmitted\n");
    }
}
//recibir paquete
uint8_t ethernet_receive(ethernet_frame_t *frame) {
    if (frame == NULL) {
        return false;
    }

    // 1) Inicializar estructura
    frame->buffer      = NULL;
    frame->length      = 0;
    frame->payload_len = 0;

    // 2) Leer tamaño total de la trama
    uint32_t total_len;
    if (ENET_GetRxFrameSize(&gHandle, &total_len, 0) != kStatus_Success || total_len == 0) {
        return false;
    }

    // 3) Reservar buffer para toda la trama
    frame->buffer = malloc(total_len);
    if (!frame->buffer) {
        return false;
    }

    // 4) Agarra la trama completa
    uint32_t timestamp;
    if (ENET_ReadFrame(EXAMPLE_ENET, &gHandle,
                       frame->buffer, total_len, 0, &timestamp) != kStatus_Success) {
        free(frame->buffer);
        return false;
    }

    // 5) filtrar MAC destino en srcAddr para solo acpetar paquetes, si no return
    for (uint32_t i = 0; i < 6; i++) {
        if (frame->buffer[i] != srcAddr[i]) {
            free(frame->buffer);
            return false;
        }
    }

    // 6) Recortar los bytes del tamaño
    uint16_t payload_len = (frame->buffer[12] << 8) | frame->buffer[13];
    uint32_t offset      = 14;
    if (offset + payload_len > total_len) {
        free(frame->buffer);
        return false;
    }

    // 7) Copiar solo el ciphertext + padding 0xFF
    uint8_t *blob = malloc(payload_len);
    memcpy(blob, frame->buffer + offset, payload_len);
    free(frame->buffer);

    // 8) Recortar  0xFF
    int32_t j = payload_len - 1;
    while (j >= 0 && blob[j] == 0xFF) {
        j--;
    }
    if (j < 0) {
        free(blob);
        return false;
    }
    uint32_t cipher_len = j + 1;


    // 9) Desencriptado
    uint8_t *decrypted = malloc(cipher_len + 1);
    size_t  dec_len    = 0;
    aes_decrypt(blob, cipher_len, aes_key, aes_iv, decrypted, &dec_len);
    free(blob);


    // 10) Mostrar como texto, respetando la longitud real
    PRINTF("Mensaje desencriptado: %.*s\r\n", (int)dec_len, decrypted);

    terminal_print_respuesta(decrypted);
    // 11)
    frame->buffer      = decrypted;
    frame->length      = dec_len;
    frame->payload_len = dec_len;

    return true;
}
