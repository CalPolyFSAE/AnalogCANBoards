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
class ADCManagerCallbackInterface
{
public:
    virtual ~ADCManagerCallbackInterface() {}

    virtual void INT_Call_ADC_Finished(const uint16_t& value, uint8_t channel) = 0;
protected:
    ADCManagerCallbackInterface() {}
};

//
class ADCManager
{
public:

    inline static ADCManager& StaticClass()
    {
        static ADCManager A {};
        return A;
    }

    //setup registers for ADC
    void Init();

    //start an ADC read
    bool StartRead(ADCManagerCallbackInterface* resultHandler, uint8_t channel);

    //if the ADC is available for a read request
    bool ADCAvailable();

    //do not call. used to handle the ADC read result
    void INT_ADCFinished();

private:
    ADCManager() {}
    //pointer to the current adc read Callback function
    static ADCManagerCallbackInterface* currentReadCallback;
    //channel of the current read
    static uint8_t channel;
};

#endif /* ADCMANAGER_H_ */
