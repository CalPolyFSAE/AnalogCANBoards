/*
 * SensorManager.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "SensorManager.h"

#include "Config/CONFIG.h"

Sensor* SensorManager::channels[] = {};

SensorManager::CANMessageTimer* SensorManager::CANMessageTimers[] = {};

void SensorManager::Init()
{
    // create timers for CAN message timing
    for (uint8_t i = 0; i < CANCHANNELS; ++i)
    {
        CANMessageTimers[i] = new CANMessageTimer (
                SensorDatas[i].CANChan.TimingInterval, SensorDatas[i].CANChan.CANID);
    }

    for (uint8_t i = 0; i < SENSORS; ++i)
    {
        Sensor* sens = new Sensor (SensorDatas[i]);//create sensor
        channels[i] = sens;
        //figure out which sensors go on which CAN messages
        for (uint8_t canc = 0; canc < CANCHANNELS; ++canc)
        {
            if (CANMessageTimers[canc]->CANID == SensorDatas[i].CANChan.CANID)
            {
                CANMessageTimers[canc]->registerSensor(sens, SensorDatas[i].CANDataChannel);
            }
        }
    }
}


void SensorManager::INT_UpdateTiming()
{

}


