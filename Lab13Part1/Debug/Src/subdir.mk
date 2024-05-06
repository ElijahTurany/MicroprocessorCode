################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/fpu.c \
../Src/gen_bluetooth_driver.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c \
../Src/timebase.c 

OBJS += \
./Src/fpu.o \
./Src/gen_bluetooth_driver.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o \
./Src/timebase.o 

C_DEPS += \
./Src/fpu.d \
./Src/gen_bluetooth_driver.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d \
./Src/timebase.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32L4 -DSTM32 -DSTM32L476RGTx -DNUCLEO_L476RG -DSTM32L476xx -c -I../Inc -I"C:/Users/TuranyElijah/OneDrive - University of Wisconsin-Stout/Documents/School/2024Spring/Microproccessor/Headers/CMSIS/Device/ST/STM32L4xx/Include" -I"C:/Users/TuranyElijah/OneDrive - University of Wisconsin-Stout/Documents/School/2024Spring/Microproccessor/Headers/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/fpu.cyclo ./Src/fpu.d ./Src/fpu.o ./Src/fpu.su ./Src/gen_bluetooth_driver.cyclo ./Src/gen_bluetooth_driver.d ./Src/gen_bluetooth_driver.o ./Src/gen_bluetooth_driver.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su ./Src/timebase.cyclo ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su

.PHONY: clean-Src

