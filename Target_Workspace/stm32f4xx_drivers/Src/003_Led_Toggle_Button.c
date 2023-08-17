/*
 * 003_LedToggle_Button.c
 *
 *  Created on: Jul 19, 2023
 *      Author: renatosoriano
 */


#include "stm32f446xx.h"

#if !defined(__SOFT_FPU__) && defined(__ARM_FP)
#warning "FPU is not initialized, but the project is compiling for an FPU. Please initialize FPU before use."
#endif

// For stm32f446xx MCU
#define HIGH 1
#define LOW 0
#define BTN_PRESSED LOW

void delay(void)
{
    for(uint32_t i = 0; i < 500000/2; i++);
}

int main(void)
{
    GPIO_Handle_t GpioLed, GpioBtn;

    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);

    GPIO_Init(&GpioLed);

    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_PeriClockControl(GPIOC, ENABLE);

    GPIO_Init(&GpioBtn);

    while(1)
    {
        if(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) == BTN_PRESSED)
        {
            delay();
            GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
        }
    }

    return 0;
}
