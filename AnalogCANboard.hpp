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

class SensorCANmod {
public:

   //mailbox numbers
   //3 IDs for 3 different possible freqs
   static constexpr uint16_t CAN0id = 0x0A0;
   static constexpr uint16_t CAN1id = 0x0A1;
   static constexpr uint16_t CAN2id = 0x0A2;
	//MOB Numbers
  
	static constexpr uint8_t sendcanMOB0 = 0;
	static constexpr uint8_t sendcanMOB1 = 1;
	static constexpr uint8_t sendcanMOB2 = 2;
	static constexpr uint8_t sendcanMOB3 = 3;
	static constexpr uint8_t sendcanMOB4 = 4;
	static constexpr uint8_t sendcanMOB5 = 5;
   
   
	typedef struct {
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CANMessageData;
   

   typedef struct {
      uint8_t msgId;
      uint8_t MOB;
      uint8_t sensNum;
   } CANMessage;

   //#include "BoardConfigurations/flConfig.hpp" //include the proper CAN board config
   //front right config
   static constexpr CANMessage CAN0 = { CAN0id, sendcanMOB0, 4};
   static constexpr CANMessage CAN1 = { CAN1id, sendcanMOB1, 3};
   static constexpr CANMessage message1000[] = { CAN0, CAN1 };
   static constexpr uint8_t Message1000length = 2;
   //static constexpr CANMessage CAN2 = { CAN2id, sendcanMOB2, 2};


   static void initADC(){
      ADCSRA = 0x87; //Turn On ADC and set prescaler (CLK/128)
      ADCSRB = 0x00; //turn off autotrigger
      ADMUX = 0x00;     //Set ADC channel ADC0
   }

   static uint16_t read(uint8_t ADCnum) {
      setADMUX(ADCnum);
      int voltage;
      int calibratedVoltage;
      ADCSRA = 0xC7;
      while(ADCSRA &(1<<ADSC));
      voltage = ADC & 0x3FF;
      calibratedVoltage = voltage+((0.014*voltage)-13.8);
      return calibratedVoltage;
   }

   static void setADMUX(uint8_t ADCnum) {
      switch (ADCnum) {
         case 0:
            ADMUX &= ~((MUX2<<1)|(MUX1<<1)|(MUX0<<1));
            break;
         case 1:
            ADMUX |= (MUX0<<1);
            ADMUX &= ~((MUX2<<1)|(MUX1<<1));
            break;
         case 2:
            ADMUX |= (MUX1<<1);
            ADMUX &= ~((MUX2<<1)|(MUX1<<1));
            break;
         case 3:
            ADMUX |= (MUX1<<1)|(MUX0<<1);
            ADMUX &= ~(MUX2<<1);
            break;
         case 4:
            ADMUX |= (MUX2<<1);
            ADMUX &= ~((MUX1<<1)|(MUX0<<1));
            break;
         case 5:
            ADMUX |= (MUX2<<1)|(MUX0<<1);
            ADMUX &= ~(MUX1<<1);
            break;
         case 6:
            ADMUX |= (MUX2<<1)|(MUX1<<1);
            ADMUX &= ~(MUX0<<1);
            break;
         case 7:
            ADMUX |= (MUX2<<1)|(MUX1<<1)|(MUX0<<1);
            break;
      }
   }

	static void txCAN(uint16_t ID, void *data, uint8_t MOB) {
		CPFECANLib::MSG msg; //comes from CPFECANLib.h in AVR library
		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		msg.data = (uint8_t *)data;
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;
		CPFECANLib::sendMsgUsingMOB(MOB, &msg);
	}
   
   static void setChanData(CANMessageData messageData, CANMessage CAN, uint8_t messageNum) {
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
	static void txCANdata(CANMessage CAN, uint8_t messageNum) {
		CANMessageData messageData = {0,0,0,0};
		setChanData(messageData, CAN, messageNum);
		txCAN(CAN.msgId, &messageData, CAN.MOB);
	}

	static void updateCAN1000() {
		for (int i = 0; i < Message1000length; ++i)
			txCANdata(message1000[i], i);
   }
   /*
   static void updateCAN250() {
		for (int i = 0; i < Message250length; ++i)
         txCANdata(message250[i], i);
   }

   static void updateCAN50() {
      for (int i = 0; i < Message50length; ++i)
         txCANdata(message[50], i);
   }
   */
};
