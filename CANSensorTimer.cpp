/*
* CANSensorTimer.cpp
*
*  Created on: Dec 10, 2017
*      Author: root
*/
#include "Config/CONFIG00.h"

#include "CANSensorTimer.h"
#include "Sensor.h"
#include "Commands/CommandManager.h"

CANSensorTimer::CANSensorTimer(uint16_t interval, const uint32_t* can_id, uint8_t can_ide) :
    TimingInterval(interval), id(*can_id), ide(can_ide)
{
    ticksToSend = interval;
    activeSensors = 0;

    CANRaw& can = CANRaw::StaticClass();
    mobHandle = can.GetNextFreeHandle();

    //bind this class to the next open mob handle
    if(!can.BindListener(this, mobHandle))
    {
        //error
        CommandManager::StaticClass().LogMessageln(FSTR("FAILED TO BIND LISTENER"));
    }
    CommandManager::StaticClass().LogMessage(FSTR("CANSensorTimer Created: "));
    CommandManager::StaticClass().LogMessage((uint8_t)(mobHandle));
    CommandManager::StaticClass().LogMessage(FSTR(" ID: "));
    CommandManager::StaticClass().LogMessage((uint8_t)id);
    CommandManager::StaticClass().LogMessage(FSTR(" Interval: "));
    CommandManager::StaticClass().LogMessageln((uint8_t)TimingInterval);

    bHaveInitialized = false;
    bHaveSentLastCAN = true;
    txCANMessageErrCnt = 0;
    txCANMessageSucCnt = 0;
}

//registers a sensor on this CANChannel at dataChannel position
bool CANSensorTimer::registerSensor(Sensor* sensor, CANDATAChannel dataChannel)
{
    uint8_t index = (uint8_t) dataChannel;
    if (index > CANMAXDATACHANNELS)          // check array bounds
        return false;
    if (sensors[index] != nullptr) // make sure another sensor is not here
        return false; // if there is TODO: ADD critical error functionality

    //sort out CAN message size (for dlc)
    if (index >= activeSensors)
        activeSensors = index + 1;

    //register the sensor
    sensors[index] = sensor;
    bHaveInitialized = false;
    return true;
}

void CANSensorTimer::INT_Call_SentFrame(const CANRaw::CAN_FRAME_HEADER& frameConfig)
{
    bHaveSentLastCAN = true;
}

//make sensors request data if a CAN message needs to be sent
//then send data over CAN
void CANSensorTimer::Update()
{
    static bool isFirstError = false;
    if(!bHaveInitialized)// has the CAN Mob been configured?
    {
        CANRaw& can = CANRaw::StaticClass();
        // create CAN Tx settings
        CANRaw::CAN_FRAME_HEADER tmpHeader{
            id,         // can id
            0,          // rtr flag
            0,          // ide flag
            (static_cast<uint8_t>(activeSensors * CANBYTESPERDATACHANNEL))      // dlc
        };
        can.ForceResetMob(mobHandle); // make sure mob is ready
        if(can.ConfigTx(tmpHeader, mobHandle))// attempt to config mob as Tx
        {
            bHaveInitialized = true;
        }else
        {
            //TODO: error
            CommandManager::StaticClass().LogMessageln(FSTR("Unable to ConfigTx"));
        }
    }
    else if(needToSend)
    {
        if(bHaveSentLastCAN)
        {
            needToSend = false;
            isFirstError = false; // reset error flag
            bHaveSentLastCAN = false;
            ++txCANMessageSucCnt; // increment success counter
            /////////////////
            /////////////////
            ////////////////

            if(txCANMessageErrCnt > 0 && txCANMessageSucCnt == 255)
            {
                //decrement error count
                --txCANMessageErrCnt;
            }

            //read data on all sensors

            for (uint8_t i = 0; i < activeSensors;)
            {
                //only go to next sensor after the request is successful
                //this should only take multiple requests if ADC clock speed is
                //very slow
                if (sensors[i] != nullptr)
                {
                    if (sensors[i]->requestADCRead ())
                    {
                        // Sensor has completed last read
                        ++i;
                    }
                }
                else
                {
                    ++i;
                }
            }


            //check that all sensors got value from ADC
            // this is really unnecessary
            //TODO: add full speed sampling so it REALLY IS unnecessary
            /*
             for (uint8_t i = 0; i < activeSensors;)
             {
             if (sensors[i] != nullptr)
             {
             if (sensors[i]->getIsReady ())
             {
             ++i;
             }
             }
             else
             {
             ++i;
             }
             }
             */

            //package CAN message using sensor values
            canData = {};
            for (uint8_t i = 0; i < activeSensors; ++i)
            {
                if (sensors[i] == nullptr)
                {
                    continue;
                }
                //TODO: Need to add variable data sizes for CAN data channels
                //CANData.data16[i] = sensors[i]->getValue();
                int16_t data = sensors[i]->getValue ();

                // hton (byte swap)
                uint8_t* p_n = (uint8_t *) &data;
                uint8_t lower = p_n[0];
                p_n[0] = p_n[1];
                p_n[1] = lower;


                // use mcpy for variable data channel sizes
                ((uint16_t*)canData.byte)[i] = data;

                //TESTING:
                /*
                 Serial.print (" DATA: ");
                 Serial.print(data);

                 Serial.print (" CAN DATA: ");
                 Serial.print(canData.data16[i]);

                 Serial.print (" CH: ");
                 Serial.print (sensors[i]->ADCChannel);
                 Serial.println ("");

                 /*
                 Serial.print (" V: ");
                 /*
                 float volts;
                 sensors[i]->getVoltage (volts);
                 Serial.print (volts, 4);
                 Serial.println ("");
                 */

            }

            //send CAN message
            CANRaw::StaticClass().TxData(canData, mobHandle);

        }else
        {
            // missed a CAN message
            if(!isFirstError)
            {
                isFirstError = true;
                ++txCANMessageErrCnt;
            }
        }


    }
}

//returns ticks until the CAN message needs to be sent
//time value dependent on how the interrupt timer is configured
uint16_t CANSensorTimer::timeUntilSend()
{
    //need to copy this atomically because of INT_Call_Tick changing value
    uint16_t time;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        time = ticksToSend;
    }
    return time;
}

uint8_t CANSensorTimer::getNumActiveSensors()
{
    return activeSensors;
}


