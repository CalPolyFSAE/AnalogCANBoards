/*
 * CANLib.cpp
 *
 *  Created on: Feb 10, 2018
 *      Author: root
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "CANLib.h"


ISR(CANIT_vect) {
    CANRaw::StaticClass ().INT_CANIT ();
}

void CANRaw::Init( CAN_BAUDRATE baud = CAN_BAUDRATE::B1M ) {
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

    //reset all mob status registers and configurations
    for (int i = 0; i < NB_MOB; ++i)
    {
        Can_set_mob(i)
        Can_clear_mob()
    }

    Can_enable();
}

bool CANRaw::BindListener( const CANListener* listener, CAN_MOB mob,
                           bool forceOverwrite = false ) {
    if (mob > LAST_MOB_NB || (Handlers[mob] != nullptr && !forceOverwrite))
    {
        return false;
    }

    Handlers[mob] = listener;
    MobModes[mob] = CAN_MOB_OPERATING_MODE::DISABLED;

    return true;
}

bool CANRaw::CanTxData( const CAN_DATA& data, CAN_MOB mobn ) {
    if (MobModes[mobn] != CAN_MOB_OPERATING_MODE::Tx_DATA_FRAME || mobn > LAST_MOB_NB)
    {
        return false;
    }
    Can_set_mob(mobn);

    const CAN_FRAME_HEADER& tmp = MobHeaders[mobn];

    if (tmp.ide)
    {
        Can_set_ext_id(tmp.id);
    }
    else
    {
        Can_set_std_id(tmp.id);
    }

    // copy data to send
    for (uint8_t cpt = 0; cpt < tmp.dataLength; cpt++)
        CANMSG = data.byte[cpt];

    Can_clear_rtr();

    Can_set_dlc(tmp.dataLength);
    Can_config_tx();

    return true;
}
