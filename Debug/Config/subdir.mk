################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Config/CONFIG00.cpp 

OBJS += \
./Config/CONFIG00.o 

CPP_DEPS += \
./Config/CONFIG00.d 


# Each subdirectory must supply rules for building sources it contributes
Config/%.o: ../Config/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -I/root/share/Code/lib_mcu/can -I/root/share/Code/lib_mcu -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


