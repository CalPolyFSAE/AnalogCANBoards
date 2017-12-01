/*
 * main.cpp
 *
 *  Created on: Apr 5, 2015
 *      Author: thomaswillson
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "SensorManager.h"

//#include <AVRLibrary/arduino/Arduino.h>
//#include <AVRLibrary/CPFECANLib.h>

void timer1_init() {
    TCCR1B |= (1 << WGM12) | (1 << CS11); //set timer 1 to CTC mode and prescaler 8
    TCNT1 = 0; //initialize counter
    OCR1A = 1999; //This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
    TIMSK1 |= (1 << OCIE1A); //enable compare interrupt
}

//interrupt function for timing
SIGNAL(TIMER1_COMPA_vect) {
    SensorManager::INT_UpdateTiming();
}

//interrupt function for ADC completion
ISR(ADC_vect)
{
    ADCManager::INT_ADCFinished();
}

int main() {
    cli();
    timer1_init();
    ADCManager::Init();
    SensorManager::Init();
    sei();
}
