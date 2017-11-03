/*
 * AnalogCANboard.hpp
 *
 *  Created on: Mar 21, 2016
 *      Author: daniel
 */

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <AVRLibrary/CPFECANLib.h>
#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/arduino/Wire.h>

#define FLCONFIG
//#define FRCONFIG
//#define RLCONFIG
//#define RRCONFIG

class SensorCANmod
{
public:
    //3 IDs for 3 different possible freqs
    static constexpr uint16_t CAN0id = 0x0A0;
    static constexpr uint16_t CAN1id = 0x0A1;
    static constexpr uint16_t CAN2id = 0x0A2;

    static constexpr uint16_t CAN3id = 0x0A3;
    static constexpr uint16_t CAN4id = 0x0A4;

    //number of CAN messages used
    static constexpr uint8_t CANMAXMESSAGES = 3;

    //MOB Numbers
    static constexpr uint8_t sendcanMOB0 = 0;
    static constexpr uint8_t sendcanMOB1 = 1;
    static constexpr uint8_t sendcanMOB2 = 2;
    static constexpr uint8_t sendcanMOB3 = 3;
    static constexpr uint8_t sendcanMOB4 = 4;
    static constexpr uint8_t sendcanMOB5 = 5;

    //Maximum channels allowed per CAN message
    static constexpr uint8_t CANMAXCHANNELS = 4;

    static volatile bool run1000;
    static volatile bool run250;
    static volatile bool run50;

protected:

    typedef struct CANChannelLengths
    {
        uint8_t CAN0ChannelsLength;
        uint8_t CAN1ChannelsLength;
        uint8_t CAN2ChannelsLength;
    } CANChannelLengths;

    //number of channels to send for each CAN message id
    static CANChannelLengths CANChannelLen;

    static void initADC();

public:

    typedef struct CANMessageData
    {
        uint16_t a;
        uint16_t b;
        uint16_t c;
        uint16_t d;
    } CANMessageData;

    enum class ADC_chan
        : uint8_t
        {
            ADC0 = 0, ADC1, ADC2, ADC3, ADC4, ADC5, ADC6, ADC7, ADCNULL
    };

    typedef struct CANMessage
    {
        uint16_t msgId;
        uint8_t MOB;
        ADC_chan channels[CANMAXCHANNELS];
        //uint8_t sensNum;
    } CANMessage;

    /**************************************
     Front left config(FLCONFIG)
     Strain gauges (quantity = 6) - 1000 Hz
     Shock Pot LF - 1000 Hz
     **************************************/
#ifdef FLCONFIG
    static constexpr CANMessage CAN0 =
        { CAN0id, sendcanMOB0,
            {
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL
            }
        }; //4x sg
    static constexpr CANMessage CAN1 =
        { CAN1id, sendcanMOB1,
            {
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL
            }
        }; //4x sg
    static constexpr CANMessage CAN2 =
        { CAN2id, sendcanMOB2,
            {
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL,
                ADC_chan::ADCNULL
            }
        }; //4x sg
    //a static constant array of constant pointers to constant CANMessages
    //constexpr tells compiler that pointers can be evaluated at compile time
    static constexpr const CANMessage* const message1000[] =
        { &CAN0, &CAN1 };
    static constexpr const CANMessage* const message250[] =
        { &CAN1 };
    static constexpr const CANMessage* const message50[] =
        { };
    static constexpr uint8_t Message1000length = 2;
    static constexpr uint8_t Message250length = 1;
    static constexpr uint8_t Message50length = 0;

#endif

    /**************************************
     Front right config(FRCONFIG)
     Shock Pot RF - 1000 Hz
     Front Brake Pressure - 250 Hz
     Rear Brake Pressure - 250 Hz
     Front accelerometer - 250 Hz
     Steering Position Pot - 50 Hz
     **************************************/
#ifdef FRCONFIG
    static constexpr CANMessage CAN0 =
        { CAN0id, sendcanMOB0, 1 }; //sp
    static constexpr CANMessage CAN1 =
        { CAN1id, sendcanMOB1, 3 }; //fbp, rbp, fa
    static constexpr CANMessage CAN2 =
        { CAN2id, sendcanMOB2, 1 }; //spp
    static constexpr CANMessage message1000[] =
        { CAN0 };
    static constexpr CANMessage message250[] =
        { CAN1 };
    static constexpr CANMessage message50[] =
        { CAN2 };
    static constexpr uint8_t Message1000length = 1;
    static constexpr uint8_t Message250length = 1;
    static constexpr uint8_t Message50length = 1;
#endif

    /**************************************
     Rear left config(RLCONFIG)
     Strain gauges (qauntity = 6) - 1000 Hz
     Shock Pot LF - 1000 Hz
     **************************************/
#ifdef RLCONFIG
    static constexpr CANMessage CAN0 =
        { CAN0id, sendcanMOB0, 4 }; //4x sg
    static constexpr CANMessage CAN1 =
        { CAN1id, sendcanMOB1, 3 }; //3x sg, sp
    static constexpr CANMessage message1000[] =
        { CAN0, CAN1 };
    static constexpr uint8_t Message1000length = 2;
#endif

    /**************************************
     Rear right config(RRCONFIG)
     Shock Pot RF - 1000 Hz
     Rear accelerometer - 250 Hz
     **************************************/
#ifdef RRCONFIG
    static constexpr CANMessage CAN0 =
        { CAN0id, sendcanMOB0, 1 }; //sp
    static constexpr CANMessage CAN1 =
        { CAN1id, sendcanMOB1, 1 }; //ra
    static constexpr CANMessage message1000[] =
        { CAN0 };
    static constexpr CANMessage message250[] =
        { CAN1 };
    static constexpr uint8_t Message1000length = 1;
    static constexpr uint8_t Message250length = 1;
#endif

    static void init();

    static void update();

    static void updateCAN( const CANMessage* const Messages[], uint8_t CANMessagesLength );

    static uint16_t read( uint8_t ADCnum );
    static void setADMUX( uint8_t ADCnum );
    static void txCAN( uint16_t ID, void *data, uint8_t dlc, uint8_t MOB );

    //need to pass by ref
    /*
     static void setChanData(CANMessageData &messageData, CANMessage CAN, uint8_t messageNum) {
     uint8_t num = messageNum*4;
     messageData.chan1 = CPFECANLib::hton_uint16_t(read(num));
     if(CAN.sensNum > num+1)
     messageData.chan2 = CPFECANLib::hton_uint16_t(read(num+1));
     else
     return;
     if(CAN.sensNum > num+2)
     messageData.chan3 = CPFECANLib::hton_uint16_t(read(num+2));
     else
     return;
     if(CAN.sensNum > num+3)
     messageData.chan4 = CPFECANLib::hton_uint16_t(read(num+3));
     }
     */
};
