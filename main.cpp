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

volatile bool run1000 = false;
volatile bool run250 = false;
volatile bool run50 = false;

void timer1_init(){
	TCCR1B |= (1 << WGM12)|(1 << CS11); //set timer 1 to CTC mode and prescaler 8
	TCNT1 = 0; //initialize counter
	OCR1A = 1999;//This sets interrupt to 1000Hz: [((16MHz/8)(0.001s))-1]
	TIMSK1 |= (1<<OCIE1A);//enable compare interrupt
	sei(); //enable global interrupts
}

//interrupt function
SIGNAL(TIMER1_COMPA_vect) {
   static int count = 0;
   count++;
   //run 1000 Hz update every time interrupt is called
   run1000 = true;
   //run 250 Hz update every 4th time
   if(count%4 == 0)
      run250 = true;
   else
      run250 = false;
   //run 50 Hz update every 20th time
   if(count%20 == 0)
      run50 = true;
   else
      run50 = false;
}

int main() {
	timer1_init(); //initialize timer 1 and interrupts
   SensorCANmod::initADC();
	CPFECANLib::init(CPFECANLib::CAN_BAUDRATE::B1M, nullptr); //Adjust back to B1M

    while (1) {
      if(run250) {
         run250 = false;
         SensorCANmod::updateCAN250();
      }
      if(run50) {
         run50 = false;
         SensorCANmod::updateCAN50();
      }
      if(run1000) {
         run1000 = false;
         SensorCANmod::updateCAN1000();
      }
    }
}
