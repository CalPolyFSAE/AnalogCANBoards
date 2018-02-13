/*
 * CANLib.h
 *
 *  Created on: Feb 10, 2018
 *      Author: root
 */

#ifndef CANLIB_H_
#define CANLIB_H_

#include "stdint.h"

#define NB_MOB       15
#define NB_DATA_MAX  8

#define Can_set_mob(mob)       { CANPAGE = ((mob) << 4);}
#define Can_set_mask_mob()     {  CANIDM4=0xFF; CANIDM3=0xFF; CANIDM2=0xFF; CANIDM1=0xFF; }
#define Can_clear_mask_mob()   {  CANIDM4=0x00; CANIDM3=0x00; CANIDM2=0x00; CANIDM1=0x00; }
#define Can_clear_status_mob() { CANSTMOB=0x00; }
#define Can_clear_mob()        { unsigned char  volatile *__i_; for (__i_=&CANSTMOB; __i_<&CANSTML; __i_++) { *__i_=0x00 ;}}

//listener interface for CAN rx callbacks
class CANListener
{
public:
    virtual ~CANListener();
    virtual void INT_Call_GotFrame(const struct CAN_FRAME* frame) = 0;

};

// the initial handler of CAN interrupts
class CANRaw
{
private:
    CANRaw();

    //array of all the CAN rx data handlers
    const CANListener* handlers[NB_MOB];

public:

    enum class CAN_BAUDRATE : uint8_t
    {
        B1M,
        B500K,
        B250K,
        B125K
    };

    union CAN_DATA
    {
        uint64_t value;
        uint8_t byte[8];
    };

    struct CAN_FRAME
    {
        uint32_t id;
        uint8_t rtr;
        uint8_t extendedId;
        uint8_t dataLength;
        CAN_DATA data;
    };

    static inline CANRaw& StaticClass()
    {
        static CANRaw CAN {};
        return CAN;
    }

    // reinitialize the can hardware
    void Init(CAN_BAUDRATE baud);

    // binds a CANListener to a mob
    void BindListener(const CANListener* listener, uint8_t Mob);

    inline void INT_CANIT()
    {

    }
};


#endif /* CANLIB_H_ */
