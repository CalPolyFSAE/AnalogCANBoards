/*
 * SensorManager.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "SensorManager.h"
#include "Config/CONFIG.h"
#include "Sensor.h"
#include "CANSensorTimer.h"

//static memory initialization
Sensor* SensorManager::AllSensors[] = {};
CANSensorTimer* SensorManager::CANMessageTimers[] = {};

//TODO: put configuration data into progmem?
void SensorManager::Init()
{
    uint8_t allSensorsIndex = 0;
    // create timers for CAN message timing and register sensors
    for (uint8_t channelIndex = 0; channelIndex < CANCONFIG::NUMCANCHANNELS;
            ++channelIndex)
    {
        CANCONFIG::CANSensorChannel tmpCANChan =
                CANCONFIG::CANChannels[channelIndex];
        //create timer
        CANSensorTimer* timer = new CANSensorTimer (
                tmpCANChan.TimingInterval, &tmpCANChan.CANSettings.CANId,
                tmpCANChan.CANSettings.ide);
        CANMessageTimers[channelIndex] = timer;

        //assign sensors to CANSensorTimer
        for (uint8_t sensorIndex = 0; sensorIndex < tmpCANChan.NumSensors;
                ++sensorIndex)
        {
            Sensor* sensor = new Sensor (tmpCANChan.SensDATA[sensorIndex]);
            AllSensors[allSensorsIndex] = sensor;
            ++allSensorsIndex;
            if (!timer->registerSensor (
                    sensor, CANSensorTimer::CANDATAChannel (sensorIndex)))
            {
                //TODO: Error message
            }
        }
    }
}

void SensorManager::Update()
{
    //update the CANSensorTimers
    for (uint8_t i = 0; i < CANCONFIG::NUMCANCHANNELS; ++i)
    {
        CANMessageTimers[i]->Update ();
    }
}

//1000Hz interrupt timing call
void SensorManager::INT_UpdateTiming()
{
    //update the CANSensorTimers
    for (uint8_t i = 0; i < CANCONFIG::NUMCANCHANNELS; ++i)
    {
        CANMessageTimers[i]->INT_Call_Tick ();
    }
}


