#include "AnalogCANboard.hpp"

// Any variables modified by an ISR MUST be declared volatile.
// Compilers like to assume variables don't change between lines
// of code but if an ISR triggers, that assumption is invalid.
// Volatile tells the compiler to reload the variable from memory
// for every line of code.

//do updates
volatile bool run1000 = false;
volatile bool run250 = false;
volatile bool run50 = false;

constexpr SensorCANmod::CANMessage SensorCANmod::CAN0;
constexpr SensorCANmod::CANMessage SensorCANmod::CAN1;
constexpr SensorCANmod::CANMessage SensorCANmod::CAN2;

// Seem to need a reference to at least one of these variables to make it build.
constexpr const SensorCANmod::CANMessage* const SensorCANmod::message1000[];
constexpr const SensorCANmod::CANMessage* const SensorCANmod::message250[];
constexpr const SensorCANmod::CANMessage* const SensorCANmod::message50[];

//////PROTECTED/////////

static SensorCANmod::CANChannelLengths CANChannelLen = 0;

void SensorCANmod::initADC() {
    ADCSRA = 0x87; //Turn On ADC and set prescaler (CLK/128)
    ADCSRB = 0x00; //turn off autotrigger
    ADMUX = 0x00;     //Set ADC channel ADC0
}

//////////PUBLIC////////////

void SensorCANmod::init() {
    initADC ();

    //setup arrays that contain CAN channel mapping
    //count number of active channels
    for (uint8_t i = 0; i < CANMAXCHANNELS; ++i)
    {
        //set the number of channels is each CAN message
        //to the largest index of active channel
        if (CAN0.channels[i] != ADC_chan::ADCNULL)
        {
            CANChannelLen[0] = i;
        }
        if (CAN1.channels[i] != ADC_chan::ADCNULL)
        {
            CANChannelLen[1] = i;
        }
        if (CAN2.channels[i] != ADC_chan::ADCNULL)
        {
            CANChannelLen[2] = i;
        }
    }
}

void SensorCANmod::update()
{
    if (run1000)
    {
        updateCAN (message1000, Message1000length);
        run1000 = false;
    }
    if (run250)
    {
        updateCAN (message250, Message250length);
        run250 = false;
    }
    if (run50)
    {
        updateCAN (message50, Message50length);
        run50 = false;
    }
}

void SensorCANmod::updateCAN( const CANMessage* const Messages[], uint8_t MessagesLength ) {
    CANMessageData messageData =
        { 0, 0, 0, 0 };

    //access structs as arrays of data
    uint16_t* data = &(messageData.a);
    uint8_t* channelLengths = &(CANChannelLen.CAN0ChannelsLength);

    //send CAN messages
    for (uint8_t i = 0; i < MessagesLength; ++i)
    {
        for (uint8_t dataCount = 0; dataCount < channelLengths[i]; ++dataCount)
        {
            if (Messages[i]->channels[dataCount] != ADC_chan::ADCNULL)
                data[dataCount] = read ((uint8_t) (Messages[i]->channels[dataCount]));
        }
        txCAN (Messages[i]->msgId, &messageData, channelLengths[i], Messages[i]->MOB);
    }
}

uint16_t SensorCANmod::read( uint8_t ADCnum ) {
    setADMUX (ADCnum);
    int voltage;
    int calibratedVoltage;
    ADCSRA = 0xC7;
    while (ADCSRA & (1 << ADSC))
        ;
    voltage = ADC & 0x3FF;
    calibratedVoltage = voltage + ((0.014 * voltage) - 13.8);
    return calibratedVoltage;
}

void SensorCANmod::setADMUX( uint8_t ADCnum ) {
    switch (ADCnum) {
        case 0:
            ADMUX &= ~((MUX2 << 1) | (MUX1 << 1) | (MUX0 << 1));
            break;
        case 1:
            ADMUX |= (MUX0 << 1);
            ADMUX &= ~((MUX2 << 1) | (MUX1 << 1));
            break;
        case 2:
            ADMUX |= (MUX1 << 1);
            ADMUX &= ~((MUX2 << 1) | (MUX1 << 1));
            break;
        case 3:
            ADMUX |= (MUX1 << 1) | (MUX0 << 1);
            ADMUX &= ~(MUX2 << 1);
            break;
        case 4:
            ADMUX |= (MUX2 << 1);
            ADMUX &= ~((MUX1 << 1) | (MUX0 << 1));
            break;
        case 5:
            ADMUX |= (MUX2 << 1) | (MUX0 << 1);
            ADMUX &= ~(MUX1 << 1);
            break;
        case 6:
            ADMUX |= (MUX2 << 1) | (MUX1 << 1);
            ADMUX &= ~(MUX0 << 1);
            break;
        case 7:
            ADMUX |= (MUX2 << 1) | (MUX1 << 1) | (MUX0 << 1);
            break;
    }
}

//TODO: sort out constants
void txCAN( uint16_t ID, void *data, uint8_t dlc, uint8_t MOB ) {
    CPFECANLib::MSG msg; //comes from CPFECANLib.h in AVR library
    msg.identifier.standard = ID; //set for standard.  for extended use identifier.extended
    msg.data = (uint8_t *) data;
    msg.dlc = dlc; //Number of bytes of data
    msg.ide = 0; //Set to 0 for standard identifier.  Set to 1 for extended address
    msg.rtr = 0;
    CPFECANLib::sendMsgUsingMOB (MOB, &msg);
}

