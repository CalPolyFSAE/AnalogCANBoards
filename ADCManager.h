/*
 * ADCManager.h
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef ADCMANAGER_H_
#define ADCMANAGER_H_

#include <stdint.h>

class ADCManager
{
public:
    typedef void (*INT_Call_ADC_Finished)(uint16_t value, uint8_t channel);

    //setup registers for ADC
    static void Init();

    //start an ADC read
    static bool StartRead(INT_Call_ADC_Finished func, uint8_t channel);

    static bool ADCAvailable();

    static void INT_ADCFinished();
private:
    //pointer to the current adc read Callback function
    static INT_Call_ADC_Finished currentReadCallback;
    static uint8_t channel;
};


#endif /* ADCMANAGER_H_ */
