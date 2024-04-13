#include "state_machine.h"
#include "ILI9341_STM32_Driver.h"
#include "ILI9341_GFX.h"
#include "main.h"
#include "DS3231.h"
#include "stdio.h"
#include"button.h"
extern BUTTON_HandleTypeDef bien_nut_bam;

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim2;
//extern read_after;
//extern char rotation;
 char buffer[30];
 char status[10];
 volatile int32_t read_before=0;
 volatile int32_t read_after;
 volatile int32_t rotation;
TIME_t time;
TIME_ALARM_t alarm;

uint8_t button_handle(void)
{
	uint16_t timer1 = HAL_GetTick();
	if (HAL_GetTick()-timer1>=100)
	{
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_3)==0)
		{
			return 0;
		}
		else
			return 1;

		timer1=HAL_GetTick();
	}
}
void Read_Alaram(void)
{
	uint8_t get_alaram_1[3];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x07, 1, get_alaram_1, 3, 1000);
	alarm.seconds_alaram_1 = bcdToDec(get_alaram_1[0]);
	alarm.minutes_alaram_1 = bcdToDec(get_alaram_1[1]);
	alarm.hour_alaram_1 = bcdToDec(get_alaram_1[2]);

	uint8_t get_alaram_2[2];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0B, 1, get_alaram_2, 2, 1000);
	alarm.minutes_alaram_2 = bcdToDec(get_alaram_2[0]);
	alarm.hour_alaram_2 = bcdToDec(get_alaram_2[1]);

	sprintf(buffer, "%02d", alarm.hour_alaram_1); // time.minutes_alaram_1,time.seconds_alaram_1);
	ILI9341_DrawText(buffer, FONT6, 90, 125, BLACK, WHITE);
	ILI9341_DrawText(":", FONT6, 125, 125, BLACK, WHITE);
	sprintf(buffer, "%02d", alarm.minutes_alaram_1);
	ILI9341_DrawText(buffer, FONT6, 135, 125, BLACK, WHITE);
	ILI9341_DrawText(":", FONT6, 170, 125, BLACK, WHITE);
	sprintf(buffer, "%02d", alarm.seconds_alaram_1);
	ILI9341_DrawText(buffer, FONT6, 180, 125, BLACK, WHITE);

	sprintf(buffer, "%02d", alarm.hour_alaram_2); //,time.minutes_alaram_2);
	ILI9341_DrawText(buffer, FONT6, 90, 175, BLACK, WHITE);
	ILI9341_DrawText(":", FONT6, 125, 175, BLACK, WHITE);
	sprintf(buffer, "%02d", alarm.minutes_alaram_2);
	ILI9341_DrawText(buffer, FONT6, 135, 175, BLACK, WHITE);

	if (alarm.status_alaram_1 == 1)

	{
		sprintf(status, "ON");
		ILI9341_DrawText(status, FONT6, 250, 125, BLACK, WHITE);
	}
	else
	{
		sprintf(status, "OFF");
		ILI9341_DrawText(status, FONT6, 250, 125, BLACK, WHITE);
	}

	if (alarm.status_alaram_2 == 1)
	{
		sprintf(status, "ON");
		ILI9341_DrawText(status, FONT6, 250, 175, BLACK, WHITE);
	}
	else
	{
		sprintf(status, "OFF");
		ILI9341_DrawText(status, FONT6, 250, 175, BLACK, WHITE);
	}
}
void mode_default()
{
	Get_Time(&time);
	sprintf(buffer, " %02d : %02d : %02d", time.hour, time.minutes, time.seconds);
	ILI9341_DrawText(buffer, FONT6, 100, 75, RED, WHITE);
	sprintf(buffer, " %02d / %02d / 20%d", time.dayofmonth, time.month, time.year);
	ILI9341_DrawText(buffer, FONT6, 30, 30, BLACK, WHITE);
	if (time.dayofweek == 1)
	{
		sprintf(buffer, "CN");
		ILI9341_DrawText(buffer, FONT6, 15, 75, BLACK, WHITE);
	}
	else
	{
		sprintf(buffer, "Thu %d", time.dayofweek);
		ILI9341_DrawText(buffer, FONT6, 3, 75, BLACK, WHITE);
	}
}

void machine_case_0()
{
	ILI9341_DrawFilledCircle(270, 115, 5, RED);
	int32_t read_before = htim2.Instance->CNT;
	while (1)
	{
		read_after = htim2.Instance->CNT;
		rotation = read_after - read_before;
		if (rotation != 0)
			break;
	}

	{
		HAL_Delay(400);
		read_after =  htim2.Instance->CNT;//__HAL_TIM_GET_COUNTER(&htim2);
		rotation = read_after - read_before;
		if (rotation < 0)
				{
					sprintf(status, "OFF");
					ILI9341_DrawText(status, FONT6, 250, 125, BLACK, WHITE);
					alarm.status_alaram_1 = 0;
					cancel_alaram_1();
					machine_case_4();
				}
		if (rotation > 0)
		{
			sprintf(status, "   ");
			ILI9341_DrawText(status, FONT6, 250, 125, WHITE, WHITE);
			sprintf(status, "ON");
			ILI9341_DrawText(status, FONT6, 250, 125, BLACK, WHITE);
			//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E << 0, 1, (uint8_t *)1, 1, 1000);
			while (1)
			{
				BUTTON_Handle(&bien_nut_bam);
						if(bien_nut_bam.counter==2)
						{
							bien_nut_bam.counter=0;
							break;

						}

			}
			alarm.status_alaram_1 = 1;
			ILI9341_DrawFilledCircle(270, 115, 5, GREEN);
			machine_case_1();
		}

	}
}
void machine_case_1() // set alaram 1 seconds
{
	ILI9341_DrawFilledCircle(270, 115, 5, WHITE);
	ILI9341_DrawFilledCircle(195, 115, 5, RED);
	uint8_t min_set = 0; // second not mintues (mistake)
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	int32_t read_after;
	int32_t rotation;
	while (1)
	{
		read_before = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(500);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
		{
			break;
		}
		else
		{
			read_after = __HAL_TIM_GET_COUNTER(&htim2);
			rotation = read_after - read_before;
			if (rotation > 0)
			{
				min_set = min_set + 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
			if (rotation < 0)
			{
				min_set = min_set - 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
		}
		sprintf(buffer, "%02d", min_set);
		ILI9341_DrawText(buffer, FONT6, 180, 125, BLACK, WHITE);
	}
	alarm.seconds_alaram_1 = min_set;
	machine_case_2();
}
void machine_case_2() // set alaram 1 mintues
{
	ILI9341_DrawFilledCircle(195, 115, 5, WHITE);
	ILI9341_DrawFilledCircle(150, 115, 5, RED);

	uint8_t min_set = 0;
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	int32_t read_after;
	int32_t rotation;
	while (1)
	{
		read_before = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(500);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
		{
			break;
		}
		else
		{
			read_after = __HAL_TIM_GET_COUNTER(&htim2);
			rotation = read_after - read_before;
			if (rotation > 0)
			{
				min_set = min_set + 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
			if (rotation <= 0)
			{
				min_set = min_set - 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
		}
		sprintf(buffer, "%02d", min_set);
		ILI9341_DrawText(buffer, FONT6, 135, 125, BLACK, WHITE);
	}
	alarm.minutes_alaram_1 = min_set;
	machine_case_3();
}
void machine_case_3() // set alram 1 hour
{
	ILI9341_DrawFilledCircle(150, 115, 5, WHITE);
	ILI9341_DrawFilledCircle(105, 115, 5, RED);
	uint8_t min_set = 0; // is is hour but i copy previous function
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	int32_t read_after;
	int32_t rotation;
	while (1)
	{
		read_before = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(500);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
		{
			break;
		}
		else
		{
			read_after = __HAL_TIM_GET_COUNTER(&htim2);
			rotation = read_after - read_before;
			if (rotation > 0)
			{
				min_set = min_set + 1;
				if (min_set > 23)
					min_set = 23;
				if (min_set < 0)
					min_set = 0;
			}
			if (rotation < 0)
			{
				min_set = min_set - 1;
				if (min_set > 23)
					min_set = 23;
				if (min_set < 0)
					min_set = 0;
			}
		}
		sprintf(buffer, "%02d", min_set);
		ILI9341_DrawText(buffer, FONT6, 90, 125, BLACK, WHITE);
	}
	alarm.hour_alaram_1 = min_set;
	Set_Alaram_1(alarm.hour_alaram_1, alarm.minutes_alaram_1, alarm.seconds_alaram_1);
	ILI9341_DrawFilledCircle(105, 115, 5, WHITE);
	alarm.status_alaram_1=1;
	machine_case_4();
}
void machine_case_4()
{

	ILI9341_DrawFilledCircle(270, 115, 5, WHITE);
	ILI9341_DrawFilledCircle(270, 165, 5, RED);
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	HAL_Delay(100);
	int32_t read_after;
	int32_t rotation = read_after - read_before;

	while (1)
	{
		read_after = __HAL_TIM_GET_COUNTER(&htim2);
		rotation = read_after - read_before;
		if (rotation != 0)
			break;
	}
	{
		HAL_Delay(100);
		read_after = __HAL_TIM_GET_COUNTER(&htim2);
		rotation = read_after - read_before;

		if (rotation > 0)
		{
			sprintf(status, "   ");
			ILI9341_DrawText(status, FONT6, 250, 175, WHITE, WHITE);
			sprintf(status, "ON");
			ILI9341_DrawText(status, FONT6, 250, 175, BLACK, WHITE);
			HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E << 1, 1, (uint8_t *)1, 1, 1000);
			while (1)
			{
				BUTTON_Handle(&bien_nut_bam);
						if(bien_nut_bam.counter==2)
						{
							bien_nut_bam.counter=0;
							break;

						}			}
			alarm.status_alaram_2 = 1;
			// ILI9341_DrawFilledCircle(270,115,5,GREEN);
			machine_case_5();
		}
		if (rotation <= 0)
		{
			sprintf(status, "OFF");
			ILI9341_DrawText(status, FONT6, 250, 175, BLACK, WHITE);
			alarm.status_alaram_2 = 0;
			cancel_alaram_2();
			ILI9341_DrawFilledCircle(270, 165, 5, WHITE);
		}
	}
}
void machine_case_5()
{
	ILI9341_DrawFilledCircle(270, 165, 5, WHITE);
	ILI9341_DrawFilledCircle(150, 155, 5, RED);

	uint8_t min_set = 0;
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	int32_t read_after;
	int32_t rotation;
	while (1)
	{
		read_before = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(500);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
		{
			break;
		}
		else
		{
			read_after = __HAL_TIM_GET_COUNTER(&htim2);
			rotation = read_after - read_before;
			if (rotation > 0)
			{
				min_set = min_set + 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
			if (rotation < 0)
			{
				min_set = min_set - 1;
				if (min_set > 60)
					min_set = 59;
				if (min_set < 0)
					min_set = 0;
			}
		}
		sprintf(buffer, "%02d", min_set);
		ILI9341_DrawText(buffer, FONT6, 135, 175, BLACK, WHITE);
	}
	alarm.minutes_alaram_2 = min_set;
	machine_case_6();
}
void machine_case_6()
{

	ILI9341_DrawFilledCircle(150, 155, 5, WHITE);
	ILI9341_DrawFilledCircle(105, 165, 5, RED);

	uint8_t min_set = 0;
	int32_t read_before = __HAL_TIM_GET_COUNTER(&htim2);
	int32_t read_after;
	int32_t rotation;
	while (1)
	{
		read_before = __HAL_TIM_GET_COUNTER(&htim2);
		HAL_Delay(500);
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3) == 0)
		{
			break;
		}
		else
		{
			read_after = __HAL_TIM_GET_COUNTER(&htim2);
			rotation = read_after - read_before;
			if (rotation > 0)
			{
				min_set = min_set + 1;
				if (min_set > 23)
					min_set = 23;
				if (min_set < 0)
					min_set = 0;
			}
			if (rotation < 0)
			{
				min_set = min_set - 1;
				if (min_set > 23)
					min_set = 23;
				if (min_set < 0)
					min_set = 0;
			}
		}
		sprintf(buffer, "%02d", min_set);
		ILI9341_DrawText(buffer, FONT6, 90, 175, BLACK, WHITE);
	}
	alarm.hour_alaram_2 = min_set;
	Set_Alaram_2(alarm.hour_alaram_2, alarm.minutes_alaram_2);
	alarm.status_alaram_2=1;
	ILI9341_DrawFilledCircle(105, 165, 5, WHITE);
}
