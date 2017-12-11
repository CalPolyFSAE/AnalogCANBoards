/*
* CANSensorTimer.cpp
*
*  Created on: Dec 10, 2017
*      Author: root
*/

#include "CANSensorTimer.h"

CANSensorTimer::CANSensorTimer(uint16_t interval, uint16_t CANId) :
    TimingInterval(interval), CANID(CANId)
{
    ticksToSend = interval;
    activeSensors = 0;
}

bool CANSensorTimer::registerSensor(Sensor* sensor, CANCONFIG::CANDATAChannel dataChannel)
{
    uint8_t index = (uint8_t) dataChannel;
    if (index > CANCONFIG::CANMAXDATACHANNELS)          // check array bounds
        return false;
    if (sensors[index] != nullptr) // make sure another sensor is not here
        return false; // if there is TODO: ADD critical error functionality

    //sort out CAN message size
    if (index >= activeSensors)
        activeSensors = index + 1;
    sensors[index] = sensor;
    return true;
}

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
            if(sensors[i]->requestADCRead())
            {
                ++i;
            }
        }
        //package CAN message using sensor values
        //TODO: CAN Message sending
        /////////////////
        /////////////////
        ////////////////
    }
}

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


