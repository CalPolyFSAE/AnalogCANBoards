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


class SensorCANmod{
public:
	//Declare variables
	uint16_t chan1, chan2, chan3, chan4;
	uint16_t ID; //ID is set for the standard format. Declare ID as a unit32_t for extended format
	uint8_t ADCaddress, reg_address;

	uint_t count = 0; loop counter

	static constexpr uint8_t sendcanMOB0 = 0;
	static constexpr uint8_t sendcanMOB1 = 1;
	static constexpr uint8_t sendcanMOB2 = 2;
	static constexpr uint8_t sendcanMOB3 = 3;
	static constexpr uint8_t sendcanMOB4 = 4;
	static constexpr uint8_t sendcanMOB5 = 5;

	//Define ADC addresses
	constexpr uint8_t ADC_A = 0x21;
	constexpr uint8_t ADC_B = 0x22;
	constexpr uint8_t ADC_C = 0x20;
	constexpr uint8_t ADC_D = 0x23;
	constexpr uint8_t ADC_E = 0x24;

	//Define ADC Registers
	constexpr uint8_t VINund = 0x00;
	constexpr uint8_t VIN1= 0x80;
	constexpr uint8_t VIN2= 0x90;
	constexpr uint8_t VIN3= 0xA0;
	constexpr uint8_t VIN4= 0xB0;
	constexpr uint8_t VIN5= 0xC0;
	constexpr uint8_t VIN6= 0xD0;
	constexpr uint8_t VIN7= 0xE0;
	constexpr uint8_t VIN8= 0xF0;

	//Define CAN Message Struct
	typedef struct CANMessage{
		uint8_t adc;
		uint8_t reg1;
		uint8_t reg2;
		uint8_t reg3;
		uint8_t reg4;
		uint16_t msgId;
		uint8_t MOB;
	};

	//Define CAN Messages
	constexpr CANMessage CAN1 = {//200Hz
		CAN1.adc = ADC_A,
		CAN1.reg1 = VIN1,
		CAN1.reg2 = VIN3,
		CAN1.reg3 = VIN5,
		CAN1.reg4 = VIN7,
		CAN1.msgId = 0x0C8,
		CAN1.MOB = sendcanMOB0};

	constexpr CANMessage CAN2 = {//200Hz
		CAN2.adc = ADC_A,
		CAN2.reg1 = VIN8,
		CAN2.reg2 = VIN6,
		CAN2.reg3 = VIN4,
		CAN2.reg4 = VIN2,
		CAN2.msgId = 0x0C9,
		CAN2.MOB = sendcanMOB1};

	constexpr CANMessage CAN3 = {//200Hz
		CAN3.adc = ADC_B,
		CAN3.reg1 = VIN1,
		CAN3.reg2 = VIN3,
		CAN3.reg3 = VIN5,
		CAN3.reg4 = VIN7,
		CAN3.msgId = 0x0CA,
		CAN3.MOB = sendcanMOB2};

	constexpr CANMessage CAN4 = {//200Hz
		CAN4.adc = ADC_B,
		CAN4.reg1 = VIN4,
		CAN4.reg2 = VIN2,
		CAN4.reg3 = VINund,
		CAN4.reg4 = VINund,
		CAN4.msgId = 0x0CC,
		CAN4.MOB = sendcanMOB3};

	constexpr CANMessage CAN5 = {//200Hz
		CAN5.adc = ADC_C,
		CAN5.reg1 = VIN1,
		CAN5.reg2 = VIN3,
		CAN5.reg3 = VIN5,
		CAN5.reg4 = VIN7,
		CAN5.msgId = 0x0CE,
		CAN5.MOB = sendcanMOB4};

	constexpr CANMessage CAN6 = {//100Hz
		CAN6.adc = ADC_B,
		CAN6.reg1 = VIN8,
		CAN6.reg2 = VIN6,
		CAN6.reg3 = VINund,
		CAN6.reg4 = VINund,
		CAN6.msgId = 0x0CB,
		CAN6.MOB = sendcanMOB5};

	constexpr CANMessage CAN7 = {//100Hz
		CAN7.adc = ADC_C,
		CAN7.reg1 = VIN8,
		CAN7.reg2 = VIN6,
		CAN7.reg3 = VIN4,
		CAN7.reg4 = VINund,
		CAN7.msgId = 0x0CF,
		CAN7. MOB = sendcanMOB0};

	//Define Sample rates
	CANMessage message200[] = {CAN1, CAN2, CAN3, CAN4, CAN5};
	CANMessage message100[] = {CAN6, CAN7};
	constexpr uint8_t Message200length = 5; //length of message200[]
	constexpr uint8_t Message100length = 2; //length of message100[]


	//Functions:
	static void txCAN(uint16_t ID, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4, uint8_t MOB) {
		uint8_t dataBuffer[8];
		CPFECANLib::MSG msg; //comes from CPECANLib.h

		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		//TODO
		msg.data = (chan1<<48) | (chan2<<32) | (chan3<<16) | chan4; //concatenate the four channels into 64bit data for transmission
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;

		CPFECANLib::sendMsgUsingMOB(MOB, &msg);
	}

	static uint16_t getTWIdata(uint8_t ADCaddress, uint8_t reg_address){
		uint16_t data;

		Wire.beginTransmission(ADCaddress); //transmit data to device
		Wire.write(reg_address); //set the register pointer to correct address
		Wire.endTransmission();//stop transmitting
		data = Wire.read();

		return data;
	}

	/*static CAN getCANData(CAN can) {
		if(can.msgId == 0x0CC || can.msgId == 0x0CE) {
			can.chan1 = SensorCANmod::getTWIdata(can.adc, can.reg1);
			can.chan2 = SensorCANmod::getTWIdata(can.adc, can.reg2);
		}
		else {
			can.chan1 = SensorCANmod::getTWIdata(can.adc, can.reg1);
			can.chan2 = SensorCANmod::getTWIdata(can.adc, can.reg2);
			can.chan3 = SensorCANmod::getTWIdata(can.adc, can.reg3);
			can.chan4 = SensorCANmod::getTWIdata(can.adc, can.reg4);
		}

		return can;
	}/*



/*void updateCAN() {
	init(); //Arduino Timer Initialization, uses Timer 0 for millis() and other timing functions.
	Wire.begin(); //begin I2C comms

	int i=0; //loop counter






	uint16_t TOREADCAP = 2;
    CAN READ100[TOREADCAP];
    CAN READ200[TOREADCAP];
	uint16_t canIndex100 = 0;
	uint16_t canIndex200 = 0;

	while (1) {
		while(i<2){

			//LOOP HERE
				//Load in 100Hz and 200Hz CANS into arrays here
				//Condition timer statement for 100Hz
					READ100[canIndex] = SensorCANmod::getCANData(READ100[canIndex100]);
					SensorCANmod::txCAN(READ100[canIndex100].msgId, READ100[canIndex100].chan1, READ100[canIndex100].chan2,
						READ100[canIndex100].chan3, READ100[canIndex100].chan4);
				//Conditional timer statement for 200Hz
					READ200[canIndex] = SensorCANmod::getCANData(READ200[canIndex200]);
					SensorCANmod::txCAN(READ200[canIndex200].msgId, READ200[canIndex200].chan1, READ200[canIndex200].chan2,
						READ200[canIndex200].chan3, READ200[canIndex200].chan4);

			i++;
		}

		i=0;

		//TCO2=SensorCANmod::getTWIdata(ADC_B, VIN8);
		//TCO1=SensorCANmod::getTWIdata(ADC_B, VIN6);
		//A5=SensorCANmod::getTWIdata(ADC_C, VIN8);
		///A6=SensorCANmod::getTWIdata(ADC_C, VIN6);
		//SensorCANmod::txCAN(0x0CB, TCO2, TCO1, A5, A6);
		//SensorCANmod::txCAN(0x0CD, A7, 0x0, 0x0, 0x0); //What to do for this one?
	}
}*/

	static void RxTxCANdata(CANMessage CAN){
		//Get TWI data

		if(CAN.reg1 != VINund){
			Wire.beginTransmission(CAN.adc); //transmit data to device
			Wire.write(CAN.reg1); //set the register pointer to correct address
			Wire.endTransmission();//stop transmitting
			chan1 = Wire.read();
		}
		else{
			chan1=0x0000;
		}

		if(CAN.reg2 != VINund){
			Wire.beginTransmission(CAN.adc); //transmit data to device
			Wire.write(CAN.reg2); //set the register pointer to correct address
			Wire.endTransmission();//stop transmitting
			chan2 = Wire.read();
		}
		else{
			chan2 = 0x0000;
		}

		if(CAN.reg3 != VINund){
			Wire.beginTransmission(CAN.adc); //transmit data to device
			Wire.write(CAN.reg3); //set the register pointer to correct address
			Wire.endTransmission();//stop transmitting
			chan3 = Wire.read();
		}
		else{
			chan3 = 0x0000;
		}
		if(CAN.reg4 != VINund){
			Wire.beginTransmission(CAN.adc); //transmit data to device
			Wire.write(CAN.reg4); //set the register pointer to correct address
			Wire.endTransmission();//stop transmitting
			chan4 = Wire.read();
		}
		else{
			chan4 = 0x0000;
		}
		//Send CAN Message
		txCAN(CAN.msgId, chan1, chan2, chan3, chan4, CAN.MOB);

	}

	static void updateCAN200(){//ISR for 200Hz Sampling
		while (count < Message200length){
			RxTxCANdata(message200[count]);
			count++;
		}
		count = 0; //reset count
	}

	static void updateCAN100(){//ISR for 100Hz Sampling
		while(count < Message100length){
			RxTxCANdata(message100[count]);
			count++
		}
		count = 0; //reset count
	}


};

