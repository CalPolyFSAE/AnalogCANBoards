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

bool CANRaw::BindListener( CANListener* listener, CAN_MOB mobn,
                           bool forceOverwrite) {
    uint8_t mob = (uint8_t)mobn;
    if (mob > LAST_MOB_NB || (Handlers[mob] != nullptr && !forceOverwrite))
    {
        return false;
    }

    Handlers[mob] = listener;

    return true;
}

bool CANRaw::TxData( const CAN_DATA& data, CAN_MOB mobn ) {
    uint8_t mob = (uint8_t)mobn;
    if (MobModes[mob] != CAN_MOB_OPERATING_MODE::Tx_DATA_FRAME || mob > LAST_MOB_NB)
    {
        return false;
    }

    // copy data to send
    for (uint8_t cpt = 0; cpt < MobHeaders[mob].dataLength; cpt++)
        CANMSG = data.byte[cpt];

    Can_config_tx();

    return true;
}


bool CANRaw::ConfigRx( const CAN_FRAME_HEADER& header, const CAN_FRAME_MASK& mask,
                  CAN_MOB mobn )
{
    uint8_t mob = (uint8_t)mobn;
    if(MobModes[mob] != CAN_MOB_OPERATING_MODE::DISABLED || Handlers[mob] == nullptr)
    {
        return false;
    }

    Can_set_mob(mob);

    MobModes[mob] = CAN_MOB_OPERATING_MODE::Rx_DATA_FRAME;
    MobHeaders[mob] = header;
    MobMasks[mob] = mask;

    ReconfigureMob(mobn);

    Can_config_rx();

    return true;
}

bool CANRaw::ConfigTx( const CAN_FRAME_HEADER& header, CAN_MOB mobn )
{
    uint8_t mob = (uint8_t) mobn;
    if (MobModes[mob] != CAN_MOB_OPERATING_MODE::DISABLED || Handlers[mob] == nullptr)
    {
        return false;
    }

    Can_set_mob(mob);

    MobModes[mob] = CAN_MOB_OPERATING_MODE::Tx_DATA_FRAME;
    MobHeaders[mob] = header;

    ReconfigureMob (mobn);

    return true;
}

void CANRaw::ForceResetMob(CAN_MOB mobn)
{
    uint8_t mob = (uint8_t) mobn;
    Can_set_mob(mob);

    // clear all mob registers
    Can_full_abort();// in case anything is in progress
    Can_clear_mob();// clear all Can mob specific registers

    MobModes[mob] = CAN_MOB_OPERATING_MODE::DISABLED;
}

void CANRaw::INT_CANIT() {
    uint8_t origCANPAGE = CANPAGE;
    while (CANSIT2 | CANSIT1)
    {

        uint8_t mob;

        if (CANSIT2 & _BV (0)) mob = (uint8_t)CAN_MOB::MOB_0;
        else if (CANSIT2 & _BV (1)) mob = (uint8_t)CAN_MOB::MOB_1;
        else if (CANSIT2 & _BV (2)) mob = (uint8_t)CAN_MOB::MOB_2;
        else if (CANSIT2 & _BV (3)) mob = (uint8_t)CAN_MOB::MOB_3;
        else if (CANSIT2 & _BV (4)) mob = (uint8_t)CAN_MOB::MOB_4;
        else if (CANSIT2 & _BV (5)) mob = (uint8_t)CAN_MOB::MOB_5;
        else if (CANSIT2 & _BV (6)) mob = (uint8_t)CAN_MOB::MOB_6;
        else if (CANSIT2 & _BV (7)) mob = (uint8_t)CAN_MOB::MOB_7;
        else if (CANSIT1 & _BV (0)) mob = (uint8_t)CAN_MOB::MOB_8;
        else if (CANSIT1 & _BV (1)) mob = (uint8_t)CAN_MOB::MOB_9;
        else if (CANSIT1 & _BV (2)) mob = (uint8_t)CAN_MOB::MOB_10;
        else if (CANSIT1 & _BV (3)) mob = (uint8_t)CAN_MOB::MOB_11;
        else if (CANSIT1 & _BV (4)) mob = (uint8_t)CAN_MOB::MOB_12;
        else if (CANSIT1 & _BV (5)) mob = (uint8_t)CAN_MOB::MOB_13;
        else if (CANSIT1 & _BV (6)) mob = (uint8_t)CAN_MOB::MOB_14;

        Can_set_mob(mob)
        uint8_t canstmob_copy = CANSTMOB;

        //first check for successful sending
        uint8_t mobStatus = canstmob_copy
                & ((_BV (RXOK) | _BV (TXOK) | _BV (DLCW)));

        if (mobStatus)
        {
            CANListener& handler = *Handlers[mob];
            if (mobStatus & MOB_TX_COMPLETED)
            {
                Can_clear_status_mob();
                // notify that message was sent (Tx callback)
                handler.INT_Call_SentFrame(MobHeaders[mob]);
            }
            else if (mobStatus & MOB_RX_COMPLETED)
            {
                Can_clear_status_mob();

                // TODO: dlcw warning
                uint8_t dlc = Can_get_dlc();// in case of dlcw and dlc changing
                for (uint8_t data_index = 0; data_index < dlc;
                        data_index++)
                {
                    handler.Data.byte[data_index] = CANMSG;
                }

                //set the Frame header data
                // TODO: copy the data out of mob config registers
                // as it will be different when using masks for Rx
                handler.FrameData = &MobHeaders[mob];

                // reset Mob to correct configuration as it will change when using masks
                ReconfigureMob(CAN_MOB(mob));


                // the dlc in received data might be different than what was specified when configured
                // notify that message was received (Rx callback)
                handler.INT_Call_GotFrame(dlc);
            }
        }
        else
        {
            //interrupt triggered from an error
            //TODO: error events, and enable error interrupts
            /*
             * // If MOb is ENABLE & NOT_COMPLETED, test if MOb is in ERROR
             // - MOb Status bit_0 = MOB_ACK_ERROR
             // - MOb Status bit_1 = MOB_FORM_ERROR
             // - MOb Status bit_2 = MOB_CRC_ERROR
             // - MOb Status bit_3 = MOB_STUFF_ERROR
             // - MOb Status bit_4 = MOB_BIT_ERROR
             mob_status = canstmob_copy & ERR_MOB_MSK; //error mask
             if (mob_status != 0) { return(mob_status); }
             * */
        }



        /*
         // SLOWER OPTION
         uint16_t CANSIT = CANSIT2 | (CANSIT1 << 8);
         for(uint8_t mob; mob < NB_MOB; ++mob)
         {
         if(CANSIT & _BV(mob))
         {
         Can_set_mob(mob);
         if(CANSTMOB & MOB_RX_COMPLETED)
         {
         Can_clear_status_mob();
         //handlers[mob]->INT_Call_GotFrame(...);
         } else if (CANSTMOB & MOB_TX_COMPLETED)
         {
         Can_clear_status_mob();
         //handlers
         }

         }
         }*/
    }
    CANPAGE = origCANPAGE;
}



void CANRaw::ReconfigureMob(CAN_MOB mobn)
{
    uint8_t origCANPAGE = CANPAGE;

    uint8_t mob = (uint8_t)mobn;

    Can_set_mob(mob);

    // make sure nothing is happening on MOB
    Can_mob_abort();
    Can_clear_mob();

    const CAN_FRAME_HEADER& tmp = MobHeaders[mob];

    //setup masks for rx
    CAN_FRAME_MASK& msk = MobMasks[mob];

    if (tmp.ide)
    {
        Can_set_ext_id(tmp.id);
    }
    else
    {
        Can_set_std_id(tmp.id);
    }

    if (tmp.rtr)
    {
        Can_set_rtr();
    }
    else
    {
        Can_clear_rtr();
    }

    Can_set_dlc(tmp.dataLength);

    // check mode so mode specific things can be done (like resetting masks)
    switch(MobModes[mob])
    {
        case CAN_MOB_OPERATING_MODE::DISABLED:
            return;
            break;
        case CAN_MOB_OPERATING_MODE::Tx_DATA_FRAME:

            break;
        case CAN_MOB_OPERATING_MODE::Rx_DATA_FRAME:
            if(msk.extendedIdMask) //check if the ide mask is being used
            {
                Can_set_idemsk();
            }else
            {
                Can_clear_idemsk();
            }

            if(msk.rtrMask)
            {
                Can_set_rtrmsk();
            }else
            {
                Can_clear_rtrmsk();
            }

            if(tmp.ide) // is the extended id in use
            {
                Can_set_ext_msk(msk.idMask);
            }else
            {
                Can_set_std_msk(msk.idMask);
            }

            break;
        default:
            return;
    }

    CANPAGE = origCANPAGE;
}
