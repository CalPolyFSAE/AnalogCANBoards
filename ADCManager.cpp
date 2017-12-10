/*
 * ADCManager.cpp
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#include "ADCManager.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

ADCManager::INT_Call_ADC_Finished ADCManager::currentReadCallback = nullptr;
uint8_t ADCManager::channel = 0;

void ADCManager::Init() {
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
    ADMUX = (1 << REFS0);
    // Set ADC reference to AVCC
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC, ADC Interrupt Enable
}

bool ADCManager::StartRead(INT_Call_ADC_Finished func, uint8_t channel)
{
    if (ADCAvailable())
    {
        currentReadCallback = func;
        ADCManager::channel = channel;

        ADMUX = (1 << REFS0) | (channel & 0xF); // Set ADC reference to AVCC and channel
        ADCSRA |= (1 << ADSC);  // Start A2D Conversions

        return true;
    }
    else
    {
        return false;
    }
}

bool ADCManager::ADCAvailable()
{
    return !(bool)(currentReadCallback);
}

void ADCManager::INT_ADCFinished()
{
    if(!ADCAvailable())
    {
        uint16_t result;
        //this should be unnecessary
        ATOMIC_BLOCK(ATOMIC_FORCEON)
        {
            result = ADCL;
            result |= ADCH << 8;
        }

        currentReadCallback (result, channel);
        currentReadCallback = nullptr;
        channel = 0;
    }

}


