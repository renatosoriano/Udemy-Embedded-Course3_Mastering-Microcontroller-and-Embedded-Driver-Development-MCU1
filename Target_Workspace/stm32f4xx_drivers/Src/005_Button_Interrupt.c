/*
 * 005_Button_Interrupt.c
 *
 *  Created on: Jul 20, 2023
 *      Author: renatosoriano
 */


#include <string.h>
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

    memset(&GpioLed, 0, sizeof(GpioLed));
    memset(&GpioBtn, 0, sizeof(GpioBtn));

	//this is led gpio configuration
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);

    GPIO_Init(&GpioLed);

	//this is btn gpio configuration
    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;

    GPIO_PeriClockControl(GPIOC, ENABLE);

    GPIO_Init(&GpioBtn);

    GPIO_WriteToOutputPin(GPIOA,GPIO_PIN_NO_5,GPIO_PIN_RESET);

    /* IRQ configuration */
    GPIO_IRQPriorityConfig(IRQ_NO_EXIT15_10, NVIC_IRQ_PRI15);
    GPIO_IRQInterruptConfig(IRQ_NO_EXIT15_10, ENABLE);

    while(1);

    return 0;
}

void EXTI15_10_IRQHandler(void)
{
	delay();
    GPIO_IRQHandling(GPIO_PIN_NO_13);
    GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
}
