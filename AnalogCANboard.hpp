/*
 * AnalogCANboard.h
 *
 *  Created on: Mar 21, 2016
 *      Author: daniel
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <AVRLibrary/CPFECANLib.h>
#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/arduino/Wire.h>

class SensorCANmod {
public:
	//Declare variables
	uint16_t chan1, chan2, chan3, chan4;
	uint16_t ID; //ID is set for the standard format. Declare ID as a unit32_t for extended format
	uint8_t ADCaddress, reg_address;

	static constexpr uint8_t sendcanMOB0 = 0;
	static constexpr uint8_t sendcanMOB1 = 1;
	static constexpr uint8_t sendcanMOB2 = 2;
	static constexpr uint8_t sendcanMOB3 = 3;
	static constexpr uint8_t sendcanMOB4 = 4;
	static constexpr uint8_t sendcanMOB5 = 5;

	//Define ADC addresses
	static constexpr uint8_t ADC_A = 0x21;
	static constexpr uint8_t ADC_B = 0x22;
	static constexpr uint8_t ADC_C = 0x20;
	static constexpr uint8_t ADC_D = 0x23;
	static constexpr uint8_t ADC_E = 0x24;

	//Define ADC Registers
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
	typedef struct CANMessage {
		uint8_t adc;
		uint8_t reg1;
		uint8_t reg2;
		uint8_t reg3;
		uint8_t reg4;
		uint16_t msgId;
		uint8_t MOB;

		constexpr CANMessage(uint8_t adc, uint8_t reg1, uint8_t reg2, uint8_t reg3, uint8_t reg4, uint16_t msgId, uint8_t MOB) : adc(adc), reg1(reg1), reg2(reg2), reg3(reg3), reg4(reg4), msgId(msgId), MOB(MOB) {};
	} CANMessage;

	//Define CAN Messages
	static constexpr CANMessage CAN1 = CANMessage(ADC_A, VIN1, VIN3, VIN5, VIN7, 0x0C8, sendcanMOB0);
	static constexpr CANMessage CAN2 = CANMessage(ADC_A, VIN8, VIN6, VIN4, VIN2, 0x0C9, sendcanMOB1);
	static constexpr CANMessage CAN3 = CANMessage(ADC_B, VIN1, VIN3, VIN5, VIN7, 0x0CA, sendcanMOB2);
	static constexpr CANMessage CAN4 = CANMessage(ADC_B, VIN4, VIN2, VINund, VINund, 0x0CC, sendcanMOB3);
	static constexpr CANMessage CAN5 = CANMessage(ADC_C, VIN1, VIN3, VIN5, VIN7, 0x0CE, sendcanMOB4);
	static constexpr CANMessage CAN6 = CANMessage(ADC_B, VIN8, VIN6, VINund, VINund, 0x0CB, sendcanMOB5);
	static constexpr CANMessage CAN7 = CANMessage(ADC_C, VIN8, VIN6, VIN4, VINund, 0x0CF, sendcanMOB0);

	//Define Sample rates
	static constexpr CANMessage message200[] = { CAN1, CAN2, CAN3, CAN4, CAN5 };
	static constexpr CANMessage message100[] = { CAN6, CAN7 };
	static constexpr uint8_t Message200length = 5; //length of message200[]
	static constexpr uint8_t Message100length = 2; //length of message100[]

	//Functions:
	static void txCAN(uint16_t ID, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, uint8_t MOB) {
		uint8_t dataBuffer[8];
		CPFECANLib::MSG msg; //comes from CPECANLib.h

		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		//TODO: Do what? Always say what otherwise it's not very helpful :)
		msg.data = (chan1 << 48) | (chan2 << 32) | (chan3 << 16) | chan4; //concatenate the four channels into 64bit data for transmission
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;

		CPFECANLib::sendMsgUsingMOB(MOB, &msg);
	}

	static uint16_t getTWIdata(uint8_t ADCaddress, uint8_t reg_address) {
		uint16_t data;

		Wire.beginTransmission(ADCaddress); //transmit data to device
		Wire.write(reg_address); //set the register pointer to correct address
		Wire.endTransmission(); //stop transmitting
		data = Wire.read();

		return data;
	}

	static void RxTxCANdata(CANMessage CAN) {
		//Get TWI data

		if (CAN.reg1 != VINund) {
			chan1 = getTWIdata(CAN.adc, CAN.reg1);
		} else {
			chan1 = 0;
		}

		if (CAN.reg2 != VINund) {
			chan2 = getTWIdata(CAN.adc, CAN.reg2);
		} else {
			chan2 = 0;
		}

		if (CAN.reg3 != VINund) {
			chan3 = getTWIdata(CAN.adc, CAN.reg3);
		} else {
			chan3 = 0;
		}
		if (CAN.reg4 != VINund) {
			chan4 = getTWIdata(CAN.adc, CAN.reg4);
		} else {
			chan4 = 0;
		}
		//Send CAN Message
		txCAN(CAN.msgId, chan1, chan2, chan3, chan4, CAN.MOB);
	}

	static void updateCAN200() { //ISR for 200Hz Sampling
		for (int i=0; i < Message200length; ++i) {
			RxTxCANdata(message200[i]);
		}
	}

	static void updateCAN100() { //ISR for 100Hz Sampling
		for (int i=0; i < Message100length; ++i) {
			RxTxCANdata(message100[i]);
		}
	}

};

