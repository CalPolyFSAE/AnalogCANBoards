/*
 * SensorManager.h
 *
 * Manages the Sensor object timing
 *
 *  Created on: Nov 13, 2017
 *      Author: root
 */

#ifndef SENSORMANAGER_H_
#define SENSORMANAGER_H_


#include <stdint.h>
#include <util/atomic.h>
#include "Config/CONFIG.h"
#include "Sensor.h"

class SensorManager
{
public:
    //all Sensors
    static Sensor* channels[SENSORS];

    //sets up channels array
    static void Init();

    //1000Hz interrupt calls this
    static void INT_UpdateTiming();

private:
    //timing objects for tracking when a message needs to be sent and what
    //is sent with it
    class CANMessageTimer
    {
    public:
        const uint16_t TimingInterval;          // milliseconds between messages
        const uint16_t CANID;                   // CANID used for this CAN Channel

        CANMessageTimer(uint16_t interval, uint16_t CANId) :
            TimingInterval(interval), CANID(CANId)
        {
            ticksToSend = interval;
            activeSensors = 0;
        }

        bool registerSensor(Sensor* sensor, CANDATAChannel dataChannel)
        {
            uint8_t index = (uint8_t) dataChannel;
            if (index > CANMAXDATACHANNELS)          // check array bounds
                return false;
            if (sensors[index] != nullptr) // make sure another sensor is not here
                return false; // if there is TODO: ADD critical error functionality

            //sort out CAN message size
            if (index >= activeSensors)
                activeSensors = index + 1;
            sensors[index] = sensor;
            return true;
        }

        void INT_Call_Tick()
        {
            --ticksToSend;
            if(ticksToSend == 0)
            {
                needToSend = true;
                ticksToSend = TimingInterval;
            }
        }

        uint16_t timeUntilSend()
        {
            //need to copy this atomically because of INT_Call_Tick changing value
            uint16_t time;
            ATOMIC_BLOCK(ATOMIC_FORCEON)
            {
                time = ticksToSend;
            }
            return time;
        }

    private:
        volatile bool needToSend;               // CAN message needs to be sent
        volatile uint16_t ticksToSend;          // ticks until next message sent
        Sensor* sensors[CANMAXDATACHANNELS];    // Sensors sent on this CAN Message
        uint8_t activeSensors;                  // Number of sensors on this CAN Message
    };

    //CAN Message Timers
    static CANMessageTimer* CANMessageTimers[CANCHANNELS];

};


#endif /* SENSORMANAGER_H_ */
