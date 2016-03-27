/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */
#include "AnalogCANboard.hpp"

int main() { //Ignore this for now. I have not fixed this/ done set up interrupts yet
	SensorCANmod::updateCAN100();
	SensorCANmod::updateCAN200();
}

