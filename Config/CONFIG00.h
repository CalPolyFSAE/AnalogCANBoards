/*
 * CONFIG01.h
 *
 * Example Configuration File.
 * This is constructed this way to allow for compile time settings
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef CONFIG00_H_
#define CONFIG00_H_

#include <stdint.h>
#include "CONFIG_Structs.h"

namespace CANCONFIG
{

    constexpr CANChannel CANChannel0 = {
                0x01,           // CAN Message ID for this channel
                50              // milliseconds between updates
            };

    constexpr CANChannel CANChannel1 = {
                0x02,           // CAN Message ID for this channel
                100             // milliseconds between updates
            };

    //list in order of increasing CANID
    constexpr CANChannel CANChannels[] =
        { CANChannel0, CANChannel1 };

    constexpr uint8_t CANCHANNELS = 2;
}


namespace SENSORCONFIG
{
    int16_t conversion00(uint16_t);
    int16_t conversion01(uint16_t);

    // Sensor 0 Name
    constexpr SensorData Sensor00 = {
            5,                                          // Min expected value (converted)
            16,                                         // Max expected value (converted)
            CANCONFIG::CANChannel0,                     // CAN Channel to use for this sensor
            CANCONFIG::CANDATAChannel::CANCHANNEL0,     // CAN Data Channel to use for this sensor
            7,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 1 Name
    constexpr SensorData Sensor01 = {
            5,                                          // Min expected value (converted)
            16,                                         // Max expected value (converted)
            CANCONFIG::CANChannel0,                     // CAN Channel to use for this sensor
            CANCONFIG::CANDATAChannel::CANCHANNEL1,     // CAN Data Channel to use for this sensor
            7,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 2 Name
    constexpr SensorData Sensor02 = {
            5,                                          // Min expected value (converted)
            16,                                         // Max expected value (converted)
            CANCONFIG::CANChannel1,                     // CAN Channel to use for this sensor
            CANCONFIG::CANDATAChannel::CANCHANNEL0,     // CAN Data Channel to use for this sensor
            7,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    constexpr SensorData SensorDatas[] = {
            Sensor00,
            Sensor01,
            Sensor02
    };

    constexpr uint8_t SENSORS = 3;
}


#endif /* CONFIG00_H_ */
