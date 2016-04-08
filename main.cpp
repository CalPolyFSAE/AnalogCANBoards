/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include <AVRLibrary/arduino/Arduino.h>
#include <AVRLibrary/CPFECANLib.h>

#include "AnalogCANboard.hpp"



// Any variables modified by an ISR MUST be declared volatile.
// Compilers like to assume variables don't change between lines
// of code but if an ISR triggers, that assumption is invalid.
// Volatile tells the compiler to reload the variable from memory
// for every line of code.

volatile bool run200 = false;
volatile bool run100 = false;
volatile bool run100Next = false;

void timer1_init(){
	TCCR1B |= (1 << WGM12)|(1 << CS11); //set timer 1 to CTC mode and prescaler 8
	TCNT1 = 0; //initialize counter
	OCR1A = 9999;//This sets interrupt to 200Hz: [((16MHz/8)(0.005s))-1]
	TIMSK1 |= (1<<OCIE1A);//enable compare interrupt
	sei(); //enable global interrupts

}



SIGNAL(TIMER1_COMPA_vect) {
    run200 = true;
    if (run100Next) {
    	run100Next = false;
        run100 = false;
    }
    else {
    	run100Next=true;
    }
}

int main() {
	timer1_init(); //initialize timer 1 and interrupts
	Serial.begin(115200);
	Wire.begin();
	Wire.setClock(100000);
	CPFECANLib::init(CPFECANLib::CAN_BAUDRATE::B1M, nullptr);

    while (1) {
        if (run100) {
            run100 = false;
            SensorCANmod::updateCAN100();
        }
        if (run200) {
            run200 = false;
            SensorCANmod::updateCAN200();
        }
    }
}
