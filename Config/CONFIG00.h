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

//TODO: make the sensors a list on CANChannel, not a value in SensorConfig
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
        CANRXTX::MOB_MODE::TX,  //mode
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
        CANRXTX::MOB_MODE::TX,  //mode
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
    //number of CAN Channels
    constexpr uint8_t NUMCANCHANNELS = 2;

    //Device id for commands over CAN
    constexpr uint8_t CANCMDDEVID = 0x00;
}


namespace SENSORCONFIG
{

    int16_t conversion00(uint16_t);
    int16_t conversion01(uint16_t);

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
            5,                                          // Min expected value (converted)
            16,                                         // Max expected value (converted)

            1,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    // Sensor 2 Name
    constexpr Sensor::SENSOR_SETTINGS Sensor02 = {
            5,                                          // Min expected value (converted)
            16,                                         // Max expected value (converted)

            3,                                          // ADC Channel to read
            conversion00                                // Function to convert data
    };

    //number of sensors
    //there should not be more sensors than ADC inputs
    constexpr uint8_t NUMSENSORS = 3;

    //SensorChannel
    //this ties sensors to CAN Channels
    //one sensor should not be on two different CAN Channels

    /*
     * typedef struct SensorChannel
     * {
     * Sensor::SENSOR_SETTINGS Sensor;                 // Sensor
     * CANCONFIG::CANChannel CANChannel;               // CAN Channel to use for this sensor
     * CANSensorTimer::CANDATAChannel CANDATAChannel;  // CAN Data Channel to use for this sensor
     * };
     */
    constexpr SensorChannel SensorChannel00 =
    {
        Sensor00,
        CANCONFIG::CANChannel0,
        CANSensorTimer::CANDATAChannel::CANCHANNEL0
    };

    constexpr SensorChannel SensorChannel01 =
    {
        Sensor01,
        CANCONFIG::CANChannel0,
        CANSensorTimer::CANDATAChannel::CANCHANNEL1
    };

    constexpr SensorChannel SensorChannel02 =
    {
        Sensor02,
        CANCONFIG::CANChannel1,
        CANSensorTimer::CANDATAChannel::CANCHANNEL0
    };

    constexpr uint8_t NUMSENSORCHANNELS = 3;

    constexpr SensorChannel SensorChannels[] = {
            SensorChannel00,
            SensorChannel01,
            SensorChannel02
    };
}


#endif /* CONFIG00_H_ */
