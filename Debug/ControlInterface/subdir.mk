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
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -I/root/share/Code/lib_mcu/can -I/root/share/Code/lib_mcu -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


