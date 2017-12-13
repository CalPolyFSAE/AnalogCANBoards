/*
 * CONFIG00.cpp
 *
 * Example Configuration File.
 * This is constructed this way to allow for compile time settings
 *
 *  Created on: Nov 14, 2017
 *      Author: root
 */

#include "CONFIG00.h"

int16_t SENSORCONFIG::conversion00(const float& voltage)
{
    return (int16_t)voltage;
}
int16_t SENSORCONFIG::conversion01(const float& voltage)
{
    return (int16_t)voltage;
}


