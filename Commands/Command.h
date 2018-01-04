/*
 * Command.h
 *
 * Created: 12/21/2017 11:40:45 AM
 *  Author: oneso
 */ 


#ifndef COMMAND_H_
#define COMMAND_H_

#include <stdint.h>

class Command
{
public:

    //virtual destructor for abstract class
    virtual ~Command() {};

    //returns the ID of the Command
    inline uint8_t getID() { return CommandID; };

    //execute command(this should be kept as short as possible)
    // args needs to be defined in child classes
    virtual uint8_t execute(uint16_t& input) = 0;

protected:
    uint8_t CommandID;
};


#endif /* COMMAND_H_ */
