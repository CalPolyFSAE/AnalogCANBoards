/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "AnalogCANboard.hpp"



// Any variables modified by an ISR MUST be declared volatile.
// Compilers like to assume variables don't change between lines
// of code but if an ISR triggers, that assumption is invalid.
// Volatile tells the compiler to reload the variable from memory
// for every line of code.

volatile bool run200 = false;
volatile bool run100 = false;
volatile bool ran100 = false;

void timer1_init(){
	TCCR1B |= (1 << WGM12)|(1 << CS10); //set timer 1 to CTC mode and prescaler 1.
	TCNT1 = 0; //initialize counter
	OCR1A = 65535;//This sets interrupt to 244.14Hz
	TIMSK1 |= (1<<OCIE1A);//enable compare interrupt
	sei(); //enable global interrupts

}


void ISR_200() {
    run200 = true;
    if (ran100) {
        ran100 = false;
        run100 = true;
    }
}

int main() {
	timer1_init(); //initialize timer 1 and interrupts

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
