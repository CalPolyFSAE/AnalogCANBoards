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

void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
}

//increments every 0.512 ms
volatile uint32_t timer2Count = 0;

void timer2_init()
{

    TCCR2A |= _BV(WGM21) | _BV(CS21) | _BV(CS20);//CTC mode, and set prescaler to 32(CTC mode compares counter with OCR2A and interrupts on match)
    TCNT2 = 0;//init counter
    //timer clock speed is 2Mhz (2us)
    //(16MHz/32) = 500kHz
    OCR2A = 255;//interrupt will fire every 256*2 microseconds (0.512 ms)
    TIMSK2 |= _BV(OCIE2A);//only want compare interrupt
}

//microseconds since start
float getMicros()
{
    cli();

    //check if there is currently an interrupt flag (this will only be set if the
    // interrupt has not yet been handled)
    if(TIFR2 & _BV(OCF2A))
    {
        ++timer2Count;
        TIFR2 &= ~(_BV(OCF2A));//clear interrupt flag
    }

    uint32_t timer2cntCp = timer2Count;
    uint8_t timer = TCNT2;
    sei();

    return (float)(timer2cntCp*512) + timer*2;
}

//interrupt function for timing
ISR(TIMER1_COMPA_vect) {
    SensorManager::GetInstance().INT_UpdateTiming();
}

ISR(TIMER2_COMP_vect)
{
    ++timer2Count;
}

//TODO: set up this interrupt as part of an ADCManager derived class
//interrupt function for ADC completion
ISR(ADC_vect) {
    ADCManager::StaticClass().INT_ADCFinished ();
}

int main()
{
    cli();

    //setup commands first for error reporting
    //CommandManager* ACommandManager = &CommandManager::StaticClass();

    //some configuration checks
    if(SENSORCONFIG::NUMSENSORS > MAXSENSORS)
    {
        //there shouldn't be more than one CAN Channel per sensor
        //this means that there is either too many sensors,
        //or one sensor has been assigned multiple times
        //

    }

    //setup 1000Hz interrupt
    timer1_init();

    //setup timer 2
    //timer2_init();

    //setup ADC sample rate
    ADCManager::StaticClass().Init();
    CANRaw::StaticClass().Init(CANRaw::CAN_BAUDRATE::B1M);

    //get the SensorManager(also initializes it)
    SensorManager* ASensorManager = &SensorManager::GetInstance();

    sei();

    // print reset flag status
    CommandManager::StaticClass().LogMessage(FSTR("START "));
    CommandManager::StaticClass().LogMessageln(MCUSR);
    CommandManager::StaticClass().LogMessage(FSTR("DEVID "));
    CommandManager::StaticClass().LogMessageln(CANCONFIG::CANCMD_DEVID);

    // test command (for testing)
    //CommandManager::StaticClass().ExecuteCommand(0, 0);


    ////////////////////////////////TESTING
    //CANRaw& can = CANRaw::StaticClass ();
    // create CAN Tx settings
    //CANRaw::CAN_FRAME_HEADER tmpHeader
    //   {       0xD5,         // can id
    //            0,          // rtr flag
    //            0,          // ide flag
    //            8 // dlc
    //    };
    //can.ForceResetMob (CANRaw::CAN_MOB::MOB_4); // make sure mob is ready
    //can.ConfigTx (tmpHeader, CANRaw::CAN_MOB::MOB_4);
    ////////////////////////////////

    //float loopTiming = 0.0f;
    //TODO: add pause state (for CMDs) and add main loop to a manager class
    while(true)
    {
        //TODO: testing

        //timing the update length
        //loopTiming = getMicros();

        ASensorManager->Update();

        //loopTiming = getMicros() - loopTiming;

        //TODO: TESTING:
        //CANRaw::CAN_DATA canData =
        //   { };

        //
        //canData.value = (uint16_t) (loopTiming);

        //hton
        //uint8_t tmp = canData.byte[0];
        //canData.byte[0] = canData.byte[1];
        //canData.byte[1] = tmp;

        //CANRaw::StaticClass ().TxData (canData, CANRaw::CAN_MOB::MOB_4);

        //loopTiming = getMicros() - loopTiming;
    }
}
