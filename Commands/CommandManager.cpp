/*
 * CommandManager.cpp
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#include "CommandManager.h"
#include "avr/pgmspace.h"
#include "util/atomic.h"
#include "AVRLibrary/arduino/Arduino.h"

#include "Command.h"
#include "Command_Test.h"
#include "../Config/CONFIG.h"

#ifdef SERIAL_TEXT

// stored in flash

// commands
static const PROGMEM char CommandText_Test[] = "";
static const PROGMEM char CommandText_Echo[] = "";

// messages
static const PROGMEM char WarningMsg_FailedToInit[] = "Failed to Initialize";
static const PROGMEM char WarningMsg_ConfigError[] = "Configuration is Incorrect";
static const PROGMEM char WarningMsg_ADCDataOutOfRange[] = "ADC value out of Range";
static const PROGMEM char WarningMsg_GeneralError[] = "General Error";

static constexpr uint8_t MAX_WARNMSG_LEN = 150;

#endif //SERIAL_TEXT


CommandManager& CommandManager::StaticClass()
{
    static CommandManager C{}; // initialized once

    return C;
}

void CommandManager::ExecuteCommand(uint8_t cmdID, const uint16_t& input)
{
    // find command with ID and mark it for execution
    if(currentCommand == nullptr)
    {
        for (uint8_t i = 0; i < NUM_COMMANDS; ++i)
        {
            if (commands[i]->getID () == cmdID)
            {
                currentCommand = commands[i];
                currentArgs = input;
            }
        }
    }else
    {
        //TODO: still waiting for command to exe
        // should only be one ExecuteCommand call per update loop
    }
}

void CommandManager::Update()
{
    if(currentCommand)
    {
        //TODO: add ability for commands to execute across multiple updates
        currentCommand->execute(currentArgs);
        currentCommand = nullptr;
        currentArgs = 0;
    }

    if(checkForCMD)
    {
        checkForCMD = false;

        // this currentCommand check is unnecessary right now
        //TODO: add ability for commands to execute across multiple updates
        if(currentCommand == nullptr)
        {
            checkForCMD = false;
            CheckForCANCMD();
        }
#ifdef SERIAL_TEXT
        //
        if(currentCommand == nullptr)
        {
            //TODO: read serial for command
        }
#endif //SERIAL_TEXT
    }
}

void CommandManager::LogMessage(Message errID)
{
#ifdef SERIAL_TEXT

    char buf [MAX_WARNMSG_LEN]; //buffer for text
    MessageToStr(errID, buf);
    Serial.println(buf);

#endif //SERIAL_TEXT
    //TODO: CAN errors
}

void CommandManager::LogMessage(const char c[])
{
#ifdef SERIAL_TEXT

    Serial.println(c);

#endif
}

void CommandManager::LogMessage( const __FlashStringHelper* c)
{
#ifdef SERIAL_TEXT

    Serial.print(c);

#endif
}


CommandManager::CommandManager() :
        commands { new Command_Test () }
{
    currentCommand = nullptr;
    currentArgs = 0;
    //TODO: make command checks based on CANRx INT
    canRXTimer = CanRXCheckInterval; // check for CMD every 50ms
    checkForCMD = false;



#ifdef SERIAL_TEXT
    //setup USART
    Serial.begin(SERIAL_BAUD);
#endif //SERIAL_TEXT
}

CommandManager::~CommandManager()
{
    for(uint8_t i = 0; i < NUM_COMMANDS; ++i)
        delete commands[i];
}

const char* CommandManager::MessageToStr(Message msg, char* output)
{

#ifdef SERIAL_TEXT
    //pointer to data in flash
    const char* progWord = nullptr;

    switch(msg)
    {
        case Message::FailedToInit:
            progWord = WarningMsg_FailedToInit;
            break;
        case Message::ConfigError:
            progWord = WarningMsg_ConfigError;
            break;
        case Message::ADCDataOutOfRange:
            progWord = WarningMsg_ADCDataOutOfRange;
            break;
        default:
            progWord = WarningMsg_GeneralError;
            break;
    }

    strncpy_P(output, (char*)progWord, MAX_WARNMSG_LEN);

#endif //SERIAL_TEXT

    return output;
}

void CommandManager::CheckForCANCMD()
{
    /*
    switch(CAN.get_status(&canCMD))
    {
        case CAN_STATUS_COMPLETED:
            // check out the data we got
            CommandData* commandData = (CommandData*)data;
            if(commandData->devID == CANCONFIG::CANCMD_DEVID)
            {
                ExecuteCommand(commandData->devID, commandData->input);
            }
            // request command again
        case CAN_STATUS_ERROR:
            // something went wrong, or we just haven't yet sent command
            CAN.cmd(&canCMD);
            break;
        case CAN_STATUS_NOT_COMPLETED:
            // keep waiting
            break;
    }
    */
}
