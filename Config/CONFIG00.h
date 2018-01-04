/*
 * CONFIG00.h
 *
 * Example Configuration File.
 * This is constructed this way to allow for compile time per board settings
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef CONFIG00_H_
#define CONFIG00_H_

#include "CONFIG_Structs.h"

namespace SENSORCONFIG
{

    int16_t conversion00( const float& voltage );
    int16_t conversion01( const float& voltage );

    // Sensor 0 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor00 = {
            //set min/max to 0 to not use min/max value clamping
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            0,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 1 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor01 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            1,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 2 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor02 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            2,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 3 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor03 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            3,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 4 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor04 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            4,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 5 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor05 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            5,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 6 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor06 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            6,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 7 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor07 = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            7,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    //number of sensors
    //there should not be more sensors than ADC inputs
    constexpr uint8_t NUMSENSORS = 8;
}

namespace CANCONFIG
{
    constexpr CAN_SETTINGS CAN0 = {
		0x00,
		0,
		0
    };

    constexpr CAN_SETTINGS CAN1 = {
		0x01,		//message id
		0,              //identifier extension
		0               //rtr
    };

    constexpr CANSensorChannel CANChannel0 = {
                CAN0,           // CAN Message ID for this channel
                50,             // milliseconds between updates
                {
                    SENSORCONFIG::Sensor00,		//sensors in data channel positions
                    SENSORCONFIG::Sensor01,
                    SENSORCONFIG::Sensor02,
                    SENSORCONFIG::Sensor03
                },
                4				//number of sensors on CANChannel
	};

    constexpr CANSensorChannel CANChannel1 = {
                CAN1,           // CAN Message ID for this channel
                100,            // milliseconds between updates
                {
                    SENSORCONFIG::Sensor04,
                    SENSORCONFIG::Sensor05,
                    SENSORCONFIG::Sensor06,
                    SENSORCONFIG::Sensor07
                },
                4
	};

    // all can channels
    constexpr CANSensorChannel CANChannels[] =
        { CANChannel0, CANChannel1 };
    //number of CAN Channels
    constexpr uint8_t NUMCANCHANNELS = 2;

    //Device id for commands over CAN
    constexpr uint8_t CANCMD_DEVID = 0x00;
}


#endif /* CONFIG00_H_ */
