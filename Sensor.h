/*
 * Sensor.h
 * Manages values for a sensor
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>
#include "ADCManager.h"

class Sensor : public ADCManagerCallbackInterface
{
public:

    typedef struct SENSOR_SETTINGS
    {
        // void pointer function pointer (cant point to a member of an object)
        typedef int16_t (*DataConversion)( uint16_t );

        //min and max expected values from processed data
        int16_t MinExpectedVal, MaxExpectedVal;

        //ADC Channel to read
        uint8_t ADCChannel;

        //function for performing the data conversions
        DataConversion ConversionFunction;
    } SENSOR_SETTINGS;

    //min and max expected values from processed data
    const int16_t MinExpectedVal, MaxExpectedVal;

    //ADC Channel read
    const uint8_t ADCChannel;

    Sensor(const SENSOR_SETTINGS& setup);

    virtual ~Sensor();

    //request an ADC read for this sensor
    bool requestADCRead();

    //Called by ADCManager when read is finished
    virtual void INT_Call_ADC_Finished(uint16_t value, uint8_t channel);

    //get corrected value to send over CAN
    int16_t getValue();

    //is the rawADC value ready
    inline bool getIsReady(){return isReady;};

private:
    //no default initializer
    Sensor();

    //raw ADC output from most recent read
    volatile uint16_t rawADC;

    //is the ADC read completed
    volatile bool isReady;

    //Is the min/max functionality being used?
    bool useMinMax;

    //conversion function
    SENSOR_SETTINGS::DataConversion conversionFunction;
};



#endif /* SENSOR_H_ */
