/*
* CANSensorTimer.cpp
*
*  Created on: Dec 10, 2017
*      Author: root
*/

#include "CANSensorTimer.h"

#include <AVRLibrary/arduino/Arduino.h>//FOR TESTING ONLY

CANSensorTimer::CANSensorTimer(uint16_t interval, uint8_t MOBnum) :
    TimingInterval(interval), MOBn(MOBnum)
{
    ticksToSend = interval;
    activeSensors = 0;
}

//registers a sensor on this CANChannel at dataChannel position
bool CANSensorTimer::registerSensor(Sensor* sensor, CANDATAChannel dataChannel)
{
    uint8_t index = (uint8_t) dataChannel;
    if (index > CANMAXDATACHANNELS)          // check array bounds
        return false;
    if (sensors[index] != nullptr) // make sure another sensor is not here
        return false; // if there is TODO: ADD critical error functionality

    //sort out CAN message size (for dlc)
    if (index >= activeSensors)
        activeSensors = index + 1;

    Serial.print("CANSensorTimer::registerSensor() activeSensors: ");//TESTING ONLY
    Serial.println(activeSensors);

    //register the sensor
    sensors[index] = sensor;
    return true;
}

//do not call. this is used for interrupt timing
void CANSensorTimer::INT_Call_Tick()
{
    --ticksToSend;
    if(ticksToSend == 0)
    {
        needToSend = true;
        ticksToSend = TimingInterval;
    }
}

//make sensors request data if a CAN message needs to be sent
//then send data over CAN
void CANSensorTimer::Update()
{
    if(needToSend)
    {
        //read data on all sensors
        for(uint8_t i = 0; i < activeSensors;)
        {
            //only go to next sensor after the request is successful
            //this should only take multiple requests if ADC clock speed is
            //very slow
            if(sensors[i] == nullptr)
            {
                ++i;
            }
            else if(sensors[i]->requestADCRead())
            {
                ++i;
            }
        }

        //wait for all Sensors to get value from ADC
        /*
        for(uint8_t i = 0; i < activeSensors;)
        {
            if(sensors[i]->getIsReady())
            {
                ++i;
            }
        }
        */
        while(!sensors[activeSensors - 1]->getIsReady())
            ;


        //package CAN message using sensor values
        //TODO: CAN Message sending
        CANRXTX::CAN_DATA CANData = {};

        for (uint8_t i = 0; i < activeSensors; ++i)
        {
            if(sensors[i] == nullptr)
            {
                continue;
            }
            CANData.data[i * CANBYTESPERDATACHANNEL] = sensors[i]->getValue();

            if(sensors[i]->ADCChannel == 5)//TESTING ONLY
            {
                Serial.println (sensors[i]->getVoltage (), 4);
            }
            /*
            Serial.print(" SEN: ");
            Serial.print(i);
            Serial.print(" CH: ");
            Serial.print(sensors[i]->ADCChannel);
            Serial.print(" V: ");
            Serial.print(sensors[i]->getVoltage(), 4);
            Serial.println("");
            */
        }

        CANRXTX::TX_UsingMOB(MOBn, &CANData, activeSensors * CANBYTESPERDATACHANNEL);

        /////////////////
        /////////////////
        ////////////////
        needToSend = false;
    }
}

//returns ticks until the CAN message needs to be sent
//time value dependent on how the interrupt timer is configured
uint16_t CANSensorTimer::timeUntilSend()
{
    //need to copy this atomically because of INT_Call_Tick changing value
    uint16_t time;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        time = ticksToSend;
    }
    return time;
}

uint8_t CANSensorTimer::getNumActiveSensors()
{
    return activeSensors;
}


