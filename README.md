
# Mastering Microcontroller and Embedded Driver Development (MCU1)

This repo contains notes and programming assignments for the Udemy's "[Mastering Microcontroller and Embedded Driver Development (MCU1)](https://www.udemy.com/course/mastering-microcontroller-with-peripheral-driver-development/)" course by FastBit Embedded Brain Academy.

Date: August, 2019.
Date Updated (STM32CubeIDE): July, 2023.

- The course is instructed by Engineer Kiran Nayak.

- The [**Certificate**](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Certificate.pdf) is available. 

- The [**Peripheral Drivers (GPIO, I2C, SPI, USART, RCC)**](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/tree/main/Target_Workspace/stm32f4xx_drivers/drivers) for STM32f446 Nucleo board are available.

- The [**Board Support Package (BSP)**](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/tree/main/Target_Workspace/stm32f4xx_drivers/Src) for LCD and DS1307 RTC module is available. 

- The [**Coding Exercises**](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/tree/main/Target_Workspace/stm32f4xx_drivers/Src) are available. 

- The [**Exercises Circuits and Outputs**](#exercises-circuits-and-outputs) are available.  

## Descriptions

This course demystifies the internal working of the Microcontroller and its Peripherals by coding step by step and developing software drivers entirely from scratch extracting maximum information from Datasheets, Reference manuals, Specs, etc. Also protocol decoding using logic analyzers debugging, testing along with hints and tips.
Will write peripheral drivers for most commonly used peripherals such as GPIOs, I2C, SPI, USART, etc., from scratch, as well as Drivers for LCD and DS1307 I2C RTC module.

## Requirements

**[STM32 Nucleo-F446RE Development Board](https://www.st.com/en/evaluation-tools/nucleo-f446re.html#overview)** - Board used in this course but any board with Arm Cortex-M0/3/4 core will work, just modifying the target board and configuring with the respective datasheet. \
**[Eclipse-based STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html)** - C/C++ development platform with peripheral configuration, code generation, code compilation, and debug features for STM32 microcontrollers and microprocessors. Works on Windows/Linux/Mac and is free.

## Notes
* #### FPU warning fix
    Right click on the project -> properties -> expand C/C++ build -> Settings -> Tool settings -> MCU settings
  * `Floating-point unit: None`
  * `Floating-point ABI: Software implementation ( -mfloat-abi=soft )`

![FPU_warning.png](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/FPU_warning.png)

* #### Setting up SWV ITM Data Console

Open *syscalls.c* file and paste following code bellow *Includes*

```c
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//           Implementation of printf like feature using ARM Cortex M3/M4/ ITM functionality
//           This function will not work for ARM Cortex M0/M0+
//           If you are using Cortex M0, then you can use semihosting feature of openOCD
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//Debug Exception and Monitor Control Register base address
#define DEMCR                   *((volatile uint32_t*) 0xE000EDFCU )

/* ITM register addresses */
#define ITM_STIMULUS_PORT0   	*((volatile uint32_t*) 0xE0000000 )
#define ITM_TRACE_EN          	*((volatile uint32_t*) 0xE0000E00 )

void ITM_SendChar(uint8_t ch)
{

	//Enable TRCENA
	DEMCR |= ( 1 << 24);

	//enable stimulus port 0
	ITM_TRACE_EN |= ( 1 << 0);

	// read FIFO status in bit [0]:
	while(!(ITM_STIMULUS_PORT0 & 1));

	//Write to ITM stimulus port0
	ITM_STIMULUS_PORT0 = ch;
}
```


After that find function *_write* and replace `__io_putchar(*ptr++)` with `ITM_SendChar(*ptr++)` like in code snippet below
```c
__attribute__((weak)) int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for (DataIdx = 0; DataIdx < len; DataIdx++)
	{
		//__io_putchar(*ptr++);
		ITM_SendChar(*ptr++);
	}
	return len;
}
```

After these steps navigate to Debug configuration and check `Serial Wire Viewer (SWV)` check box like on snapshot below

![Debugger.png](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/Debugger.png)

Once you enter *Debug* mode, go to `Window -> Show View -> SWV -> Select SWV ITM Data Console`. On this way `ITM Data Console` will be shown in *Debug* session.


In `SWV ITM Data Console Settings` in section `ITM Stimulus Ports` enable port 0, so that you can see `printf` data.


## Exercises Circuits and Outputs

### SPI Circuit:

![SPI_Testing_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/SPI_Testing_Circuit.jpg)

![SPI_Real_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/SPI_Real_Circuit.jpg)

### I2C Circuit:

![I2C_Testing_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/I2C_Testing_Circuit.jpg)

![I2C_Real_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/I2C_Real_Circuit.jpg)

### USART Circuit:

![USART_Testing_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/USART_Testing_Circuit.jpg)

![USART_Real_Circuit.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/USART_Real_Circuit.jpg)

### RTC and LCD Circuit:

`(Changed GPIOD to GPIOC, pins remain the same).`

![RTC_LCD_interfacing.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/RTC_LCD_interfacing.jpg)

![RTC_LCD_Real_circuit1.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/RTC_LCD_Real_circuit1.jpg)

![RTC_LCD_Real_circuit2.jpg](https://github.com/renatosoriano/Udemy-Embedded-Course3_Mastering-Microcontroller-and-Embedded-Driver-Development-MCU1/blob/main/Images/RTC_LCD_Real_circuit2.jpg)


