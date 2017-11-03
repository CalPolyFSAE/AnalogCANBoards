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

void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
    sei();
    //enable global interrupts
}

//interrupt function
SIGNAL(TIMER1_COMPA_vect) {
    static int count = 0;
    count++;
    //run 1000 Hz update every time interrupt is called
    SensorCANmod::run1000 = true;
    //run 250 Hz update every 4th time
    if (count % 4 == 0)
        SensorCANmod::run250 = true;
    //run 50 Hz update every 20th time
    if (count % 20 == 0)
        SensorCANmod::run50 = true;
}

int main() {
    timer1_init (); //initialize timer 1 and interrupts
    CPFECANLib::init (CPFECANLib::CAN_BAUDRATE::B1M, nullptr); //Adjust back to B1M
    SensorCANmod::init ();

    while (1)
    {
        SensorCANmod::update ();
    }
}
