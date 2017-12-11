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

    //number of available MObs
    constexpr uint8_t MAXMOBS = 15;



    //setup data in MOBSettings array
    //will return false if MOB has already been configured
    //need to call InitWithCurrentSettings for changes to
    //take effect
    static bool ConfigureMOB(uint8_t mobNum, MOB_SETTINGS& settings);

    //this function pushes all settings to hardware and enables CAN controller
    //call it only after everything has been set up
    //or to update hardware
    static void InitWithCurrentSettings();

private:
    CANRXTX();

    //MOB settings
    static MOB_SETTINGS MOBSettings[MAXMOBS];
};

#endif /* CAN_CANRXTX_H_ */
