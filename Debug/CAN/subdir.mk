################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CAN/can_drv.c 

CPP_SRCS += \
../CAN/CANRXTX.cpp 

C_DEPS += \
./CAN/can_drv.d 

OBJS += \
./CAN/CANRXTX.o \
./CAN/can_drv.o 

CPP_DEPS += \
./CAN/CANRXTX.d 


# Each subdirectory must supply rules for building sources it contributes
CAN/%.o: ../CAN/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: AVR C++ Compiler'
	avr-g++ -std=gnu++11 -fpermissive -I"/root/share/Code/AVRLibrary" -I/root/share/Code/lib_mcu/can -I/root/share/Code/lib_mcu -Wall -g2 -gstabs -O0 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -funsigned-char -funsigned-bitfields -fno-exceptions -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

CAN/%.o: ../CAN/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/root/share/Code/AVRLibrary" -Wall -O3 -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -Wl,-u,vfprintf -lprintf_flt -lm -mmcu=at90can128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


