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
#include "Sensor.h"
#include "CANSensorTimer.h"

class SensorManager
{
public:

    //sets up channels array
    static void Init();

    //1000Hz interrupt calls this
    static void INT_UpdateTiming();

private:
    //all Sensors
    static Sensor* channels[SENSORCONFIG::SENSORS];

    //CAN Message Timers
    static CANSensorTimer* CANMessageTimers[CANCONFIG::CANCHANNELS];

};


#endif /* SENSORMANAGER_H_ */
