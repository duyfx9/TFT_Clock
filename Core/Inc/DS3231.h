#ifndef __DS3231_H__
#define __DS3231_H__

#include"stdint.h"

#define DS3231_ADDRESS 0xD0

 typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
}  TIME_t;

 typedef struct
{
	// alarm 1
	uint8_t seconds_alaram_1;
	uint8_t minutes_alaram_1;
	uint8_t hour_alaram_1;
	uint8_t status_alaram_1;

	// alarm 2
	uint8_t minutes_alaram_2;
	uint8_t hour_alaram_2;
	uint8_t status_alaram_2;
}  TIME_ALARM_t;

 typedef enum
{
	SET_ALARM_1 = 0,
	ALARM_1_SEC,
	ALARM_1_MIN,
	ALARM_1_HOUR,
	CANCEL_ALARM_1,

	SET_ALARM_2,
	ALARM_2_MIN,
	ALARM_2_HOUR,
	CANCEL_ALARM_2

}STATE_ALARM_t;

//TIME_t time;
//TIME_ALARM_t alarm;
// TIME

void Set_Time (uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow,
				uint8_t dom, uint8_t month, uint8_t year);
void Get_Time (TIME_t * time);

// ALARM
void Read_Alaram(void);
STATE_ALARM_t cancel_alaram_1(void);
STATE_ALARM_t cancel_alaram_2(void);
STATE_ALARM_t Set_Alaram_1(uint8_t hour,uint8_t min, uint8_t sec);
STATE_ALARM_t Set_Alaram_2(uint8_t hour, uint8_t min);
void Set_bit_move_1(uint8_t adress, uint8_t bit);
void Set_bit_move_0(uint8_t adress, uint8_t bit);
uint8_t Set_bit(uint8_t adress_register, uint8_t bit);
uint8_t Clear_bit(uint8_t adress_register, uint8_t bit);
uint8_t bcdToDec(uint8_t val);
uint8_t decToBcd(uint8_t val);



#endif
