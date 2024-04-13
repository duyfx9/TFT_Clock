#include "DS3231.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

uint8_t decToBcd(uint8_t val)
{
	return (uint8_t)((val / 10 * 16) + (val % 10));
}

// Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val)
{
	return (int)((val / 16 * 10) + (val % 16));
}

uint8_t Set_bit(uint8_t adress_register, uint8_t bit)
{
	adress_register |= (1 << bit);
	return adress_register;
}
uint8_t Clear_bit(uint8_t adress_register, uint8_t bit)
{
	adress_register &= ~(1 << bit);
	return adress_register;
}

void Set_bit_move_1(uint8_t adress, uint8_t bit)
{
	uint8_t data = 0;
	uint8_t data1 = 0;
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, adress, 8, &data, 1, 1000);
	data1 = Set_bit(data, bit);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, adress, 8, &data1, 1, 1000);
}
void Set_bit_move_0(uint8_t adress, uint8_t bit)
{
	uint8_t data = 0;
	uint8_t data1 = 0;
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, adress, 8, &data, 1, 1000);
	data1 = Clear_bit(data, bit);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, adress, 8, &data1, 1, 1000);
}

void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow,
				uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow); // day of weak
	set_time[4] = decToBcd(dom); // day of month
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

void Get_Time(TIME_t * time)  //TIME_t * time
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time->seconds = bcdToDec(get_time[0]);
	time->minutes = bcdToDec(get_time[1]);
	time->hour = bcdToDec(get_time[2]);
	time->dayofweek = bcdToDec(get_time[3]);
	time->dayofmonth = bcdToDec(get_time[4]);
	time->month = bcdToDec(get_time[5]);
	time->year = bcdToDec(get_time[6]);
}

STATE_ALARM_t cancel_alaram_1()
{
	Set_bit_move_0(0x0E, 0);
	return 	CANCEL_ALARM_1;
}

STATE_ALARM_t cancel_alaram_2()
{
	Set_bit_move_0(0x0E, 1);
	return 	CANCEL_ALARM_2;
}

STATE_ALARM_t Set_Alaram_1(uint8_t hour, uint8_t min, uint8_t sec)
{

	uint8_t alaram_1_register[3];
	alaram_1_register[0] = decToBcd(sec);
	alaram_1_register[1] = decToBcd(min);
	alaram_1_register[2] = decToBcd(hour);

	Set_bit_move_1(0x0E,0);
	Set_bit_move_1(0x0E,2);

	//Set_bit_move_1(0x0E,2);
	//HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, (uint8_t *)1, 1, 1000);		// set bit enable
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E << 0, 1, (uint8_t *)1, 1, 1000); // set bit enable
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E << 2, 1, (uint8_t *)1, 1, 1000); // set bit intcn
	//data = 0 << 7;
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x07, 1, &data, 1, 1000);
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x08, 1, &data, 1, 1000);
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x09, 1, &data, 1, 1000);
	//data = 1 << 7;
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0A, 1, &data, 1, 1000);
	Set_bit_move_0(0x07,7);
	Set_bit_move_0(0x08,7);
	Set_bit_move_0(0x09,7);
	Set_bit_move_1(0x0A,7);

	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x07, 1, &alaram_1_register[0], 1, 1000);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x08, 1, &alaram_1_register[1], 1, 1000);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x09, 1, &alaram_1_register[2], 1, 1000);

	return SET_ALARM_1;
}
STATE_ALARM_t Set_Alaram_2(uint8_t hour, uint8_t min)
{
	uint8_t alaram_2_register[2];
	alaram_2_register[0] = decToBcd(min);
	alaram_2_register[1] = decToBcd(hour);
	//data = 1 << 1;
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &data, 1, 1000); // set bit enable
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &data, 1, 1000); // set bit intcn
	Set_bit_move_1(0x0E,1);
	Set_bit_move_1(0x0E,2);

	//Set_bit_move_1(0x0E,2);
	//data = 0 << 7;
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0B, 1, &data, 1, 1000);
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0C, 1, &data, 1, 1000);
	//data = 1 << 7;
	//HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0D, 1, &data, 1, 1000);
	Set_bit_move_0(0x0B,7);
	Set_bit_move_0(0x0C,7);
	Set_bit_move_1(0x0D,7);


	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0B, 1, &alaram_2_register[0], 1, 1000);
	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0C, 1, &alaram_2_register[1], 1, 1000);

	return SET_ALARM_2;
}


