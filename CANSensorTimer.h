/*
* CANSensorTimer.h
*
*  Created on: Dec 10, 2017
*      Author: root
*/

#ifndef CANSENSORTIMER_H_
#define CANSENSORTIMER_H_

#include <stdint.h>
#include <util/atomic.h>
#include "CANLib.h"

#include "Commands/CommandManager.h"

//timing objects for tracking when a message needs to be sent and what
//is sent with it and fetching the data to be sent
class CANSensorTimer: public CANListener
{
public:

    //bytes per data channel
    static constexpr uint8_t CANBYTESPERDATACHANNEL = 2;
    //maximum number of Data Channels
    static constexpr uint8_t CANMAXDATACHANNELS = NB_DATA_MAX / CANBYTESPERDATACHANNEL;

    /*
     * Usage of CANDATAChannel for 2 bytes per channel
     *
     * CAN Message:
     * 8 bytes of data
     * can store 4 16 bit values
     * Every 16 bit value is a channel
     *
     * CAN Message:
     * 16 bits: CAN Data Channel 0
     * 16 bits: CAN Data Channel 1
     * 16 bits: CAN Data Channel 2
     * 16 bits: CAN Data Channel 3
     * */
    enum class CANDATAChannel
        : uint8_t
        {
            CANCHANNEL0 = 0,
            CANCHANNEL1 = 1,
            CANCHANNEL2 = 2,
            CANCHANNEL3 = 3
    };

    const uint16_t TimingInterval;              // milliseconds between messages

    const uint32_t id;                          //CAN ID for this info
    const uint8_t ide;                          //request return message flag and identifier extension
    

    CANSensorTimer(uint16_t interval, const uint32_t* can_id, uint8_t can_ide);
    virtual ~CANSensorTimer() {};

    //registers a sensor on this CANID
    bool registerSensor(class Sensor* sensor, CANDATAChannel dataChannel);

    //1000Hz interrupt to keep track of timing
    void INT_Call_Tick();

    // CANListener interface
    virtual void INT_Call_SentFrame(const CANRaw::CAN_FRAME_HEADER& frameConfig) override;


    // sets up CANRaw to accept tx data and callback on send
    void Init();

    //make sensors request data if a CAN message needs to be sent
    //then send data over CAN
    void Update();

    //milliseconds until CAN message needs to be sent
    uint16_t timeUntilSend();

    //number of sensors on this timer
    uint8_t getNumActiveSensors();

private:

    /* CAN Tx Timing */
    volatile uint16_t ticksToSend;                     // ticks until next message sent

    /* CAN Tx monitoring */
    volatile bool bHaveSentLastCAN;                     // has the last CAN message been sent

    volatile bool bNeedToSend;

    /*  number of CAN Tx message misses. This increments
     * every time a CAN message fails to send within TimingInterval time frame.
     * Decrements every 256 successful messages, every TimingInterval*256 ms
     */
    volatile uint16_t txCANMessageErrCnt;
    volatile uint8_t txCANMessageSucCnt;         // successful message counter

    /* CAN Lib Data */
    CANRaw::CAN_MOB mobHandle;                  //mob number that this class has been bound to
    volatile CANRaw::CAN_DATA canData;

    class Sensor* sensors[CANMAXDATACHANNELS] = {};          // Sensors sent on this CAN Message
    uint8_t activeSensors;                             // Number of sensors on this CAN Channel (used for dlc)
};


#endif /* CANSENSORTIMER_H_ */
