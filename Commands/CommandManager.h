/*
 * CommandManager.h
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#ifndef COMMANDS_COMMANDMANAGER_H_
#define COMMANDS_COMMANDMANAGER_H_

#include <stdint.h>

class CommandManager
{
public:

    enum WarningMessage : uint8_t
    {
        FailedToInit,
        ConfigError,
        ADCDataOutOfRange,
        GeneralError
    };

    // must be called first
    static void Init();

    // flags a command to be executed(external classes can request a command)
    // for debugging only
    static void FlagCommandToExe(uint8_t cmdID, const uint16_t& input);

    // command is executed during this function
    static void Update();

    //used to output warning message
    static void LogWarning(WarningMessage errID);

private:
    //gets message associated with warning
    static const char* WarningToStr(WarningMessage msg, char* output);

private:

    // array of pointers to all commands
    static class Command* Commands[];
    static uint8_t NUM_COMMANDS;

    // the command that needs to be executed
    static class Command* CurrentCommand;
    static uint16_t CurrentArgs;

};


#endif /* COMMANDS_COMMANDMANAGER_H_ */
