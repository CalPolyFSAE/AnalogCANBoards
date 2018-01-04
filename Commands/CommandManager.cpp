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

static const PROGMEM char WarningMsg_FailedToInit[] = "Failed to Initialize";
static const PROGMEM char WarningMsg_ConfigError[] = "Configuration is Incorrect";
static const PROGMEM char WarningMsg_ADCDataOutOfRange[] = "ADC value out of Range";
static const PROGMEM char WarningMsg_GeneralError[] = "General Error";

static constexpr uint8_t MAX_WARNMSG_LEN = 150;

#endif //SERIAL_TEXT

#ifdef COMMANDS //only create the commands if they are enabled


//add all commands here
static Command_Test cmdtst;

Command* CommandManager::Commands[] =
    {
            &cmdtst
    };
uint8_t CommandManager::NUM_COMMANDS = 1;
Command* CommandManager::CurrentCommand = nullptr;
uint16_t CommandManager::CurrentArgs = 0;

#endif //COMMANDS


void CommandManager::Init()
{
#ifdef SERIAL_TEXT
    //setup USART
    Serial.begin(SERIAL_BAUD);
#endif //SERIAL_TEXT
    //setup CAN Controller hardware
    CAN.set_baudrate(CAN_BAUD);
    CAN.init(1);
}

void CommandManager::FlagCommandToExe(uint8_t cmdID, const uint16_t& input)
{
#ifdef COMMANDS
    // find command with ID and mark it for execution
    if(CurrentCommand == nullptr)
    {
        for (uint8_t i = 0; i < NUM_COMMANDS; ++i)
        {
            if (Commands[i]->getID () == cmdID)
            {
                CurrentCommand = Commands[i];
                //copy atomically because it is 16 bit
                ATOMIC_BLOCK(ATOMIC_FORCEON)
                {
                    CurrentArgs = input;
                }
            }
        }
    }else
    {
        //TODO: error still waiting for command to exe
    }
#endif //COMMANDS
}

void CommandManager::Update()
{
#ifdef COMMANDS
    if(CurrentCommand)
    {
        CurrentCommand->execute(CurrentArgs);
        CurrentCommand = nullptr;
        CurrentArgs = 0;
    }
#endif // COMMANDS
}

void CommandManager::LogWarning(WarningMessage errID)
{
#ifdef SERIAL_TEXT

    char buf [MAX_WARNMSG_LEN]; //buffer for text
    WarningToStr(errID, buf);
    Serial.println(buf);

#endif //SERIAL_TEXT
    //TODO: CAN errors
}

const char* CommandManager::WarningToStr(WarningMessage msg, char* output)
{

#ifdef SERIAL_TEXT
    //pointer to data in flash
    const char* progWord = nullptr;

    switch(msg)
    {
        case WarningMessage::FailedToInit:
            progWord = WarningMsg_FailedToInit;
            break;
        case WarningMessage::ConfigError:
            progWord = WarningMsg_ConfigError;
            break;
        case WarningMessage::ADCDataOutOfRange:
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
