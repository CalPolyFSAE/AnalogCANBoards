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
#include "../CAN/CANRXTX.h"

namespace CANCONFIG
{
    /*
     *  CANRXTX::MOB_SETTINGS
     *  //MOB mode of operation
        MOB_MODE mode;

        //CAN Message settings
        uint32_t id;            //CANID
        uint8_t dlc;            //data Length code
        uint8_t rtr;            //return request
        uint8_t ide;            //identifier extension

        //Masks. bit 1 = filter, bit 0 = ignore
        uint32_t idMask;        //CANID Mask
        uint8_t dlcMask;        //
        uint8_t rtrMask;        //
        uint8_t ideMask;        //
     */
    constexpr CANRXTX::MOB_SETTINGS CAN0
    {
        CANRXTX::MOB_MODE::RX,  //mode
        0x01,                   //CANID
        0,                      //dlc determined at runtime
        0,                      //no rtr flag
        0,                      //no identifier extension

        0,                      //no filtering for CAN RX
        0,                      //
        0,                      //
        0                       //
    };

    constexpr CANRXTX::MOB_SETTINGS CAN2
    {
        CANRXTX::MOB_MODE::RX,  //mode
        0x02,                   //CANID
        0,                      //dlc determined at runtime
        0,                      //no rtr flag
        0,                      //no identifier extension

        0,                      //no filtering for CAN RX
        0,                      //
        0,                      //
        0                       //
    };

    constexpr CANChannel CANChannel0 = {
                CAN0,           // CAN Message ID for this channel
                50              // milliseconds between updates
            };

    constexpr CANChannel CANChannel1 = {
                CAN2,           // CAN Message ID for this channel
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
