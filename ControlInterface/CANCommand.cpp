/*
 * CANCommand.cpp
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */

#include "CANCommand.h"

#include <stdint.h>

CANCommand::CANCommand(uint8_t commandID) :
    commandID(commandID)
{
    inProgress = false;
    requested = false;
}

uint8_t CANCommand::getCommandID()
{
    return commandID;
}

bool CANCommand::isRequested()
{
    return requested;
}

//TODO: Atomic? for 32bit data
void CANCommand::INT_Call_request(const uint32_t& data)
{
    //double check the request status
    if(!requested)
    {
        requested = true;
        this->data = data;
    }
}

void CANCommand::exec()
{
    //double check the request status
    if(requested)
    {
        inProgress = true;
        //execute command
        command(data);
        inProgress = false;
        requested = false;
    }
}


