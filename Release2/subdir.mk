################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ADCManager.cpp \
../CANLib.cpp \
../CANSensorTimer.cpp \
../Sensor.cpp \
../SensorManager.cpp \
../main.cpp 

OBJS += \
./ADCManager.o \
./CANLib.o \
./CANSensorTimer.o \
./Sensor.o \
./SensorManager.o \
./main.o 

CPP_DEPS += \
./ADCManager.d \
./CANLib.d \
./CANSensorTimer.d \
./Sensor.d \
./SensorManager.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -DDEBUG -Wall -g2 -gstabs -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -felide-constructors -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


