################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/018_RTC_LCD.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/018_RTC_LCD.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/018_RTC_LCD.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"/Users/renatosoriano/FastBit_Embedded_Courses_Udemy/Course3_Workspace/My_workspace/target/stm32f4xx_drivers/bsp" -I"/Users/renatosoriano/FastBit_Embedded_Courses_Udemy/Course3_Workspace/My_workspace/target/stm32f4xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/018_RTC_LCD.d ./Src/018_RTC_LCD.o ./Src/018_RTC_LCD.su ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

