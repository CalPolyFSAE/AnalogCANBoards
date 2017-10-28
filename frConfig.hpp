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
static constexpr CANmessage CAN1 = { CAN identifier, sendcanMOB0, 1}; //sp 
static constexpr CANmessage CAN2 = { CAN identifier, sendcanMOB1, 3}; //fbp, rbp, fa
static constexpr CANmessage CAN3 = { CAN identifier, sendcanMOB2, 1}; //spp
static constexpr CANMessage message1000[] = { CAN1 };
static constexpr CANMessage message250[] = { CAN2 };
static constexpr CANMessage message50[]= { CAN3 };
static constexpr uint8_t message1000length = 1; 
static constexpr uint8_t message250length = 1;
static constexpr uint8_t message50length = 1;

