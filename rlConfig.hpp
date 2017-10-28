/***********************************
Rear left CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Strain gauges (qauntity = 6) - 1000 Hz
Shock Pot LF - 1000 Hz
**************************************/

//defining CAN messages
static constexpr CANmessage CAN1 = { CAN identifier, sendcanMOB0, 4}; //sg3:0 
static constexpr CANmessage CAN2 = { CAN identifier, sendcanMOB1, 3}; //sg5:4, shock pot
static constexpr CANMessage message1000[] = { CAN1, CAN2 };
static constexpr uint8_t message1000length = 2; 
