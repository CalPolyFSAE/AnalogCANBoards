/*
 * CONFIG_ControlInterface.h
 *
 *  Created on: Dec 10, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_COMMANDS_H_
#define CONFIG_CONFIG_COMMANDS_H_

#include <stdint.h>

#ifdef DEBUG
#define SERIAL_TEXT
#define COMMANDS
#endif //DEBUG

namespace CFG_CI
{
    //TODO: Finish Command CFG
    constexpr uint8_t RX_CANID = 0x05;
    constexpr uint8_t TX_CANID = 0x06;

}


#endif /* CONFIG_CONFIG_COMMANDS_H_ */
