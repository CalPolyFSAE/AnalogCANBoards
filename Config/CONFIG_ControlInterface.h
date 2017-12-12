/*
 * CONFIG_ControlInterface.h
 *
 *  Created on: Dec 10, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_CONTROLINTERFACE_H_
#define CONFIG_CONFIG_CONTROLINTERFACE_H_

#include <stdint.h>
#include "../CAN/CANRXTX.h"

namespace CFG_CI
{
    //TODO: this still needs to be setup properly
    constexpr CANRXTX::MOB_SETTINGS CMDCANRX
    {
        CANRXTX::MOB_MODE::RX,  //mode
        0x01,                   //CANID
        0,                      //dlc determined at runtime
        0,                      //no rtr flag
        0,                      //no identifier extension

        0,                      //no filtering for CAN RX
        0,                      //
        0,                      //
        0                       //
    };

    constexpr CANRXTX::MOB_SETTINGS CMDCANTX
    {
        CANRXTX::MOB_MODE::TX,  //mode
        0x01,                   //CANID
        0,                      //dlc determined at runtime
        0,                      //no rtr flag
        0,                      //no identifier extension

        0,                      //no filtering for CAN TX
        0,                      //
        0,                      //
        0                       //
    };
    //number of reserved MOBs (for commands over CAN)
    constexpr uint8_t NUMCANRESERVEDMOBS = 2;//one send one receive
}


#endif /* CONFIG_CONFIG_CONTROLINTERFACE_H_ */
