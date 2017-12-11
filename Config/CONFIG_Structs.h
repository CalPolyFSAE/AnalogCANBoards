/*
 * CONFIG_Structs.h
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_STRUCTS_H_
#define CONFIG_CONFIG_STRUCTS_H_


#include <stdint.h>

/*
 * Usage of CANDATAChannel
 *
 * CAN Message:
 * 8 bytes of data
 * can store 4 16 bit values
 * Every 16 bit value is a channel
 *
 * CAN Message:
 * 16 bits: CAN Data Channel 0
 * 16 bits: CAN Data Channel 1
 * 16 bits: CAN Data Channel 2
 * 16 bits: CAN Data Channel 3
 * */
namespace CANCONFIG
{

    enum class CANDATAChannel
        : uint8_t
        {
            CANCHANNEL0 = 0, CANCHANNEL1, CANCHANNEL2, CANCHANNEL3
    };

    constexpr uint8_t CANMAXDATACHANNELS = 4;

    typedef struct CANChannel
    {
        //ID used for this CAN Message
        const uint16_t CANID;
        //milliseconds between Message sending
        const uint16_t TimingInterval;
    } CANChannel;

}

namespace SENSORCONFIG
{
    typedef struct SensorData
    {
        typedef int16_t (*DataConversion)( uint16_t );

        //min and max expected values from processed data
        const int16_t MinExpectedVal, MaxExpectedVal;

        //CAN ID used for this data
        const CANCONFIG::CANChannel CANChan;
        //slot in CAN Message used for this data
        const CANCONFIG::CANDATAChannel CANDataChannel;

        //ADC Channel to read
        const uint8_t ADCChannel;

        //function for performing the data conversions
        const DataConversion ConversionFunction;
    } SensorData;
}


#endif /* CONFIG_CONFIG_STRUCTS_H_ */
