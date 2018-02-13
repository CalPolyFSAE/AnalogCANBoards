/*
 * CANLib.cpp
 *
 *  Created on: Feb 10, 2018
 *      Author: root
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "CANLib.h"


ISR(CANIT_vect)
{

}

void CANRaw::Init(CAN_BAUDRATE baud = CAN_BAUDRATE::B1M)
{
    Can_reset();

    // disable mob specific interrupts
    CANIE2 = 0x00;
    CANIE1 = 0x00;

    // CAN Interrupts:
    // Timer Overrun: Off
    // General Errors: Off
    // Frame Buffer: Off
    // MOb Errors: Off
    // Transmit: On
    // Receive: On
    // Bus Off: Off
    // (ENIT): All, except Timer Overrun: On
    CANGIE = _BV(ENRX) | _BV(ENTX) | _BV(ENIT);

    // Highest Interrupt Priority: MOb0
    CANHPMOB = 0x00;

    switch (baud) {
        case CAN_BAUDRATE::B1M:
            CANBT1 = 0x00;
            CANBT2 = 0x0C;
            CANBT3 = 0x36;
            break;
        case CAN_BAUDRATE::B500K:
            CANBT1 = 0x02;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
        case CAN_BAUDRATE::B250K:
            CANBT1 = 0x06;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
        case CAN_BAUDRATE::B125K:
            CANBT1 = 0x0E;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
    }

    // CAN Timer Clock Period: 1.000 us
    CANTCON = 0x00;

    Can_enable();

    //reset all mob status registers and configurations
    for (int i = 0; i < NB_MOB; ++i)
    {
        Can_set_mob(i)
        Can_clear_mob()
    }
}

void CANRaw::BindListener(const CANListener* listener, uint8_t Mob)
{

}
