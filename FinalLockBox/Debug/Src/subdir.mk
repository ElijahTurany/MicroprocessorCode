################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/adc.c \
../Src/fpu.c \
../Src/keypad.c \
../Src/lcd.c \
../Src/main.c \
../Src/servo.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/uart.c 

OBJS += \
./Src/adc.o \
./Src/fpu.o \
./Src/keypad.o \
./Src/lcd.o \
./Src/main.o \
./Src/servo.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/uart.o 

C_DEPS += \
./Src/adc.d \
./Src/fpu.d \
./Src/keypad.d \
./Src/lcd.d \
./Src/main.d \
./Src/servo.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DSTM32L476RGTx -DNUCLEO_L476RG -DSTM32L476xx -c -I../Inc -I"C:/Users/TuranyElijah/OneDrive - University of Wisconsin-Stout/Documents/School/2024Spring/Microproccessor/Headers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Users/TuranyElijah/OneDrive - University of Wisconsin-Stout/Documents/School/2024Spring/Microproccessor/Headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/adc.cyclo ./Src/adc.d ./Src/adc.o ./Src/adc.su ./Src/fpu.cyclo ./Src/fpu.d ./Src/fpu.o ./Src/fpu.su ./Src/keypad.cyclo ./Src/keypad.d ./Src/keypad.o ./Src/keypad.su ./Src/lcd.cyclo ./Src/lcd.d ./Src/lcd.o ./Src/lcd.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/servo.cyclo ./Src/servo.d ./Src/servo.o ./Src/servo.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/uart.cyclo ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

