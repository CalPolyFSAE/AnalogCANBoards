################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Commands/Command.cpp \
../Commands/CommandManager.cpp \
../Commands/Command_Test.cpp 

OBJS += \
./Commands/Command.o \
./Commands/CommandManager.o \
./Commands/Command_Test.o 

CPP_DEPS += \
./Commands/Command.d \
./Commands/CommandManager.d \
./Commands/Command_Test.d 


# Each subdirectory must supply rules for building sources it contributes
Commands/%.o: ../Commands/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -felide-constructors -funsigned-char -funsigned-bitfields -fno-exceptions -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


