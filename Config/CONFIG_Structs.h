/*
 * CONFIG_Structs.h
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_STRUCTS_H_
#define CONFIG_CONFIG_STRUCTS_H_


#include <stdint.h>
#include "../CAN/CANRXTX.h"
#include "../Sensor.h"
#include "../CANSensorTimer.h"

//this contains struct definitions that allow for specific configuration only
//functionality.

//these are needed to tie MOBs to Timing intervals and Sensors

namespace CANCONFIG
{

    typedef struct CANChannel
    {
        //MOB settings for this CAN channel
        CANRXTX::MOB_SETTINGS CANMOB;
        //milliseconds between Message sending
        uint16_t TimingInterval;
    } CANChannel;

}

namespace SENSORCONFIG
{
    typedef struct SensorChannel
    {
        Sensor::SENSOR_SETTINGS SensorData;                 // Sensor
        CANCONFIG::CANChannel CANChannel;               // CAN Channel to use for this sensor
        CANSensorTimer::CANDATAChannel CANDATAChannel;  // CAN Data Channel to use for this sensor
    } SensorChannel;
}


#endif /* CONFIG_CONFIG_STRUCTS_H_ */
