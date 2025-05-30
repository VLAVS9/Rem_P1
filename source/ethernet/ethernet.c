/*
 * ethernet/ethernet.h
 *
 *      Author: Vladimir Elian Andre
 */

#include "ethernet/ethernet.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_sysmpu.h"
#include "fsl_enet.h"
#include "fsl_enet_mdio.h"
#include "fsl_phyksz8081.h"
#include <string.h>
