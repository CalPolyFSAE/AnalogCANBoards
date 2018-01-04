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

class SensorManager
{
public:

    //sets up sensors and CAN timers
    static void Init();

    //main program loop calls this
    static void Update();

    //1000Hz interrupt calls this
    static void INT_UpdateTiming();

private:

    //all Sensors (used for diagnostics) TODO: implement
    static class Sensor* AllSensors[SENSORCONFIG::NUMSENSORS];

    //all CAN Message Timers (used for update loop)
    static class CANSensorTimer* CANMessageTimers[CANCONFIG::NUMCANCHANNELS];

};


#endif /* SENSORMANAGER_H_ */
