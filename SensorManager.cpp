/*
 * SensorManager.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "SensorManager.h"

#include "Config/CONFIG.h"

//static memory initialization
Sensor* SensorManager::channels[] = {};
CANSensorTimer* SensorManager::CANMessageTimers[] = {};

//TODO: put configuration data into progmem
void SensorManager::Init()
{
    // create timers for CAN message timing
    for (uint8_t i = 0; i < CANCONFIG::CANCHANNELS; ++i)
    {
        CANMessageTimers[i] = new CANSensorTimer (
                CANCONFIG::CANChannels[i].TimingInterval, CANCONFIG::CANChannels[i].CANID );
    }

    //create sensors
    //sensors could be part of static memory, but this would require restructuring
    //the config headers
    for (uint8_t i = 0; i < SENSORCONFIG::SENSORS; ++i)
    {
        Sensor* sens = new Sensor (SENSORCONFIG::SensorDatas[i]);//create sensor
        channels[i] = sens;
        //figure out which sensors go on which CAN messages
        for (uint8_t canc = 0; canc < CANCONFIG::CANCHANNELS; ++canc)
        {
            if (CANMessageTimers[canc]->CANID == SENSORCONFIG::SensorDatas[i].CANChan.CANID)
            {
                if(!CANMessageTimers[canc]->registerSensor(sens, SENSORCONFIG::SensorDatas[i].CANDataChannel))
                {
                    //TODO: Error message CAN / SERIAL
                    //or error is triggered in register sensor?
                }
            }
        }
    }
}

//1000Hz interrupt timing call
void SensorManager::INT_UpdateTiming()
{
    //update the CANSensorTimers
    for(uint8_t i = 0; i < CANCONFIG::CANCHANNELS; ++i)
    {
        CANMessageTimers[i]->Update();
    }
}


