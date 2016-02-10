/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
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





	static constexpr uint8_t sendcanMOB = 5;

	//Functions:

	static void txCAN(uint16_t ID, uint16_t chan1, uint16_t chan2, uint16_t chan3, uint16_t chan4) {
		uint8_t dataBuffer[8];
		CPFECANLib::MSG msg;

		msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
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
};


int main() {
	init(); //Arduino Timer Initialization, uses Timer 0 for millis() and other timing functions.
	Wire.begin(); //begin I2C comms

	int i=0; //loop counter

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

	while (1) {
		while(i<2){
			delay(5);

			//inner loop contains channels sampled at 200Hz
			SGO4=SensorCANmod::getTWIdata(ADC_A, VIN1);
			SGO3=SensorCANmod::getTWIdata(ADC_A, VIN3);
			SGO2=SensorCANmod::getTWIdata(ADC_A, VIN5);
			SGO1=SensorCANmod::getTWIdata(ADC_A, VIN7);
			SensorCANmod::txCAN(0x0C8, SGO4, SGO3,SGO2, SGO1);

			i++;
		}
		i=0;
		//Outer loop includes channels sampled at 100Hz
		TCO2=SensorCANmod::getTWIdata(ADC_B, VIN8);
		TCO1=SensorCANmod::getTWIdata(ADC_B, VIN6);
		A5=SensorCANmod::getTWIdata(ADC_C, VIN8);
		A6=SensorCANmod::getTWIdata(ADC_C, VIN6);
		SensorCANmod::txCAN(0x0CB, TCO2, TCO1, A5, A6);
	}
}

