/*
 * CANCMDExecution.h
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */

#ifndef CAN_CANCMDEXECUTION_H_
#define CAN_CANCMDEXECUTION_H_

#include <stdint.h>

#include "CANCommand.h"

class CANCMDExecution
{
public:
    //startup
    //must be called before interrupts are enabled
    static void Init(CANCommand commands[], uint8_t numCommands);

    //receive command
    static void INT_Call_Command(const uint32_t& data, uint8_t commandID);
private:
    //list of commands
    static CANCommand* commands;
    static uint8_t numCommands;
};


#endif /* CAN_CANCMDEXECUTION_H_ */
