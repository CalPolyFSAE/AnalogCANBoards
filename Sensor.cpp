/*
 * Sensor.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "Sensor.h"

#include <stdint.h>
#include <util/atomic.h>

Sensor::Sensor() :
        MinExpectedVal (0),
        MaxExpectedVal (0),
        ADCChannel (0),
        //make sure that useMinMax can be used
        UseMinMax(false)
{
    conversionFunction = nullptr;
    isReady = false;
}


Sensor::Sensor(SENSOR_SETTINGS const& setup ) :
        MinExpectedVal (setup.MinExpectedVal),
        MaxExpectedVal (setup.MaxExpectedVal),
        ADCChannel (setup.ADCChannel),
        //make sure that useMinMax can be used
        UseMinMax(EvalMinMax(setup.MinExpectedVal, setup.MaxExpectedVal))
{
    conversionFunction = setup.ConversionFunction;
    isReady = false;
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
void Sensor::INT_Call_ADC_Finished( uint16_t const& value, uint8_t channel ) {
        rawADC = value;
        isReady = true;
}

//get corrected value to send over CAN
int16_t Sensor::getValue() {
    int16_t value = 0;

    float voltage;
    //make sure nothing changes rawADC value
    //although this should never be able to happen
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        getVoltage(voltage);
    }
    value = conversionFunction(voltage);

    //check if min/max functionality should be used
    if (UseMinMax)
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

    return value;
}

void Sensor::getVoltage(float& out)
{
    //gain error calculation for ADC
    out = rawADC * VSCALE;
    out = out - (VERRORSCALE * out + VERROR);
    if(out < 0)
        out = 0;
}

