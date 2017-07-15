// Key:

// ADC_A VIN1: SG_4 p25
// ADC_A VIN2: SG_5 p32
// ADC_A VIN3: SG_3 p17
// ADC_A VIN4: SG_6 p40
// ADC_A VIN5: SG_2 p10
// ADC_A VIN6: SG_7 p47
// ADC_A VIN7: SG_1 p04
// ADC_A VIN8: SG_8 p05

// ADC_B VIN1: 
// ADC_B VIN2: 
// ADC_B VIN3: 
// ADC_B VIN4: (More SGs, 1 thermo)
// ADC_B VIN5: 
// ADC_B VIN6: 
// ADC_B VIN7: 
// ADC_B VIN8: 

// ADC_C VIN1: A1	 p54
// ADC_C VIN2: A8	 p52
// ADC_C VIN3: A2	 p55
// ADC_C VIN4: A7	 p45
// ADC_C VIN5: A3	 p50
// ADC_C VIN6: A6	 p37
// ADC_C VIN7: A4	 p51
// ADC_C VIN8: A5	 p44

// ADC_D VIN1: A9	 p24
// ADC_D VIN2: A16	 p46
// ADC_D VIN3: A10	 p31
// ADC_D VIN4: A15	 p38
// ADC_D VIN5: A11	 p39
// ADC_D VIN6: A14	 p30
// ADC_D VIN7: A12	 p16
// ADC_D VIN8: A13	 p23

// ADC_E VIN1: A17	
// ADC_E VIN2: A24	 p02
// ADC_E VIN3: A18
// ADC_E VIN4: A23	 p03
// ADC_E VIN5: A19
// ADC_E VIN6: A22	 p07
// ADC_E VIN7: A20	 p14
// ADC_E VIN8: A21	 p08


//Define CAN Messages
static constexpr CANMessage CAN1 = { ADC_A, VIN7, VIN1, VIN5, VIN2, 0x0D0, sendcanMOB0 };			// Strain gauges 1, 2, 3, 4
static constexpr CANMessage CAN2 = { ADC_A, VIN4, VIN2, VINund, VINund, 0x0D1, sendcanMOB1 };		// Strain gauges 5, 6
static constexpr CANMessage CAN3 = { ADC_D, VIN8, VIN1, VIN7, VINund, 0x0D2, sendcanMOB2 };			// Accel X, Y, Z
static constexpr CANMessage CAN4 = { ADC_E, VIN2, VIN4, VINund, VINund, 0xD3, sendcanMOB3 };		// Shock pot RL, RR
//static constexpr CANMessage CAN3 = { ADC_A, VIN1, VIN3, VIN5, VIN7, 0x0D2, sendcanMOB2 }; //Strain Gauges
//static constexpr CANMessage CAN4 = { ADC_B, VIN4, VIN2, VINund, VINund, 0x0CC, sendcanMOB3 };
//static constexpr CANMessage CAN5 = { ADC_C, VIN1, VIN3, VIN5, VIN7, 0x0CE, sendcanMOB5 }; //0x0CE, MOB4
//static constexpr CANMessage CAN6 = { ADC_B, VIN8, VIN6, VINund, VINund, 0x0CB, sendcanMOB5 };
//static constexpr CANMessage CAN7 = { ADC_C, VIN8, VIN6, VIN4, VINund, 0x0CF, sendcanMOB0 };

//Define Sample rates
static constexpr CANMessage message200[] = { CAN1, CAN2 };
static constexpr CANMessage message100[] = {  };
static constexpr uint8_t Message200length = 2; //length of message200[]
static constexpr uint8_t Message100length = 0; //length of message100[]
