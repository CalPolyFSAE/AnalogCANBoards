/*
* CANSensorTimer.cpp
*
*  Created on: Dec 10, 2017
*      Author: root
*/

#include "CANSensorTimer.h"
#include "Sensor.h"

#include <AVRLibrary/arduino/Arduino.h>

CANSensorTimer::CANSensorTimer(uint16_t interval, const CAN_ID* can_id, uint8_t can_ide) :
    TimingInterval(interval), id(*can_id), ide(can_ide)
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
// TODO: make static?
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
        CAN_Data canData = {};

        for (uint8_t i = 0; i < activeSensors; ++i)
        {
            if (sensors[i] == nullptr)
            {
                continue;
            }
            //TODO: Need to add variable data sizes for CAN data channels
            //CANData.data16[i] = sensors[i]->getValue();
            int16_t data = sensors[i]->getValue();
            canData.data16[i] = *(uint16_t*)(&data);// TODO: keep the sign bit?
            Serial.print(data);

            Serial.print (" CH: ");
            Serial.print (sensors[i]->ADCChannel);
            Serial.print (" V: ");
            float volts;
            sensors[i]->getVoltage (volts);
            Serial.print (volts, 4);
            Serial.println ("");
            
        }

        //send CAN message
        st_cmd_t CMD =
            { };
        // set up command for can lib
        CMD.cmd = can_cmd_t::CMD_TX_DATA;
        CMD.dlc = activeSensors * CANBYTESPERDATACHANNEL;// figure out data size
        if (ide)
        {
            CMD.id.ext = id.ext;
        }
        else
        {
            CMD.id.std = id.std;
        }
        CMD.ctrl.ide = ide;

        CMD.pt_data = canData.data;

        // send command to CAN lib
        CAN.cmd (&CMD);
        uint16_t i = 0;
        while(CAN.get_status(&CMD) != CAN_STATUS_COMPLETED && i < 65000)
            ++i;

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


