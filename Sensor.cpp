/*
 * Sensor.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "Sensor.h"

#include <stdint.h>
#include <util/atomic.h>

Sensor::Sensor(const SENSOR_SETTINGS& setup ) :
        MinExpectedVal (setup.MinExpectedVal),
        MaxExpectedVal (setup.MaxExpectedVal),
        ADCChannel (setup.ADCChannel)
{
    conversionFunction = setup.ConversionFunction;
    isReady = false;

    //make sure that the min/max can be used
    if(MinExpectedVal >= MaxExpectedVal)
        useMinMax = false;
    else
        useMinMax = true;
}

Sensor::~Sensor()
{

}

//request an ADC read for this sensor
bool Sensor::requestADCRead()
{
    if(ADCManager::StartRead(this, ADCChannel))
    {
        isReady = false;
        return true;
    }else
    {
        return false;
    }
}

//Called by ADCManager when read is finished
void Sensor::INT_Call_ADC_Finished( uint16_t value, uint8_t channel ) {
        rawADC = value;
        isReady = true;
}

//get corrected value to send over CAN
int16_t Sensor::getValue() {
    int16_t value = 0;
    //make sure nothing changes rawADC value
    //although this should never be able to happen
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        value = conversionFunction(rawADC);
    }

    //check if min/max functionality should be used
    if (useMinMax)
    {
        if (value > MaxExpectedVal)
        {
            value = MaxExpectedVal;
        }
        else if (value < MinExpectedVal)
        {
            value = MinExpectedVal;
        }
    }

    if(!isReady)
        value = 0;

    return value;
}

