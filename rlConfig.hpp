/***********************************
Rear left CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Strain gauges (qauntity = 6) - 1000 Hz
Shock Pot LF - 1000 Hz
**************************************/

//defining CAN messages
static constexpr CANmessage CAN0 = { CAN0id, sendcanMOB0, 7};
