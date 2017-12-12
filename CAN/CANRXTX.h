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

    //number of available MObs
    //TODO: move to a config file eventually?
    static constexpr uint8_t MAXMOBS = 15;

    //max number of bytes per CAN
    //
    static constexpr uint8_t MAXCANDLC = 8;

    //CAN Bus Baudrate
    enum class BAUDRATE
        : uint8_t
        {
            B1M, B500K, B250K, B125K
    };

    //MOB mode
    //Framebuffer and automatic reply not currently supported
    enum class MOB_MODE
        : uint8_t
        {
            DISABLED, TX, RX, AUTOMATIC_REPLY, FRAME_BUFFER_RX
    };

    //Settings for type of message to be used with MOB
    //Can also be used for the MOB mask
    typedef struct MOB_SETTINGS
    {
        //MOB mode of operation
        MOB_MODE mode;

        //CAN Message settings
        uint32_t id;            //CANID
        uint8_t dlc;            //data Length code
        uint8_t rtr;            //return request
        uint8_t ide;            //identifier extension

        //Masks. bit 1 = filter, bit 0 = ignore
        uint32_t idMask;        //CANID Mask
        uint8_t dlcMask;        //
        uint8_t rtrMask;        //
        uint8_t ideMask;        //
    } MOB_SETTINGS;

    //structure to hold CAN data
    typedef struct CAN_DATA
    {
        uint8_t dlc;
        uint8_t data[MAXCANDLC];
    } CAN_DATA;


    //get next disabled MOB number
    static uint8_t GetNextDisabledMOB();

    //setup data in MOBSettings array
    //need to call InitWithCurrentSettings for changes to
    //take effect
    ///Error Codes:
    // 1: Invalid MOb number
    // 2: MOB is already configured
    // 3: invalid MOB settings
    static uint8_t ConfigureMOB(uint8_t mobNum, const MOB_SETTINGS& settings, bool overwrite);

    //set the CAN Bus baudrate
    //this setting only takes effect after InitWithCurrentSettings is called
    static void SetBaudrate(BAUDRATE baud);

    //this function pushes all settings to hardware and enables CAN controller
    //call it only after everything has been set up
    //or to update hardware
    static void InitWithCurrentSettings();

    //send a message using MOB number mobn
    //dlc is used to verify that message is same
    //length as the one configured
    static void TX_UsingMOB(uint8_t mobn, void* data, uint8_t dlc);

private:
    CANRXTX();

    //selected Baudrate
    static BAUDRATE baud;

    //MOB settings
    static MOB_SETTINGS MOBSettings[MAXMOBS];

    //set active MOB
    static void setActiveMOB(uint8_t n);

    //set the ID of the active MOB
    static void setID(uint32_t id, bool ide);

    //TODO: implement function
    //set the RX ID mask
    static void setIDMask(uint32_t id, bool ide);
};

#endif /* CAN_CANRXTX_H_ */
