/*
 * 010_I2C_MasterTx.c
 *
 *  Created on: Jul 25, 2023
 *      Author: renatosoriano
 */

#include <stdio.h>
#include <string.h>
#include <stm32f446xx.h>

#define MY_ADDR         0x61;
#define SLAVE_ADDR      0x68

void delay(void)
{
    for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

I2C_Handle_t I2C1Handle;

/* Test data */
uint8_t some_data[] = "We are testing I2C master Tx\n";

/*
 * PB6 -> SCL
 * PB7 or PB9 -> SDA
 */
void I2C1_GPIOInits(void)
{
    GPIO_Handle_t I2CPins;

	/* Note : Internal pull-up resistors are used */

    I2CPins.pGPIOx = GPIOB;
    I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	/*
	 * Note : In the below line use GPIO_NO_PUPD option if you want to
	 * use external pullup resistors,then you have to use 3.3K pull up resistors
	 * for both SDA and SCL lines
	 */
    I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
    I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* SCL */
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&I2CPins);


    /* SDA */
    I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&I2CPins);
}

void I2C1_Inits(void)
{
    I2C1Handle.pI2Cx = I2C1;
    I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
    I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
    I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
    I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

    I2C_Init(&I2C1Handle);
}

void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GpioBtn;

    /* BTN GPIO Config */
    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_Init(&GpioBtn);

}


int main(void)
{

    GPIO_ButtonInit();

    /* I2C Pin initialization */
    I2C1_GPIOInits();

    /* I2C Peripheral configuration */
    I2C1_Inits();

    /* Enabling I2C peripheral */
    I2C_PeripheralControl(I2C1, ENABLE);

    while(1)
    {
        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

        /* De-bouncing protection */
        delay();

        /* Sending data to slave */
        I2C_MasterSendData(&I2C1Handle,some_data,strlen((char*)some_data),SLAVE_ADDR,I2C_ENABLE_SR);
    }
}

