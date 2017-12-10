/*
 * Sensor.cpp
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "Sensor.h"

#include <stdint.h>
#include <util/atomic.h>

Sensor::Sensor( SensorData setup ) :
        MinExpectedVal (setup.MinExpectedVal),
        MaxExpectedVal (setup.MaxExpectedVal),
        ADCChannel (setup.ADCChannel)
{
    conversionFunction = setup.ConversionFunction;
}

//get corrected value to send over CAN
int16_t Sensor::getValue() {
    int16_t value = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        value = conversionFunction(rawADC);
    }
    if(value > MaxExpectedVal){
        value = MaxExpectedVal;
    }else if(value < MinExpectedVal)
    {
        value = MinExpectedVal;
    }
    return value;
}

//Called by ADCManager when read is finished
//TODO: This seems like a rough method of doing a callback
void Sensor::INT_Call_ADCReadFinished( uint16_t value, uint8_t channel ) {
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        rawADC = value;
    }
}

