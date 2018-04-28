/*
 * CONFIG_LR.h
 *
 *  Created on: Apr 26, 2018
 *      Author: root
 */

#ifndef CONFIG_CONFIG_LR_H_
#define CONFIG_CONFIG_LR_H_


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
    constexpr Sensor::SENSOR_SETTINGS SensorSPRL = {
            //set min/max to 0 to not use min/max value clamping
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            0,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    // Sensor 1 Name
    constexpr Sensor::SENSOR_SETTINGS SensorSPRR = {
            0,                                          // Min expected value (converted)
            0,                                          // Max expected value (converted)

            1,                                          // ADC Channel to read
            conversion_none                             // Function to convert data
    };

    //number of sensors
    //there should not be more sensors than ADC inputs
    constexpr uint8_t NUMSENSORS = 2;
}

namespace CANCONFIG
{
    constexpr CAN_SETTINGS CAN0 = {
                0xC1,   //id
                0,      //rtr
                0       //ide
    };


    constexpr CANSensorChannel CANChannel0 = {
            CAN0,           // CAN Message ID for this channel
            1,              // milliseconds between updates
            {
                SENSORCONFIG::SensorSPRL,             //sensors in data channel positions
                SENSORCONFIG::SensorSPRR
                //SENSORCONFIG::SensorPushRodRight
            },
            2                               //number of sensors on CANChannel
    };

    // all can channels
    constexpr CANSensorChannel CANChannels[] =
        {
                CANChannel0
        };
    //number of CAN Channels
    constexpr uint8_t NUMCANCHANNELS = 1;

    //Device id for commands over CAN
    constexpr uint8_t CANCMD_DEVID = 0x02;
}


#endif /* CONFIG_CONFIG_FR_H_ */



#endif /* CONFIG_CONFIG_LR_H_ */
