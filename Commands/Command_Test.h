/*
 * Command_Test.h
 *
 * Created: 12/21/2017 4:09:15 PM
 *  Author: oneso
 */ 


#ifndef COMMAND_TEST_H_
#define COMMAND_TEST_H_

#include "Command.h"
#include <stdint.h>

class Command_Test : public Command
{
public:

    Command_Test();

    virtual ~Command_Test();

    virtual uint8_t execute(uint16_t& input);

protected:
    
    // found this online
    uint16_t checkFreeRam ();
};

#endif /* COMMAND_TEST_H_ */
