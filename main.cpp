/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "ADCManager.h"
#include "Config/CONFIG.h"
#include "SensorManager.h"
#include "Commands/CommandManager.h"

#include <AVRLibrary/arduino/Arduino.h>//for serial and CAN stuff

void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
}

//interrupt function for timing
SIGNAL(TIMER1_COMPA_vect) {
    SensorManager::INT_UpdateTiming();
}

//interrupt function for ADC completion
ISR(ADC_vect)
{
    ADCManager::INT_ADCFinished();
}

int main() {
    cli();
	
    //setup commands first for error reporting
    CommandManager::Init();

    //some configuration checks
    if(SENSORCONFIG::NUMSENSORS > MAXSENSORS)
    {
        //there shouldn't be more than one CAN Channel per sensor
        //this means that there is either too many sensors,
        //or one sensor has been assigned multiple times
        //
        CommandManager::LogWarning(CommandManager::WarningMessage::ConfigError);
    }

    //setup 1000Hz interrupt
    timer1_init();

    //setup ADC sample rate
    ADCManager::Init();

    //setup message timing
    SensorManager::Init();

    sei();

    //CommandManager::FlagCommandToExe(2, 123);

    //TODO: add pause state (for CMDs)
    while(true)
    {
        Serial.flush();
		
        //FOR TESTING ONLY
        uint32_t c = 0;
        while(c < 150000)
        {
            ++c;
        }

        //for testing Serial input
        int16_t input = Serial.read();
        if(input != -1)
        {
            Serial.println(input);
        }

        SensorManager::Update();
        CommandManager::Update();
    }
}
