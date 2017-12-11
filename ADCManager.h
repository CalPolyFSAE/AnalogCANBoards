/*
 * ADCManager.h
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include <stdint.h>

//this allows ADCManager to have a common interface for callbacks
//implement this whenever you want to use ADCManager
class ADCManagerCallbackInterface
{
public:
    virtual ~ADCManagerCallbackInterface() = 0;
    virtual void INT_Call_ADC_Finished(uint16_t value, uint8_t channel) = 0;
};

class ADCManager
{
public:

    //setup registers for ADC
    static void Init();

    //start an ADC read
    static bool StartRead(ADCManagerCallbackInterface* resultHandler, uint8_t channel);

    static bool ADCAvailable();

    static void INT_ADCFinished();
private:
    //pointer to the current adc read Callback function
    static ADCManagerCallbackInterface* currentReadCallback;
    static uint8_t channel;
};


#endif /* ADCMANAGER_H_ */
