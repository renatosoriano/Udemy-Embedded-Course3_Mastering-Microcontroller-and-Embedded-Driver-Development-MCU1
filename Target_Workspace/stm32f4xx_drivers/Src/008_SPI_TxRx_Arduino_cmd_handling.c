/*
 * 008_SPI_TxRx_Arduino_cmd_handling.c
 *
 *  Created on: Jul 23, 2023
 *      Author: renatosoriano
 */

#include<stdio.h>
#include <stm32f446xx.h>
#include <string.h>

/*
 * Alternate functionality
 *
 * PB15 --> SPI2_MOSI
 * PB14 --> SPI2_MISO
 * PB13 --> SPI2_SLCK
 * PB12 --> SPI2_NSS
 * ALT function mode: AF5
 */

/* Command codes */
#define COMMAND_LED_CTRL        0x50
#define COMMAND_SENSOR_READ     0x51
#define COMMAND_LED_READ        0x52
#define COMMAND_PRINT           0x53
#define COMMAND_ID_READ         0x54

#define LED_ON      1
#define LED_OFF     0

/* Analog pins */
#define ANALOG_PIN0     0
#define ANALOG_PIN1     1
#define ANALOG_PIN2     2
#define ANALOG_PIN3     3
#define ANALOG_PIN4     4

/* Arduino LED */
#define LED_PIN         9

void delay(void)
{
	for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}

/*
 * PB14 --> SPI2_MISO
 * PB15 --> SPI2_MOSI
 * PB13 -> SPI2_SCLK
 * PB12 --> SPI2_NSS
 * ALT function mode : 5
 */

void SPI2_GPIOInits(void)
{
    GPIO_Handle_t SPIPins;

    SPIPins.pGPIOx = GPIOB;
    SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
    SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* SCLK Init */
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GPIO_Init(&SPIPins);

    /* MOSI Init */
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
    GPIO_Init(&SPIPins);

    /* MISO Init */
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
    GPIO_Init(&SPIPins);

    /* NSS Init */
    SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
    GPIO_Init(&SPIPins);
}


void SPI2_Inits(void)
{
    SPI_Handle_t SPIHandle;

    SPIHandle.pSPIx = SPI2;
    SPIHandle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
    SPIHandle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
    SPIHandle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; //2MHz
    SPIHandle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
    SPIHandle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
    SPIHandle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
    SPIHandle.SPIConfig.SPI_SSM = SPI_SSM_DI; //HW Slave management enabled for NSS pin

    SPI_Init(&SPIHandle);
}


void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GpioBtn;

    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_PeriClockControl(GPIOC, ENABLE);

    GPIO_Init(&GpioBtn);
}

uint8_t SPI_VerifyResponse(uint8_t ackByte)
{
    if(ackByte == (uint8_t)0xF5)
    {
        return 1;
    }

    return 0;
}


int main(void)
{
    uint8_t dummyWrite = 0xFF;
    uint8_t dummyRead;

	printf("Application is running\n");

    /* Initialize button */
    GPIO_ButtonInit();

    /* Initialize GPIO pins to behave as SPI2 pins */
    SPI2_GPIOInits();

    /* Initialize SPI2 peripheral parameters */
    SPI2_Inits();

	printf("SPI Init. done\n");

    /*
     * Making SSOE 1 does NSS output enable.
     * The NSS pin is automatically managed by hardware.
     * i.e. when SPE=1, NSS will be pulled to LOW and
     * NSS pin will be HIGH when SPE=0
     */
    SPI_SSOEConfig(SPI2, ENABLE);

    while(1)
    {
        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

        printf("\nSPI communication started!\n");

        /* 200ms delay */
        delay();

        /* Enable SPI2 peripheral */
        SPI_PeripheralControl(SPI2, ENABLE);

	    //1. CMD_LED_CTRL  	<pin no(1)>     <value(1)>

        /* Send SPI data: CMD LED Control */
        uint8_t commandCode = COMMAND_LED_CTRL;

        /* Send command */
        SPI_SendData(SPI2, &commandCode, 1);

        /* Dummy read to clear off the RXNE */
        SPI_ReceiveData(SPI2, &dummyRead, 1);

        /* Send dummy bits (byte) to fetch the response from slave */
        SPI_SendData(SPI2, &dummyWrite, 1);

        /* Receive Acknowledgment byte */
        uint8_t ackByte;
        SPI_ReceiveData(SPI2, &ackByte, 1);

        /* Verify response from SPI slave */
        uint8_t args[2];
        if( SPI_VerifyResponse(ackByte) )
        {
            /* Send arguments */
            args[0] = LED_PIN;
            args[1] = LED_ON;
            SPI_SendData(SPI2, args, 2);

            printf("COMMAND LED executed!\n");
        }
        /* End of CMD LED Control */

		//2. CMD_SENSOR_READ   <analog pin number(1) >

        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

        /* Send SPI data: CMD Sensor Read */
        commandCode = COMMAND_SENSOR_READ;

        /* Send command */
        SPI_SendData(SPI2, &commandCode, 1);

        /* Dummy read to clear off the RXNE */
        SPI_ReceiveData(SPI2, &dummyRead, 1);

        /* Send dummy bits (byte) to fetch the response from slave */
        SPI_SendData(SPI2, &dummyWrite, 1);

        /* Receive Acknowledgment byte */
        SPI_ReceiveData(SPI2, &ackByte, 1);

        /* Verify response from SPI slave */
        if( SPI_VerifyResponse(ackByte) )
        {
            /* Send arguments */
            args[0] = ANALOG_PIN0;
            SPI_SendData(SPI2, args, 1);
            printf("COMMAND Sensor Read executed!\n");

            /* Dummy read to clear off the RXNE */
            SPI_ReceiveData(SPI2, &dummyRead, 1);

            /* Wait for Slave to be ready with data */
            delay();

            /* Send dummy bits (byte) to fetch the response from slave */
            SPI_SendData(SPI2, &dummyWrite, 1);

            /* Receive Sensor Data from Slave */
            uint8_t analogRead;
            SPI_ReceiveData(SPI2, &analogRead, 1);
            printf("Analog read value: %d\n",analogRead);
        }
        /* End of CMD Sensor Read */

		//3.  CMD_LED_READ 	 <pin no(1) >

        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

        /* Send SPI data: CMD LED Read */
        commandCode = COMMAND_LED_READ;

        /* Send command */
        SPI_SendData(SPI2, &commandCode, 1);

        /* Dummy read to clear off the RXNE */
        SPI_ReceiveData(SPI2, &dummyRead, 1);

        /* Send dummy bits (byte) to fetch the response from slave */
        SPI_SendData(SPI2, &dummyWrite, 1);

        /* Receive Acknowledgment byte */
        SPI_ReceiveData(SPI2, &ackByte, 1);

        /* Verify response from SPI slave */
        if( SPI_VerifyResponse(ackByte) )
        {
            /* Send arguments */
            args[0] = LED_PIN;
            SPI_SendData(SPI2, args, 1);
            printf("COMMAND Read LED executed!\n");

            /* Dummy read to clear off the RXNE */
            SPI_ReceiveData(SPI2, &dummyRead, 1);

            /* Wait for Slave to be ready with data */
            delay();

            /* Send dummy bits (byte) to fetch the response from slave */
            SPI_SendData(SPI2, &dummyWrite, 1);

            /* Receive Sensor Data from Slave */
            uint8_t led_status;
            SPI_ReceiveData(SPI2, &led_status, 1);
            printf("Read LED: %d\n",led_status);
        }
        /* End of CMD LED Read */

		//4. CMD_PRINT 		<len(2)>  <message(len) >

        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

        /* Send SPI data: CMD Print */
        commandCode = COMMAND_PRINT;

        /* Send command */
        SPI_SendData(SPI2, &commandCode, 1);

        /* Dummy read to clear off the RXNE */
        SPI_ReceiveData(SPI2, &dummyRead, 1);

        /* Send dummy bits (byte) to fetch the response from slave */
        SPI_SendData(SPI2, &dummyWrite, 1);

        /* Receive Acknowledgment byte */
        SPI_ReceiveData(SPI2, &ackByte, 1);

		uint8_t message[] = "Hello ! How are you ??";

        /* Verify response from SPI slave */
        if( SPI_VerifyResponse(ackByte) )
        {
            /* Send arguments */
			args[0] = strlen((char*)message);
            SPI_SendData(SPI2, args, 1);

            /* Dummy read to clear off the RXNE */
            SPI_ReceiveData(SPI2, &dummyRead, 1);

            /* Wait for Slave to be ready with data */
            delay();

			/* Send message */
			for(int i = 0 ; i < args[0] ; i++){
				SPI_SendData(SPI2,&message[i],1);
				SPI_ReceiveData(SPI2,&dummyRead,1);
			}

            printf("COMMAND Print executed!\n");
        }
        /* End of CMD Print */

		//5. CMD_ID_READ

        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

		//to avoid button de-bouncing related issues 200ms of delay
		delay();

        /* Send SPI data: CMD ID Read */
        commandCode = COMMAND_ID_READ;

        /* Send command */
        SPI_SendData(SPI2, &commandCode, 1);

        /* Dummy read to clear off the RXNE */
        SPI_ReceiveData(SPI2, &dummyRead, 1);

        /* Send dummy bits (byte) to fetch the response from slave */
        SPI_SendData(SPI2, &dummyWrite, 1);

        /* Receive Acknowledgment byte */
        SPI_ReceiveData(SPI2, &ackByte, 1);

		uint8_t id[11];
		uint32_t i=0;

        /* Verify response from SPI slave */
        if( SPI_VerifyResponse(ackByte) )
        {
			/* Read 10 bytes id from the slave */
			for(  i = 0 ; i < 10 ; i++){
		        /* Send dummy bits (byte) to fetch the response from slave */
				SPI_SendData(SPI2,&dummyWrite,1);
				SPI_ReceiveData(SPI2,&id[i],1);
			}

			id[10] = '\0';

			printf("COMMAND ID : %s \n",id);
        }
        /* End of CMD ID Read */

        /* Confirm SPI2 not busy */
		while( SPI_GetFlagStatus(SPI2,SPI_FLAG_BUSY) );

        /* Disable SPI2 peripheral */
        SPI_PeripheralControl(SPI2, DISABLE);

        printf("\nSPI communication closed!\n");
    }

    return 0;
}
