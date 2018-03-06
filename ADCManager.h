/*
 * ADCManager.h
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

//TODO: This is a bit of a mess, and the ADCManager is not a guaranteed singleton (across different types)

#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>


//this allows ADCManager to have a common interface for callbacks
//implement INT_Call_ADC_Finished_Implementation whenever you want to use ADCManager
//CRTP callback for fast
template<class T>
class ADCManagerCallbackInterface
{
public:
    void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel)
    {
        //calling INT_Call_ADC_Finished_Implementation(const uint16_t& value, uint8_t channel);
        //resolve the function at compile time
        static_cast<T*>(this)->INT_Call_ADC_Finished_Implementation(value, channel);
    }
protected:
    ADCManagerCallbackInterface() {}
};

// S needs to be inherit from ADCManagerCallbackInterface
template<class S>
class ADCManager
{
public:

    static ADCManager<S>& StaticClass();

    //setup registers for ADC
    void Init();

    //start an ADC read
    bool StartRead(ADCManagerCallbackInterface<S>* resultHandler, uint8_t channel);

    //if the ADC is available for a read request
    bool ADCAvailable();

    //do not call. used to handle the ADC read result
    void INT_ADCFinished();

private:
    static ADCManager<S>* staticObj;
    //pointer to the current adc read Callback function
    static ADCManagerCallbackInterface<S>* currentReadCallback;
    //channel of the current read
    static uint8_t channel;
};



/////////Template implementations//////////
// these have to be in the header

template<class S>
ADCManagerCallbackInterface<S>* ADCManager<S>::currentReadCallback = nullptr;

template<class S>
uint8_t ADCManager<S>::channel = 0;

template<class S>
ADCManager<S>* ADCManager<S>::staticObj = nullptr;

template<class S>
ADCManager<S>& ADCManager<S>::StaticClass()
{
    // workaround for static ADCManager<S> ADC {}
    if(!staticObj)
        staticObj = new ADCManager<S>();
    return *staticObj;
}

template<class S>
void ADCManager<S>::Init() {
    ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz
    ADMUX = (1 << REFS0);
    // Set ADC reference to AVCC
    ADCSRA |= (1 << ADEN) | (1 << ADIE); // Enable ADC, ADC Interrupt Enable
}

template<class S>
bool ADCManager<S>::StartRead(ADCManagerCallbackInterface<S>* resultHandler, uint8_t channel)
{
    if (ADCAvailable())
    {
        currentReadCallback = resultHandler;
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

template<class S>
bool ADCManager<S>::ADCAvailable()
{
    return !(bool)(currentReadCallback);
}

template<class S>
void ADCManager<S>::INT_ADCFinished()
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

        currentReadCallback->INT_Call_ADC_Finished(result, channel);
        currentReadCallback = nullptr;
        channel = 0;
    }

}

#endif /* ADCMANAGER_H_ */
