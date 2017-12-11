/*
 * CANCommand.h
 *
 *  Created on: Nov 15, 2017
 *      Author: root
 */

#ifndef CAN_CANCOMMAND_H_
#define CAN_CANCOMMAND_H_

#include <stdint.h>

class CANCommand
{
public:
    CANCommand( uint8_t commandID );
    virtual ~CANCommand();

    //return command ID
    uint8_t getCommandID();

    //is command requested
    bool isRequested();

    //request command be executed
    void INT_Call_request(const uint32_t& data);

    //execute command
    //called in main loop
    void exec();
private:
    //command ID
    const uint8_t commandID;
    //command data
    volatile uint32_t data;

    //command is being executed
    //checked when CAN rx interrupt
    bool inProgress;
    //command execution has been requested
    volatile bool requested;
protected:
    //command
    virtual void command( uint32_t data ) {};

};


#endif /* CAN_CANCOMMAND_H_ */
