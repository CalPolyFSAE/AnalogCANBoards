/*
 * SensorManager.h
 *
 * Manages the Sensor object timing
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef SENSORMANAGER_H_
#define SENSORMANAGER_H_


#include <stdint.h>
#include <util/atomic.h>
#include "Config/CONFIG.h"

/** This is a singleton class */
class SensorManager
{
public:
    static SensorManager& GetInstance();

    //main program loop calls this
    inline void Update() {
        //update the CANSensorTimers
        for (uint8_t i = 0; i < CANCONFIG::NUMCANCHANNELS; ++i)
        {
            CANMessageTimers[i]->Update ();
        }
    }

    //1000Hz interrupt calls this
    void INT_UpdateTiming();

    //c++11 just delete the functions we don't want
    SensorManager(SensorManager const&) = delete;
    void operator=(SensorManager const&) = delete;

private:
    //sets up sensors and CAN timers
    SensorManager();

private:

    //all Sensors (used for diagnostics) TODO: implement
    class Sensor* AllSensors[SENSORCONFIG::NUMSENSORS];

    //all CAN Message Timers (used for update loop)
    class CANSensorTimer* CANMessageTimers[CANCONFIG::NUMCANCHANNELS];

};


#endif /* SENSORMANAGER_H_ */
