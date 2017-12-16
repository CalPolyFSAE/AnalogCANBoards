/*
 * CONFIG_Structs.h
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_STRUCTS_H_
#define CONFIG_CONFIG_STRUCTS_H_


#include <stdint.h>
#include "../Sensor.h"
#include "../CANSensorTimer.h"

//this contains struct definitions that allow for specific configuration only
//functionality.

namespace SENSORCONFIG
{
	
}

//these are needed to tie CANIDs to Timing intervals and Sensors
namespace CANCONFIG
{	
	struct CAN_SETTINGS
	{
		//use for this CANMSG
		//MOB settings for this CAN channel
		CANSensorTimer::CAN_ID CANId;
		uint8_t ide;
		uint8_t rtr;
	};

    struct CANSensorChannel
    {
		CAN_SETTINGS CANSettings;
        //milliseconds between Message sending
        uint16_t TimingInterval;
		//sensors on channel in data channel positions
		Sensor::SENSOR_SETTINGS SensDATA[CANSensorTimer::CANMAXDATACHANNELS];
		uint8_t NumSensors;
    };

}


#endif /* CONFIG_CONFIG_STRUCTS_H_ */
