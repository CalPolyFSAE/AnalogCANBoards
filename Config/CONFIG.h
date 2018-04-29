/*
 * CONFIG.h
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#define MAXSENSORS 8

#define SERIAL_BAUD 115200
#define CAN_BAUD 1000

#include "CONFIG_Commands.h"


#define FR 1
#define LR 2
#define NONE 0

#define CONFIG_ FR

#if CONFIG_==1
#include "CONFIG_FR.h"
#elif CONFIG_==2
#include "CONFIG_LR.h"
#elif CONFIG_==0
#include "CONFIG00.h"
#endif


#endif /* CONFIG_CONFIG_H_ */
