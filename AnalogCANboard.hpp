/*
 * AnalogCANboard.h
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

	//ADC addresses
	static constexpr uint8_t ADC_A = 0x21;
	static constexpr uint8_t ADC_B = 0x22;
	static constexpr uint8_t ADC_C = 0x20;
	static constexpr uint8_t ADC_D = 0x23;
	static constexpr uint8_t ADC_E = 0x24;

	//ADC Registers
	static constexpr uint8_t VINund = 0x00;
	static constexpr uint8_t VIN1 = 0x80;
	static constexpr uint8_t VIN2 = 0x90;
	static constexpr uint8_t VIN3 = 0xA0;
	static constexpr uint8_t VIN4 = 0xB0;
	static constexpr uint8_t VIN5 = 0xC0;
	static constexpr uint8_t VIN6 = 0xD0;
	static constexpr uint8_t VIN7 = 0xE0;
	static constexpr uint8_t VIN8 = 0xF0;

	//Define CAN Message Struct
	typedef struct {
		uint8_t adc;
		uint8_t reg1;
		uint8_t reg2;
		uint8_t reg3;
		uint8_t reg4;
		uint16_t msgId;
		uint8_t MOB;
	} CANMessage;

	typedef struct {
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CANMessageData;

#include "BoardConfigurations/Combustion-Rear.hpp" //include the proper CAN board configuration

	//Functions:
	static void txCAN(uint16_t ID, CANMessageData *data, uint8_t MOB) {
		CPFECANLib::MSG msg; //comes from CPECANLib.h

		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		msg.data = (uint8_t *)data;
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;
		CPFECANLib::sendMsgUsingMOB(MOB, &msg);
	}

	static uint16_t getTWIdata(uint8_t ADCaddress, uint8_t reg_address) {
		uint16_t data;

		Wire.beginTransmission(ADCaddress); //transmit data to device
		Wire.write(reg_address); //set the register pointer to correct address
		Wire.endTransmission(); //transmit
		Wire.requestFrom(ADCaddress, 2, true);
		data = (uint16_t)(Wire.read() & 0x0F) << 8;
		data |= Wire.read();


		return data; //And data with a 12 bitmask
	}

	static void RxTxCANdata(CANMessage CAN) {
		float volts;
		uint16_t printdata;
		CANMessageData messageData = {0, 0, 0, 0};



		if (CAN.reg1 != VINund) {
			messageData.chan1 = CPFECANLib::hton_uint16_t(getTWIdata(CAN.adc, CAN.reg1));


		}
		if (CAN.reg2 != VINund) {
			messageData.chan2 = CPFECANLib::hton_uint16_t(getTWIdata(CAN.adc, CAN.reg2));


		}
		if (CAN.reg3 != VINund) {
			messageData.chan3 = CPFECANLib::hton_uint16_t(getTWIdata(CAN.adc, CAN.reg3));


		}
		if (CAN.reg4 != VINund) {
			messageData.chan4 = CPFECANLib::hton_uint16_t(getTWIdata(CAN.adc, CAN.reg4));
		}

		txCAN(CAN.msgId, &messageData, CAN.MOB);

	}

	static void updateCAN200() { //ISR for 200Hz Sampling
		for (int i = 0; i < Message200length; ++i) {
			RxTxCANdata(message200[i]);
		}
	}

	static void updateCAN100() { //ISR for 100Hz Sampling
		for (int i = 0; i < Message100length; ++i) {
			RxTxCANdata(message100[i]);
		}
	}
};

