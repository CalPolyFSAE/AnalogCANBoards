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
#include "Config/CONFIG.h"
#include "Sensor.h"

//timing objects for tracking when a message needs to be sent and what
//is sent with it and fetching the data to be sent
class CANSensorTimer
{
public:
    const uint16_t TimingInterval;          // milliseconds between messages
    const uint16_t CANID;                   // CANID used for this CAN Channel

    CANSensorTimer(uint16_t interval, uint16_t CANId);

    //registers a sensor on this CANID
    bool registerSensor(Sensor* sensor, CANCONFIG::CANDATAChannel dataChannel);

    //1000Hz interrupt to keep track of timing
    void INT_Call_Tick();

    //make sensors request data if a CAN message needs to be sent
    //then send data over CAN
    void Update();

    //milliseconds until CAN message needs to be sent
    uint16_t timeUntilSend();

private:
    volatile bool needToSend;                          // CAN message needs to be sent
    volatile uint16_t ticksToSend;                     // ticks until next message sent
    Sensor* sensors[CANCONFIG::CANMAXDATACHANNELS];    // Sensors sent on this CAN Message
    uint8_t activeSensors;                             // Number of sensors on this CAN Channel
};


#endif /* CANSENSORTIMER_H_ */
