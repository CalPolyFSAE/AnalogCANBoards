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

//TODO: TESTING:
volatile bool sendCAN = false;
uint32_t totalUs = 0;
uint16_t iterations = 0;

void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
}

//increments every 128us, overflows every 152.7 hours
volatile uint32_t timer2Count = 0;

void timer2_init()
{

    TCCR2A |= _BV(WGM21) | _BV(CS21);//CTC mode, and set prescaler to 8(CTC mode compares counter with OCR2A and interrupts on match)
    TCNT2 = 0;//init counter
    //timer clock speed is 2Mhz (0.5us)
    //(16MHz/8) = 2mhz
    OCR2A = 255;//interrupt will fire every 256/2 microseconds (128)
    TIMSK2 |= _BV(OCIE2A);//only want compare interrupt
}

//microseconds since start
float getMicros()
{
    cli();

    //check if there is currently an interrupt flag (this will only be set if the
    // interrupt has not yet been handled)
    bool flagTifr2 = TIFR2 & _BV(OCF2A);
    if(flagTifr2)
    {
        ++timer2Count;
        TIFR2 |= ~(_BV(OCF2A));//clear interrupt flag
    }

    uint8_t timer = TCNT2;
    float out = (float)(timer2Count*128) + timer/2.0f;

    sei();

    return out;
}

//interrupt function for timing
ISR(TIMER1_COMPA_vect) {
    sendCAN = true;
    SensorManager::GetInstance().INT_UpdateTiming();
    CommandManager::StaticClass().INT_UpdateTiming();
}

ISR(TIMER2_COMP_vect)
{
    ++timer2Count;
}

//interrupt function for ADC completion
ISR(ADC_vect) {
    ADCManager::INT_ADCFinished ();
}

int main()
{
    cli();
	
    //setup commands first for error reporting
    CommandManager* ACommandManager = &CommandManager::StaticClass();

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

    //setup us timer
    timer2_init();

    //setup ADC sample rate
    ADCManager::Init();
    CANRaw::StaticClass().Init(CANRaw::CAN_BAUDRATE::B1M);

    //get the SensorManager(also initializes it)
    SensorManager* ASensorManager = &SensorManager::GetInstance();

    sei();



    CommandManager::StaticClass().ExecuteCommand(0, 0);


    float loopTiming = 0.0f;
    //TODO: add pause state (for CMDs) and add main loop to a manager class
    while(true)
    {
        //FOR TESTING ONLY
        /*
        uint32_t c = 0;
        while(c < 150000)
        {
            ++c;
        }
        */

        /*//for testing Serial input
        int16_t input = Serial.read();
        if(input != -1)
        {
            Serial.println(input);
        }*/

        //TODO: testing

        //timing the update length
        loopTiming = getMicros();

        ASensorManager->Update();
        ACommandManager->Update();

        loopTiming = getMicros() - loopTiming;

        //previous
        totalUs += uint16_t(loopTiming*2);
        ++iterations;

        if(sendCAN)
        {
            //TODO: TESTING:
            sendCAN = false;
            uint8_t canData[8] =
                { };

            //go from 0.5us resolution to int
            canData[0] = totalUs/iterations;//moving average
            totalUs = 0;
            iterations = 0;

            //hton
            uint8_t tmp = canData[0];
            canData[0] = canData[1];
            canData[1] = tmp;

            //canid
            /*uint16_t CANID = 0xD5;

            //send CAN message
            st_cmd_t CMD =
                { };
            // set up command for can lib
            CMD.cmd = can_cmd_t::CMD_TX_DATA;
            CMD.dlc = 2;        // data size

            CMD.id.std = CANID;
            CMD.ctrl.ide = 0;

            CMD.pt_data = canData;

            // send command to CAN lib
            CAN.cmd (&CMD);

            uint16_t i = 0;
            // wait for CAN message to send

            while (CAN.get_status (&CMD) != CAN_STATUS_COMPLETED && i < 65000)
            {
                ++i;
            }*/
        }


    }
}
