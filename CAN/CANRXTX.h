/*
 * CANRXTX.h
 *
 *  Created on: Dec 9, 2017
 *      Author: root
 */

#ifndef CAN_CANRXTX_H_
#define CAN_CANRXTX_H_

#include <stdint.h>

class CANRXTX
{
public:
    typedef union CAN_MSG_ID
    {
        uint32_t extended;      //29 bit CAN ID
        uint16_t standard;      //11 bit CAN ID
    } CAN_MSG_ID;

};

#endif /* CAN_CANRXTX_H_ */
