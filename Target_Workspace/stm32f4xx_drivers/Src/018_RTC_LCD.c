/*
 * 018_RTC_LCD.c
 *
 *  Created on: Jul 31, 2023
 *      Author: renatosoriano
 */

#include <stdio.h>
#include "ds1307.h"
#include "lcd.h"

#define SYSTICK_TIMER_CLK   16000000UL

/* Enable this macro if you want to test RTC on LCD */
#define PRINT_LCD

/* Helper functions */
char* GetDayOfWeek(uint8_t i);
void NumberToString(uint8_t num, char *buf);
char* TimeToString(RTC_Time_t *RTC_Time);
char* DateToString(RTC_Date_t *RTC_Date);
void InitSystickTimer(uint32_t tick_hz);

static void Delay_ms(uint32_t value);


int main()
{
    RTC_Time_t current_time;
    RTC_Date_t current_date;

#ifndef PRINT_LCD
	printf("RTC test\n");
#else

    /* LCD Settings */
    LCD_Init();

    LCD_PrintString("RTC Test...");

    Delay_ms(2000);

    LCD_DisplayClear();

    LCD_DisplayReturnHome();

#endif

    if(DS1307_Init())
    {
        printf("RTC initialization has failed\n");
        while(1);
    }

    InitSystickTimer(1);

    current_date.day = THURSDAY;
    current_date.date = 16;
    current_date.month = 5;
    current_date.year = 23;

    current_time.hours = 11;
    current_time.minutes = 59;
    current_time.seconds = 45;
    current_time.time_format = TIME_FORMAT_12HRS_PM;

    DS1307_SetCurrentDate(&current_date);
    DS1307_SetCurrentTime(&current_time);

    DS1307_GetCurrentDate(&current_date);
    DS1307_GetCurrentTime(&current_time);

    char *AM_PM;
    if(current_time.time_format != TIME_FORMAT_24HRS)
    {
        AM_PM = (current_time.time_format) ? "PM" : "AM";
#ifndef PRINT_LCD
		printf("Current time = %s %s\n",TimeToString(&current_time),AM_PM); // 04:25:41 PM
#else
        LCD_PrintString(TimeToString(&current_time));
        LCD_PrintString(AM_PM);
#endif
    }
    else
    {
#ifndef PRINT_LCD
		printf("Current time = %s\n",TimeToString(&current_time)); // 04:25:41
#else
        LCD_PrintString(TimeToString(&current_time));
#endif
    }
#ifndef PRINT_LCD
	printf("Current date = %s <%s>\n",DateToString(&current_date), GetDayOfWeek(current_date.day));
#else
    LCD_SetCursor(2, 1);
    LCD_PrintString(DateToString(&current_date));
#endif

    while(1);

    return 0;
}

/* Helper functions */
char* GetDayOfWeek(uint8_t i)
{
    char* days[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    return days[i - 1];
}

void NumberToString(uint8_t num, char *buf)
{
    if(num < 10)
    {
        buf[0] = '0';
        buf[1] = num + 48;

    }
    else if(num >= 10 && num < 99)
    {
        buf[0] = (num / 10) + 48;
        buf[1] = (num % 10) + 48;
    }
}

//hh:mm:ss
char* TimeToString(RTC_Time_t *RTC_Time)
{
    static char buf[9];

    buf[2] = ':';
    buf[5] = ':';

    NumberToString(RTC_Time->hours, buf);
    NumberToString(RTC_Time->minutes, &buf[3]);
    NumberToString(RTC_Time->seconds, &buf[6]);

    buf[8] = '\0';

    return buf;
}

//dd/mm/yy
char* DateToString(RTC_Date_t *RTC_Date)
{
    static char buf[9];

    buf[2] = '/';
    buf[5] = '/';

    NumberToString(RTC_Date->date, buf);
    NumberToString(RTC_Date->month, &buf[3]);
    NumberToString(RTC_Date->year, &buf[6]);

    buf[8] = '\0';

    return buf;
}

void InitSystickTimer(uint32_t tick_hz)
{
    uint32_t *pSCSR = (uint32_t*)0xE000E010;
    uint32_t *pSRVR = (uint32_t*)0xE000E014;

    /* Calculation of reload value */
    uint32_t count_value = (SYSTICK_TIMER_CLK / tick_hz) - 1;

    /* Clear the value of SVR */
    *pSRVR &= ~(0x00FFFFFFFF);

    /* Load the value into SVR */
    *pSRVR |= count_value;

    /* Settings: Enable SysTick exception request */
    *pSCSR |= (1 << 1);

    /* Settings: Indicate processor clock source */
    *pSCSR |= (1 << 2);

    /* Settings: Enable SysTick counter */
    *pSCSR |= (1 << 0);
}

void SysTick_Handler(void)
{
    RTC_Time_t current_time;
    RTC_Date_t current_date;

    DS1307_GetCurrentTime(&current_time);

    char *AM_PM;
    if(current_time.time_format != TIME_FORMAT_24HRS)
    {
        AM_PM = (current_time.time_format) ? "PM" : "AM";
#ifndef PRINT_LCD
		printf("Current time = %s %s\n",TimeToString(&current_time),AM_PM); // 04:25:41 PM
#else
        LCD_SetCursor(1, 1);
        LCD_PrintString(TimeToString(&current_time));
        LCD_PrintString(AM_PM);
#endif
    }
    else
    {
#ifndef PRINT_LCD
		printf("Current time = %s\n",TimeToString(&current_time)); // 04:25:41
#else
        LCD_SetCursor(1, 1);
        LCD_PrintString(TimeToString(&current_time));
#endif
    }

    DS1307_GetCurrentDate(&current_date);

#ifndef PRINT_LCD
	printf("Current date = %s <%s>\n",DateToString(&current_date), GetDayOfWeek(current_date.day));
#else
    LCD_SetCursor(2, 1);
    LCD_PrintString(DateToString(&current_date));
    LCD_PrintChar('<');
    LCD_PrintString(GetDayOfWeek(current_date.day));
    LCD_PrintChar('>');
#endif

}

static void Delay_ms(uint32_t value)
{
    for(uint32_t i = 0 ; i < (value * 1000); i++);
}

