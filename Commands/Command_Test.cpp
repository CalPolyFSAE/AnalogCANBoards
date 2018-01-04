/*
 * Command_Test.cpp
 *
 * Created: 12/21/2017 4:09:33 PM
 *  Author: oneso
 */ 
#include "Command_Test.h"
#include "CommandManager.h"

Command_Test::Command_Test()
{
    CommandID = 2;
}

Command_Test::~Command_Test()
{

}

uint8_t Command_Test::execute(uint16_t& input)
{
    CommandManager::LogWarning(CommandManager::WarningMessage::ConfigError);
    return 0;
}
