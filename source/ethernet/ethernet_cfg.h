/*
 * ethernet_cfg.h
 *
 *  Created on: 31 may. 2025
 *      Author: vonta
 */

#ifndef ETHERNET_ETHERNET_CFG_H_
#define ETHERNET_ETHERNET_CFG_H_

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

#define AES_BLOCK_SIZE     16
#define ENET_HEADER_LEN    14

#define ENET_RXBD_NUM           4
#define ENET_TXBD_NUM           4
#define ENET_FRAME_LEN          64
#define ENET_TX_COUNT           15

#endif /* ETHERNET_ETHERNET_CFG_H_ */
