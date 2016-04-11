/*
 * Combustion-Front.hpp
 *
 *  Created on: Apr 10, 2016
 *      Author: thomaswillson
 */

#ifndef COMBUSTION_FRONT_HPP_
#define COMBUSTION_FRONT_HPP_

//Define CAN Messages
static constexpr CANMessage CAN1 = { ADC_A, VIN1, VIN3, VIN5, VIN7, 0x0C8, sendcanMOB0 };
static constexpr CANMessage CAN2 = { ADC_A, VIN8, VIN6, VIN4, VIN2, 0x0C9, sendcanMOB1 };
static constexpr CANMessage CAN3 = { ADC_B, VIN1, VIN3, VIN5, VIN7, 0x0CA, sendcanMOB2 };
static constexpr CANMessage CAN4 = { ADC_B, VIN4, VIN2, VINund, VINund, 0x0CC, sendcanMOB3 };
static constexpr CANMessage CAN5 = { ADC_C, VIN1, VIN3, VIN5, VIN7, 0x0CE, sendcanMOB5 }; //0x0CE, MOB4
static constexpr CANMessage CAN6 = { ADC_B, VIN8, VIN6, VINund, VINund, 0x0CB, sendcanMOB5 };
static constexpr CANMessage CAN7 = { ADC_C, VIN8, VIN6, VIN4, VINund, 0x0CF, sendcanMOB0 };

//Define Sample rates
static constexpr CANMessage message200[] = { CAN5, CAN7 };
static constexpr CANMessage message100[] = {  };
static constexpr uint8_t Message200length = 2; //length of message200[]
static constexpr uint8_t Message100length = 0; //length of message100[]


#endif /* COMBUSTION_FRONT_HPP_ */
