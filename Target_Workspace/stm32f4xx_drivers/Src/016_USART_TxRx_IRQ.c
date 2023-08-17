/*
 * 016_USART_TxRx_IRQ.c
 *
 *  Created on: Jul 28, 2023
 *      Author: renatosoriano
 */

#include <stdio.h>
#include <string.h>
#include <stm32f446xx.h>

/* Transmitter buffer */
char *msg[3] = {"hihihihihihi123\n", "Hello How are you ?\n" , "Today is Monday !\n"};

/* Receiver buffer */
char rx_buffer[1024];

USART_Handle_t USART3Handle;

/* Indicates reception completion */
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

void USART3_Init(void)
{
    USART3Handle.pUSARTx = USART3;
    USART3Handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
    USART3Handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    USART3Handle.USART_Config.USART_Mode = USART_MODE_TXRX;
    USART3Handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    USART3Handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    USART3Handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

    USART_Init(&USART3Handle);
}


/*
 * PC10-> TX
 * PC11-> RX
 */
void USART3_GPIOInit(void)
{
    GPIO_Handle_t USART3Pins;

    USART3Pins.pGPIOx = GPIOC;
    USART3Pins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    USART3Pins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    USART3Pins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    USART3Pins.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    USART3Pins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    /* TX */
    USART3Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    GPIO_Init(&USART3Pins);

    /* RX */
    USART3Pins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
    GPIO_Init(&USART3Pins);
}


void GPIO_ButtonInit(void)
{
    GPIO_Handle_t GpioBtn,GpioLed;

    /* BTN GPIO Config */
    GpioBtn.pGPIOx = GPIOC;
    GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_Init(&GpioBtn);

    /* LED GPIO Config */
    GpioLed.pGPIOx = GPIOA;
    GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
    GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
    GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);

    GPIO_Init(&GpioLed);
}


void delay(void)
{
    for(uint32_t i = 0 ; i < 500000/2 ; i ++);
}


int main(void)
{
    uint32_t cnt = 0;

    GPIO_ButtonInit();

    USART3_GPIOInit();

    USART3_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART3, ENABLE);

    USART_PeripheralControl(USART3, ENABLE);

    printf("Application is running\n");

    while(1)
    {
        /* Wait till button is pressed */
    	/* For NUCLEO-F446RE the button pressed state is inverse, thus no '!' in while for reading*/
        while( GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13) );

        /* De-bouncing protection */
        delay();

        /* Next message index. Value shoudn't cross 2. */
        cnt = cnt % 3;

		//First lets enable the reception in interrupt mode, this code enables the receive interrupt
        while(USART_ReceiveDataInterrupt(&USART3Handle, (uint8_t *)rx_buffer, strlen(msg[cnt])) != USART_READY);

		//Send the msg indexed by cnt in blocking mode
        USART_SendData(&USART3Handle, (uint8_t*)msg[cnt], strlen(msg[cnt]));

        printf("Transmitted : %s\n", msg[cnt]);

    	//Now lets wait until all the bytes are received from the arduino.
    	//When all the bytes are received rxCmplt will be SET in application callback
        while(rxCmplt != SET);

    	//Last byte should be null otherwise %s fails while printing
        rx_buffer[strlen(msg[cnt])+ 1] = '\0';

        printf("Received: %s\n", rx_buffer);

    	//Invalidate the flag
        rxCmplt = RESET;

    	//Move on to next message indexed in msg[]
        cnt ++;
    }

    return 0;
}

void USART3_IRQHandler(void)
{
    USART_IRQHandling(&USART3Handle);
}


void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle, uint8_t AppEvent)
{
    if(AppEvent == USART_EVENT_RX_CMPLT)
    {
        rxCmplt = SET;
    }
    else if (AppEvent == USART_EVENT_TX_CMPLT)
    {
        printf("Tx Complete\n");
    }
}

