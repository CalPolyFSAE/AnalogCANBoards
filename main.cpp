/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <AVRLibrary/CPFECANLib.h>
#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/arduino/Wire.h>

class SensorCANmod{
public:
	//Declare variables
	uint16_t chan1, chan2, chan3, chan4;
	uint16_t ID; //ID is set for the standard format. Declare ID as a unit32_t for extended format
	uint8_t ADCaddress, reg_address;
	static constexpr uint8_t sendcanMOB = 5;

	//Functions:

	static void txCAN(uint16_t ID, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4) {
		uint8_t dataBuffer[8];
		CPFECANLib::MSG msg; //comes from CPECANLib.h

		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
		//TODO
		msg.data = (chan1<<48) | (chan2<<32) | (chan3<<16) | chan4; //concatenate the four channels into 64bit data for transmission
		msg.dlc = 8; //Number of bytes of data
		msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
		msg.rtr = 0;

		CPFECANLib::sendMsgUsingMOB(sendcanMOB, &msg);
	}

	static uint16_t getTWIdata(uint8_t ADCaddress, uint8_t reg_address){
		uint16_t data;

		Wire.beginTransmission(ADCaddress); //transmit data to device
		Wire.write(reg_address); //set the register pointer to correct address
		Wire.endTransmission();//stop transmitting
		data = Wire.read();

		return data;
	}

	static CAN getCANData(CAN can) {
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
	}
};


int main() {
	init(); //Arduino Timer Initialization, uses Timer 0 for millis() and other timing functions.
	Wire.begin(); //begin I2C comms

	int i=0; //loop counter
	double duration = 0.0; //amount by which to delay loop
	double start; //The start time of the timer

	//Declare variables for input channels
	uint16_t SGO4, SGO3, SGO2, SGO1, SGO8,SGO7, SGO6, SGO5;
	uint16_t SGO12, SGO11, SGO10, SGO9, TCO2, TCO1, SGO14, SGO13;
	uint16_t A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13;
	uint16_t A14, A15, A16, A17, A18, A19, A20, A21, A22, A23, A24;

	//Define ADC addresses
	uint8_t ADC_A = 0x21;
	uint8_t ADC_B = 0x22;
	uint8_t ADC_C = 0x20;
	uint8_t ADC_D = 0x23;
	uint8_t ADC_E = 0x24;

	//Define ADC Registers
	uint8_t VIN1= 0x80;
	uint8_t VIN2= 0x90;
	uint8_t VIN3= 0xA0;
	uint8_t VIN4= 0xB0;
	uint8_t VIN5= 0xC0;
	uint8_t VIN6= 0xD0;
	uint8_t VIN7= 0xE0;
	uint8_t VIN8= 0xF0;

	typedef struct CAN1 {
		uint8_t adc = ADC_A;
		uint8_t reg1 = VIN1;
		uint8_t reg2 = VIN3;
		uint8_t reg3 = VIN5;
		uint8_t reg4 = VIN7;
		uint16_t msgId = 0x0C8;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN1;

	typedef struct CAN2 {
		uint8_t adc = ADC_A;
		uint8_t reg1 = VIN8;
		uint8_t reg2 = VIN6;
		uint8_t reg3 = VIN4;
		uint8_t reg4 = VIN2;
		uint16_t msgId = 0x0C9;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN2;

	typedef struct CAN3 {
		uint8_t adc = ADC_B;
		uint8_t reg1 = VIN1;
		uint8_t reg2 = VIN3;
		uint8_t reg3 = VIN5;
		uint8_t reg4 = VIN7;
		uint16_t msgId = 0x0CA;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN3;

	typedef struct CAN4 {
		uint8_t adc = ADC_B;
		uint8_t reg1 = VIN1;
		uint8_t reg2 = VIN3;
		uint8_t reg3 = VIN5;
		uint8_t reg4 = VIN7;
		uint16_t msgId = 0x0CA;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN4;

	typedef struct CAN5 {
		uint8_t adc = ADC_C;
		uint8_t reg1 = VIN1;
		uint8_t reg2 = VIN3;
		uint16_t msgId = 0x0CC;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3 = 0x0;
		uint16_t chan4 = 0x0;
	} CAN5;

	typedef struct CAN6 {
		uint8_t adc = ADC_C;
		uint8_t reg1 = VIN5;
		uint8_t reg2 = VIN7;
		uint16_t msgId = 0x0CE;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3 = 0x0;
		uint16_t chan4 = 0x0;
	} CAN6;

	typedef struct CAN7 {
		uint8_t adc = ADC_B;
		uint8_t reg1 = VIN8;
		uint8_t reg2 = VIN6;
		uint16_t msgId = 0x0CB;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN7;

	typedef struct CAN8 { //Special case
		uint8_t adc = ADC_C;
		uint8_t reg1 = VIN8;
		uint8_t reg2 = VIN6;
		uint8_t reg3 = VIN8;
		uint8_t reg4 = VIN6;
		uint16_t msgId = 0x0CB;
		uint16_t chan1;
		uint16_t chan2;
		uint16_t chan3;
		uint16_t chan4;
	} CAN8;

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



			//RECEIVE
			//SGO4=SensorCANmod::getTWIdata(ADC_A, VIN1);
			//SGO3=SensorCANmod::getTWIdata(ADC_A, VIN3);
			//SGO2=SensorCANmod::getTWIdata(ADC_A, VIN5);
			//SGO1=SensorCANmod::getTWIdata(ADC_A, VIN7);
			//SensorCANmod::txCAN(0x0C8, SGO4, SGO3, SGO2, SGO1);

			//SGO8=SensorCANmod::getTWIdata(ADC_A, VIN8);
			//SGO7=SensorCANmod::getTWIdata(ADC_A, VIN6);
			//SGO6=SensorCANmod::getTWIdata(ADC_A, VIN4);
			//SGO5=SensorCANmod::getTWIdata(ADC_A, VIN2);
			//SensorCANmod::txCAN(0x0C9, SGO8, SGO7, SGO6, SGO5);

			//SGO12=SensorCANmod::getTWIdata(ADC_B, VIN1);
			//SGO11=SensorCANmod::getTWIdata(ADC_B, VIN3);
			//SGO10=SensorCANmod::getTWIdata(ADC_B, VIN5);
			//SGO9=SensorCANmod::getTWIdata(ADC_B, VIN7);
			//SensorCANmod::txCAN(0x0CA, SGO12, SGO11, SGO10, SGO9);

			//SGO14=SensorCANmod::getTWIdata(ADC_B, VIN4);
			//SGO13=SensorCANmod::getTWIdata(ADC_B, VIN2);
			//SensorCANmod::txCAN(0x0CC, SGO14, SGO13, 0x0, 0x0);

			//A1=SensorCANmod::getTWIdata(ADC_C, VIN1);
			//A2=SensorCANmod::getTWIdata(ADC_C, VIN3);
			//SensorCANmod::txCAN(0x0CC, A1, A2, 0x0, 0x0);

			//A3=SensorCANmod::getTWIdata(ADC_C, VIN5);
			//A4=SensorCANmod::getTWIdata(ADC_C, VIN7);
			//SensorCANmod::txCAN(0x0CE, A3, A4, 0x0, 0x0);

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
}

