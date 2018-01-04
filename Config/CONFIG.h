/*
 * CONFIG.h
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#define CONF00

#define MAXSENSORS 8

#define SERIAL_BAUD 9600
#define CAN_BAUD 1000

#include "CONFIG_Commands.h"

#ifdef CONF00
#include "CONFIG00.h"
#endif


#endif /* CONFIG_CONFIG_H_ */
