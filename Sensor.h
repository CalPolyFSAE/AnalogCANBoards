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

    struct SENSOR_SETTINGS
    {
        // void pointer function pointer (cant point to a member of an object)
        typedef int16_t (*DataConversion)( const float& voltage );

        //min and max expected values from processed data
        int16_t MinExpectedVal, MaxExpectedVal;

        //ADC Channel to read
        uint8_t ADCChannel;

        //function for performing the data conversions
        DataConversion ConversionFunction;
    };

    //gain error constants
    static constexpr float VREF = 5.0;
    static constexpr float VSCALE = VREF/1023.0;
    static constexpr float VERRORSCALE = -0.014;
    static constexpr float VERROR = 0.067;

    //min and max expected values from processed data
    const int16_t MinExpectedVal, MaxExpectedVal;

    //ADC Channel read
    const uint8_t ADCChannel;

    Sensor();

    Sensor(const SENSOR_SETTINGS& setup);

    virtual ~Sensor();

    //request an ADC read for this sensor
    bool requestADCRead();

    //Called by ADCManager when read is finished
    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel);

    //get corrected value to send over CAN
    int16_t getValue();

    //get corrected voltage
    float getVoltage();

    //is the rawADC value ready
    inline bool getIsReady() const {
        return isReady;
    }

private:

    //raw ADC output from most recent read
    volatile uint16_t rawADC;

    //is the ADC read completed
    volatile bool isReady;

    //Is the min/max functionality being used?
    const bool UseMinMax;

    //conversion function
    SENSOR_SETTINGS::DataConversion conversionFunction;

    //function to figure out value of UseMinMax
    static inline constexpr bool EvalMinMax (const int16_t& min, const int16_t& max)
    {
        return (min >= max)? false : true;
    }
};



#endif /* SENSOR_H_ */
