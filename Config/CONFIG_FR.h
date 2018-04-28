/*
 * CONFIG_FR.h
 *
 *  Created on: Apr 23, 2018
 *      Author: root
 */

#ifndef CONFIG_CONFIG_FR_H_
#define CONFIG_CONFIG_FR_H_

#include "CONFIG_Structs.h"
#include "../float16.h"

namespace SENSORCONFIG
{

    inline int16_t conversion_none( float const& voltage )
    {
        // normalize across domain of 16 bit signed int
        //(2^15-1)/500 = 65.536
        return (int16_t)(voltage * 65.536);
    }

    inline int16_t conversionAccelFX( float const& voltage )
    {
        //Accel F X' .326x+1.6317
        float output = voltage * 0.326 - 1.6317;
        //output = (output < 0) ? 0 : output; // clamp at 0
        return (int16_t)(output * 65.536);
    }

    inline int16_t conversionAccelFY( float const& voltage )
    {
        //.3126x+1.638
        float output = voltage * 0.3126 + 1.638;
        //output = (output < 0) ? 0 : output; // clamp at 0
        return (int16_t)(output * 65.536);
    }

    inline int16_t conversionBP( float const& voltage )
    {
        //0 psi = 0.5V, 2500psi = 5V
        //
        float output = voltage * 499.9 + 0.5;
        //output = (output < 0) ? 0 : output; // clamp at 0
        return (int16_t) (output * 65.536);
    }

    // Sensor 0 Name
    constexpr Sensor::SENSOR_SETTINGS SensorSPFL = {
            //set min/max to 0 to not use min/max value clamping
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            0,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    // Sensor 1 Name
    constexpr Sensor::SENSOR_SETTINGS SensorSPFR = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            1,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    // Sensor 2 Name
    constexpr Sensor::SENSOR_SETTINGS SensorPushRodRight = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            2,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    // Sensor 3 Name
    constexpr Sensor::SENSOR_SETTINGS SensorSteeringPot = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            3,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    // Sensor 4 Name
    constexpr Sensor::SENSOR_SETTINGS SensorAccelX = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            4,                                          // ADC Channel to read
            conversionAccelFX                           // Function to convert data
    };

    // Sensor 5 Name
    constexpr Sensor::SENSOR_SETTINGS SensorAccelY = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            5,                                          // ADC Channel to read
            conversionAccelFY                           // Function to convert data
    };

    // Sensor 6 Name
    constexpr Sensor::SENSOR_SETTINGS SensorBPFront = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            6,                                          // ADC Channel to read
            conversionBP                                // Function to convert data
    };

    // Sensor 7 Name
    constexpr Sensor::SENSOR_SETTINGS SensorBPRear = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            7,                                          // ADC Channel to read
            conversionBP                                // Function to convert data
    };

    //number of sensors
    //there should not be more sensors than ADC inputs
    constexpr uint8_t NUMSENSORS = 7;
}

namespace CANCONFIG
{
    constexpr CAN_SETTINGS CAN0 = {
                0xC0,
                0,
                0
    };

    constexpr CAN_SETTINGS CAN1 = {
                0xF0,           //message id
                0,              //identifier extension
                0               //rtr
    };

    constexpr CAN_SETTINGS CAN2 = {
                0xF1,           //message id
                0,              //identifier extension
                0               //rtr
    };


    constexpr CANSensorChannel CANChannel0 = {
                CAN0,           // CAN Message ID for this channel
                1,              // milliseconds between updates
                {
                    SENSORCONFIG::SensorSPFL,             //sensors in data channel positions
                    SENSORCONFIG::SensorSPFR
                    //SENSORCONFIG::SensorPushRodRight
                },
                2                               //number of sensors on CANChannel
        };

    constexpr CANSensorChannel CANChannel1 = {
                CAN1,           // CAN Message ID for this channel
                4,              // milliseconds between updates
                {
                    SENSORCONFIG::SensorSteeringPot,
                    SENSORCONFIG::SensorAccelX,
                    SENSORCONFIG::SensorAccelY,
                    SENSORCONFIG::SensorBPFront
                },
                4               //number of sensors on CANChannel
        };

    constexpr CANSensorChannel CANChannel2 = {
                CAN2,           // CAN Message ID for this channel
                4,              // milliseconds between updates
                {
                    SENSORCONFIG::SensorBPRear
                },
                1               //number of sensors on CANChannel
        };

    // all can channels
    constexpr CANSensorChannel CANChannels[] =
        {
                CANChannel0,
                CANChannel1,
                CANChannel2
        };
    //number of CAN Channels
    constexpr uint8_t NUMCANCHANNELS = 3;

    //Device id for commands over CAN
    constexpr uint8_t CANCMD_DEVID = 0x01;
}


#endif /* CONFIG_CONFIG_FR_H_ */
