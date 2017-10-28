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
	//MOB Numbers
  
	static constexpr uint8_t sendcanMOB0 = 0;
	static constexpr uint8_t sendcanMOB1 = 1;
	static constexpr uint8_t sendcanMOB2 = 2;
	static constexpr uint8_t sendcanMOB3 = 3;
	static constexpr uint8_t sendcanMOB4 = 4;
	static constexpr uint8_t sendcanMOB5 = 5;

   //one channel needed for each sensor, 6 sg, 1 sp
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

   #include "flConfig.hpp" //include the proper CAN board config
   //#include "frConfig.hpp"
   //#include "rlConfig.hpp"
   //#include "rrConfig.hpp"

   void initADC(){
      ADCSRA = 0x87; //Turn On ADC and set prescaler (CLK/128)
      ADCSRB = 0x00; //turn off autotrigger
      ADMUX = 0x00;     //Set ADC channel ADC0
   }

   uint16_t read(uint8_t ADCnum) {
      setADMUX(ADCnum);
      int voltage;
      int calibratedVoltage;
      ADCSRA = 0xC7;
      while(ADCSRA &(1<<ADSC));
      voltage = ADC & 0x3FF;
      calibratedVoltage = voltage+((0.014*voltage)-13.8);
      return calibratedVoltage;
   }

   void setADMUX(uint8_t ADCnum) {
      switch ADCnum {
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

	static void txCAN(uint16_t ID, CANMessageData *data, uint8_t MOB) {
		CPFECANLib::MSG msg; //comes from CPFECANLib.h in AVR library
		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		msg.data = (uint8_t *)data;
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;
		CPFECANLib::sendMsgUsingMOB(MOB, &msg);
	}
  
	static void TxCANdata(CANMessage CAN, uint8_t index) {
		CANMessageData messageData = {0, 0, 0, 0};
      if(index == 1)
         index += 3;
      //if channel 1 or 5 used
      //if CAN.sensNum
      messageData.chan1 = CPFECANLib::hton_uint16_t(read(index));
      //if channel 2 or 6 used
      messageData.chan2 = CPFECANLib::hton_uint16_t(read(index+1));
      messageData.chan3 = CPFECANLib::hton_uint16_t(read(index+2));
      messageData.chan4 = CPFECANLib::hton_uint16_t(read(index+3));
		txCAN(CAN.msgId, &messageData, CAN.MOB);
	}
   static void updateCAN1000() {
      for(int i=0; i<message1000length; i++)
        txCANdata(message1000[i], i);
   }
	
   static void updateCAN250() {
      for(int i=0; i<message250length; i++)
         txCANdata(message250[i], i);
   }
   static void updateCAN50() {
      for(int i=0; i<message50length; i++)
        txCANdata(message500[i], i);
   }

 
