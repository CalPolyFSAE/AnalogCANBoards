/***********************************
Front left CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Strain gauges (quantity = 6) - 1000 Hz
Shock Pot LF - 1000 Hz
**************************************/

//defining CAN messages
//need to change CAN identifier to proper can msgID
static constexpr CANmessage CAN0 = { 0x0A0, sendcanMOB0, 7}; 

