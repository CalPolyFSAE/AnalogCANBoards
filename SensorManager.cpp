/*
 * SensorManager.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "SensorManager.h"
#include "CAN/CANRXTX.h"
#include "Config/CONFIG.h"

//static memory initialization
//Sensor* SensorManager::AllSensors[] = {};
CANSensorTimer* SensorManager::CANMessageTimers[] = {};

//TODO: put configuration data into progmem
void SensorManager::Init()
{
    // create timers for CAN message timing and register sensors
    for (uint8_t channelIndex = 0; channelIndex < CANCONFIG::NUMCANCHANNELS; ++channelIndex)
    {
        CANRXTX::MOB_SETTINGS tmpMOB = CANCONFIG::CANChannels[channelIndex].CANMOB;
        //get next MOB to configure
        uint8_t mobNum = CANRXTX::GetNextDisabledMOB ();
        if (CANRXTX::ConfigureMOB (mobNum, tmpMOB, false) != 0)
        {
            //failed to configure this MOB
            //TODO: some other terrible thing happened
        }

        //create timer
        CANMessageTimers[channelIndex] = new CANSensorTimer (
                CANCONFIG::CANChannels[channelIndex].TimingInterval,
                mobNum);

        //find the sensors that go on this CAN timer
        for (uint8_t sensorIndex = 0; sensorIndex < SENSORCONFIG::NUMSENSORS; ++sensorIndex)
        {
            SENSORCONFIG::SensorChannel tmpSensorChannel = SENSORCONFIG::SensorChannels[sensorIndex];
            if(tmpSensorChannel.CANChannel.CANMOB.id == tmpMOB.id)
            {
                Sensor* sens = new Sensor(tmpSensorChannel.SensorData);

                //register sensor with timer
                if (!CANMessageTimers[channelIndex]->registerSensor (
                        sens, tmpSensorChannel.CANDATAChannel))
                {
                    //TODO: Error message CAN / SERIAL
                    //or error is triggered in register sensor?
                }
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


