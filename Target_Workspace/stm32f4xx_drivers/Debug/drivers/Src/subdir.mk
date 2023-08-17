################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/Src/stm32f446xx_gpio_driver.c \
../drivers/Src/stm32f446xx_i2c_driver.c \
../drivers/Src/stm32f446xx_spi_driver.c \
../drivers/Src/stm32f446xx_usart_driver.c \
../drivers/Src/stm32f466xx_rcc_driver.c 

OBJS += \
./drivers/Src/stm32f446xx_gpio_driver.o \
./drivers/Src/stm32f446xx_i2c_driver.o \
./drivers/Src/stm32f446xx_spi_driver.o \
./drivers/Src/stm32f446xx_usart_driver.o \
./drivers/Src/stm32f466xx_rcc_driver.o 

C_DEPS += \
./drivers/Src/stm32f446xx_gpio_driver.d \
./drivers/Src/stm32f446xx_i2c_driver.d \
./drivers/Src/stm32f446xx_spi_driver.d \
./drivers/Src/stm32f446xx_usart_driver.d \
./drivers/Src/stm32f466xx_rcc_driver.d 


# Each subdirectory must supply rules for building sources it contributes
drivers/Src/%.o drivers/Src/%.su: ../drivers/Src/%.c drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -c -I../Inc -I"/Users/renatosoriano/FastBit_Embedded_Courses_Udemy/Course3_Workspace/My_workspace/target/stm32f4xx_drivers/bsp" -I"/Users/renatosoriano/FastBit_Embedded_Courses_Udemy/Course3_Workspace/My_workspace/target/stm32f4xx_drivers/drivers/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-drivers-2f-Src

clean-drivers-2f-Src:
	-$(RM) ./drivers/Src/stm32f446xx_gpio_driver.d ./drivers/Src/stm32f446xx_gpio_driver.o ./drivers/Src/stm32f446xx_gpio_driver.su ./drivers/Src/stm32f446xx_i2c_driver.d ./drivers/Src/stm32f446xx_i2c_driver.o ./drivers/Src/stm32f446xx_i2c_driver.su ./drivers/Src/stm32f446xx_spi_driver.d ./drivers/Src/stm32f446xx_spi_driver.o ./drivers/Src/stm32f446xx_spi_driver.su ./drivers/Src/stm32f446xx_usart_driver.d ./drivers/Src/stm32f446xx_usart_driver.o ./drivers/Src/stm32f446xx_usart_driver.su ./drivers/Src/stm32f466xx_rcc_driver.d ./drivers/Src/stm32f466xx_rcc_driver.o ./drivers/Src/stm32f466xx_rcc_driver.su

.PHONY: clean-drivers-2f-Src

