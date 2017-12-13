################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ControlInterface/CANCMDExecution.cpp \
../ControlInterface/CANCommand.cpp 

OBJS += \
./ControlInterface/CANCMDExecution.o \
./ControlInterface/CANCommand.o 

CPP_DEPS += \
./ControlInterface/CANCMDExecution.d \
./ControlInterface/CANCommand.d 


# Each subdirectory must supply rules for building sources it contributes
ControlInterface/%.o: ../ControlInterface/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


