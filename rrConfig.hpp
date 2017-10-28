/***********************************
Rear right CAN board configuration
Created by: Mitchell Myjak
Date created: 10/23/17
Date last edited: 10/23/17

Rear accelerometer - 250 Hz
Shock Pot RF - 1000 Hz
**************************************/

//defining CAN messages
static constexpr CANmessage CAN1 = { CAN identifier, sendcanMOB0 }; //ra
static constexpr CANmessage CAN2 = { CAN identifier, sendcanMOB1 }; //sp
static constexpr CANMessage message1000[] = { CAN1 };
static constexpr CANMessage message250[] = { CAN2 };
static constexpr uint8_t message1000length = 1; 
static constexpr uint8_t message250length = 1;

