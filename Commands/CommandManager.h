/*
 * CommandManager.h
 *
 *  Created on: Jan 3, 2018
 *      Author: root
 */

#ifndef COMMANDS_COMMANDMANAGER_H_
#define COMMANDS_COMMANDMANAGER_H_

#include <stdint.h>

/** This is a singleton class */
class CommandManager
{
public:

    // this is what the incoming Command data should look like
    struct CommandData
    {
        uint8_t devID;
        uint8_t cmdID;
        uint16_t input;
        uint32_t unused;
    };

    //TODO: move messages to a command?
    enum Message : uint8_t
    {
        FailedToInit,
        ConfigError,
        ADCDataOutOfRange,
        GeneralError
    };

    //gets the CommandMnager singleton
    //TODO: can this inline? (maybe make the static var member)
    static CommandManager& StaticClass();

    // flags a command to be executed(external classes can request a command)
    // for debugging only
    // TODO: add a command queue?
    void ExecuteCommand(uint8_t cmdID, const uint16_t& input);

    // requested command is executed during this function
    // also checks for more command requests
    void Update();

    // called by timer interrupt at 1000Hz
    //TODO: make this a CANListener so it doesn't have to poll
    inline void INT_UpdateTiming()
    {
        if(canRXTimer > 0)
        {
            --canRXTimer;
        }else
        {
            canRXTimer = CanRXCheckInterval;
            checkForCMD = true;
        }
    }

    //used to output message
    void LogMessage(Message errID);
    void LogMessage(const char c[]);

    //c++11 delete the functions we don't want
    CommandManager(CommandManager const&) = delete;
    void operator=(CommandManager const&) = delete;

private:
    // prevent creation of a new instance
    CommandManager();
    // not sure if this should be private
    ~CommandManager();

    //gets message associated with warning
    const char* MessageToStr(Message msg, char* output);

    //check for CAN RX
    void CheckForCANCMD();

private:
    //used to keep track of CAN polling
    static constexpr uint8_t CanRXCheckInterval = 50;
    volatile uint8_t canRXTimer;
    volatile bool checkForCMD;

    //st_cmd_t canCMD;
    //CAN RX data
    uint8_t data[8];

    // array of pointers to all commands
    static constexpr uint8_t NUM_COMMANDS = 1;
    class Command* commands[NUM_COMMANDS];

    // the command that needs to be executed
    class Command* currentCommand;
    uint16_t currentArgs;

};


#endif /* COMMANDS_COMMANDMANAGER_H_ */
