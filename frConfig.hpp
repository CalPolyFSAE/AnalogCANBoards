/***********************************
Front right CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Shock Pot RF - 1000 Hz
Front Brake Pressure - 250 Hz
Rear Brake Pressure - 250 Hz
Front accelerometer - 250 Hz
Steering Position Pot - 50 Hz
**************************************/

//defining CAN messages
static constexpr CANmessage CAN1 = { CAN0id, sendcanMOB0, 1}; //sp 
static constexpr CANmessage CAN2 = { CAN1id, sendcanMOB1, 3}; //fbp, rbp, fa
static constexpr CANmessage CAN3 = { CAN2id, sendcanMOB2, 1}; //spp

