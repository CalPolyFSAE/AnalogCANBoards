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
#include "Config/CONFIG.h"
#include "ADCManager.h"

class Sensor
{
public:
    //min and max expected values from processed data
    const int16_t MinExpectedVal, MaxExpectedVal;

    //ADC Channel read
    const uint8_t ADCChannel;

    Sensor(SensorData setup);

    //get corrected value to send over CAN
    int16_t getValue();

    //Called by ADCManager when read is finished
    void INT_Call_ADCReadFinished(uint16_t value, uint8_t channel);

private:
    Sensor();

    //raw ADC output from most recent read
    volatile uint16_t rawADC;

    //conversion function
    SensorData::DataConversion conversionFunction;
};



#endif /* SENSOR_H_ */
