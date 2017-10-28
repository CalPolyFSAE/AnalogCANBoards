/***********************************
Rear right CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Shock Pot RF - 1000 Hz
Rear accelerometer - 250 Hz
**************************************/

//defining CAN messages
static constexpr CANmessage CAN0 = { CAN0id, sendcanMOB0, 1}; //sp
static constexpr CANmessage CAN1 = { CAN1id, sendcanMOB1, 1}; //ra

