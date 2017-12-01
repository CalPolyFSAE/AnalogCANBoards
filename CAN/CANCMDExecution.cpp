/*
 * CANCMDExecution.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */

#include "CANCMDExecution.h"

CANCommand* CANCMDExecution::commands;
uint8_t CANCMDExecution::numCommands;

void CANCMDExecution::Init(CANCommand commands[], uint8_t numCommands)
{
    CANCMDExecution::commands = commands;
    CANCMDExecution::numCommands = numCommands;
}

void CANCMDExecution::INT_Call_Command(const uint32_t& data, uint8_t commandID)
{
    for(uint8_t i = 0; i < numCommands; ++i)
    {
        if(commands[i].getCommandID() == commandID && commands[i].isRequested() == false)
        {
            commands[i].INT_Call_request(data);
        }
    }
}


