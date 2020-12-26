/*
 * sht20.c
 *
 *  Created on: Dec 21, 2020
 *      Author: sleep
 */
#include "stm32f1xx_hal.h"

#include "sht20.h"
#include "i2c.h"
#include "crc.h"


float static SHT20_calc_temp(uint16_t raw_temp)
{
	float temperature = -46.85 + 175.72 * ((float)raw_temp / 65536);
	return temperature;
}



float static SHT20_calc_humidity(uint16_t raw_temp)
{
	float humidity = -6 + 125 * ((float)raw_temp / 65536);
	return humidity;
}



float static SHT20_get_temp(void)
{
	uint8_t temp[3] = {0};
	HAL_I2C_Mem_Write(&hi2c1, SHT20_ADDR << 1, SHT20_Temp_HM, I2C_MEMADD_SIZE_8BIT, (void *)0, 0, 100);

	HAL_I2C_Master_Receive(&hi2c1, SHT20_ADDR << 1, temp, 3, 100);

	sht20_data.crc[0] = HAL_CRC_Calculate(&hcrc, (uint32_t *)temp[2], 1);

	return SHT20_calc_temp(temp[0] << 8 | temp[1]);
}



float static SHT20_get_humidity(void)
{
	uint8_t temp[2] = {0};
	HAL_I2C_Mem_Write(&hi2c1, SHT20_ADDR << 1, SHT20_HUMIDITY_HM, I2C_MEMADD_SIZE_8BIT, (void *)0, 0, 100);

	HAL_I2C_Master_Receive(&hi2c1, SHT20_ADDR << 1, temp, 2, 100);

	return SHT20_calc_humidity(temp[0] << 8 | temp[1]);
}



void SHT20_get(void)
{
	sht20_data.temp = SHT20_get_temp();
	sht20_data.humidity = SHT20_get_humidity();


}
