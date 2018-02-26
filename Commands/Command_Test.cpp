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
    CommandID = 0;
}

Command_Test::~Command_Test()
{

}

uint8_t Command_Test::execute(uint16_t& input)
{
    CommandManager::StaticClass().LogMessageln(FSTR("Command_Test"));
    return 0;
}

uint16_t Command_Test::checkFreeRam()
{
    extern int __heap_start, *__brkval;
	uint16_t v;
    return (uint16_t) &v - (__brkval == 0 ? (uint16_t) &__heap_start : (uint16_t) __brkval);
}
