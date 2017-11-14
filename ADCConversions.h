/*
 * ADCConversions.h
 *
 *  Created on: Nov 6, 2017
 *      Author: root
 */

#ifndef ADCCONVERSIONS_H_
#define ADCCONVERSIONS_H_

#include <stdint.h>

class ADCConversions
{
public:
    uint16_t adctoval(const uint16_t & adc_val, uint8_t channel) const;
private:
    ADCConversions();
};


#endif /* ADCCONVERSIONS_H_ */
