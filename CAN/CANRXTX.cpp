/*
 * CANRXTX.cpp
 *
 *  Created on: Dec 10, 2017
 *      Author: root
 */
#include <avr/io.h>

#include "CANRXTX.h"

CANRXTX::BAUDRATE CANRXTX::baud = CANRXTX::BAUDRATE::B125K;
CANRXTX::MOB_SETTINGS CANRXTX::MOBSettings[CANRXTX::MAXMOBS] = {};


 //get next disabled MOB number
uint8_t CANRXTX::GetNextDisabledMOB()
{
    for(uint8_t i = 0; i < MAXMOBS; ++i)
    {
        if(MOBSettings[i].mode == MOB_MODE::DISABLED)
        {
            return i;
        }
    }
    return MAXMOBS;
}

//setup data in MOBSettings array
//will return false if MOB has already been configured
//need to call InitWithCurrentSettings for changes to
//take effect
uint8_t CANRXTX::ConfigureMOB(uint8_t mobNum, const MOB_SETTINGS& settings, bool forceOverwrite)
{
    if(mobNum >= MAXMOBS)
        return 1;

    //make sure MOB is not already configured if you dont want to overwrite it
    if (MOBSettings[mobNum].mode != MOB_MODE::DISABLED && !forceOverwrite)
        return 2;

    //make sure the CANID is correct for identifier extension
    if(settings.dlc > 8 || (settings.id > 0b11111111111 && !(settings.ide)))
        return 3;

    MOBSettings[mobNum] = settings;
    return 0;
}

//set the CAN Bus baudrate
//this setting only takes effect after InitWithCurrentSettings is called
void CANRXTX::SetBaudrate( BAUDRATE baud )
{
    CANRXTX::baud = baud;
}

//this function pushes all settings to hardware and enables CAN controller
//call it only after everything has been set up
//or to update hardware
void CANRXTX::InitWithCurrentSettings()
{
    //reset CAN controller
    CANGCON = (1 << SWRES);

    // MOb0..7 Interrupts: MOb0: Off, MOb1: Off, MOb2: Off, MOb3: Off, MOb4: Off, MOb5: Off, MOb6: Off, MOb7: Off
    CANIE2 = 0x00;
    // MOb8..14 Interrupts: MOb8: Off, MOb9: Off, MOb10: Off, MOb11: Off, MOb12: Off, MOb13: Off, MOb14: Off
    CANIE1 = 0x00;

    // CAN Interrupts:
    // Timer Overrun: Off
    // General Errors: Off
    // Frame Buffer: Off
    // MOb Errors: Off
    // Transmit: Off
    // Receive: On
    // Bus Off: Off
    // All, except Timer Overrun: Off
    CANGIE = (1 << ENRX) | (1 << ENIT);

    // Highest Interrupt Priority: MOb0
    CANHPMOB = 0x00;

    //setup MOBs
    for (uint8_t i = 0; i < MAXMOBS; ++i)
    {
        MOB_SETTINGS *curMOB = &MOBSettings[i];
        setActiveMOB(i);

        CANSTMOB = 0x00;                                //reset status registers for MOB

        if (curMOB->mode == MOB_MODE::TX)
        {
            //set the CAN id of MOB
            setID (curMOB->id, curMOB->ide);
            CANCDMOB = (curMOB->dlc & 0x0F) << DLC0         //set MOB dlc
            | (curMOB->ide && 1) << IDE             //set MOB ide
            | (curMOB->rtr && 1) << RPLV;           //set MOB rtr
        }
        else if (curMOB->mode == MOB_MODE::RX)
        {

        }

    }

    //configure bus speed
    switch (baud) {
        case BAUDRATE::B1M:
            CANBT1 = 0x00;
            CANBT2 = 0x0C;
            CANBT3 = 0x36;
            break;
        case BAUDRATE::B500K:
            CANBT1 = 0x02;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
        case BAUDRATE::B250K:
            CANBT1 = 0x06;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
        case BAUDRATE::B125K:
            CANBT1 = 0x0E;
            CANBT2 = 0x0C;
            CANBT3 = 0x37;
            break;
    }

    //CAN timer control
    //1.00 us timing for 0 to 255 counter
    CANTCON = 0x00;

    //CAN General Control Register
    //enable CAN controller
    CANGCON |= (1 << ENASTB);
}

/////////////////////////////////////////////////////////////////

//set active MOB
void CANRXTX::setActiveMOB(uint8_t n)
{
    CANPAGE = CANPAGE | (n << 4);
}

//set the ID of the active MOB
void CANRXTX::setID(uint32_t id, bool ide)
{
    //TODO: make sure this is the easiest way of doing this
    if (ide)
    {
        CANIDT1 = (id >> 21) & 0xFF;
        CANIDT2 = (id >> 13) & 0xFF;
        CANIDT3 = (id >> 5) & 0xFF;
        CANIDT4 = (id << 3) & 0xF8;
    }
    else
    {
        CANIDT1 = (id >> 3) & 0xFF;
        CANIDT2 = (id << 5) & 0xFF;
        CANIDT3 = 0x00;
        CANIDT4 = 0x00;
    }
}

//send a message using MOB number mobn
//dlc is used to verify that message is same
//length as the one configured
void CANRXTX::TX_UsingMOB( uint8_t mobn, void* data, uint8_t dlc )
{
    if(mobn >= MAXMOBS)
    {
        //TODO: error something bad happened
        return;
    }

    if(dlc != MOBSettings[mobn].dlc)
    {
        //TODO: warn, wrong message size sending anyways
    }

    CANSTMOB = 0x00;//reset status registers

    for (uint8_t i = 0; i < MOBSettings[mobn].dlc; i++)
    {
        CANMSG = ((uint8_t*)data)[i];
    }

    CANCDMOB |= (1 << CONMOB0);//enable MOB
}

