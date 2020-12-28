/*
 * sht20.c
 *
 *  Created on: Dec 21, 2020
 *      Author: sleep
 */

#include "sht20.h"
#include "i2c.h"

const int8_t POLYNOMIAL = 0x31; //x8 + x5 + x4 +1

void SHT20_soft_reset(void)
{
	HAL_I2C_Mem_Write(&hi2c1, SHT20_ADDR << 1, SHT20_RESET, I2C_MEMADD_SIZE_8BIT, (void *)0, 0, 100);
	HAL_Delay(50);
	return;
}



uint8_t static SHT20_get_crc(uint8_t *pcBlock, uint8_t len)
{
    unsigned char crc = 0;
    unsigned int i;

    while (len--)
    {
        crc ^= *pcBlock++;

        for (i = 0; i < 8; i++)
            crc = crc & 0x80 ? (crc << 1) ^ POLYNOMIAL : crc << 1;
    }

    return crc;
}


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
	HAL_I2C_Mem_Write(&hi2c1, SHT20_ADDR << 1, SHT20_TEMPERATURE_HM, I2C_MEMADD_SIZE_8BIT, (void *)0, 0, 100);

	HAL_I2C_Master_Receive(&hi2c1, SHT20_ADDR << 1, temp, 3, 100);

	if (temp[2] != SHT20_get_crc(temp, 2))
		return HAL_ERROR;
	uint16_t rawTemp = temp[0] << 8 | temp[1];

	rawTemp &= ~0x3; //clear status bit

	return SHT20_calc_temp(rawTemp);
}



float static SHT20_get_humidity(void)
{
	uint8_t temp[3] = {0};
	HAL_I2C_Mem_Write(&hi2c1, SHT20_ADDR << 1, SHT20_HUMIDITY_HM, I2C_MEMADD_SIZE_8BIT, (void *)0, 0, 100);

	HAL_I2C_Master_Receive(&hi2c1, SHT20_ADDR << 1, temp, 3, 100);

	if (temp[2] != SHT20_get_crc(temp, 2))
		return HAL_ERROR;

	uint16_t rawHum = temp[0] << 8 | temp[1];

	rawHum &= ~0x3; //clear status bit

	return SHT20_calc_humidity(rawHum);
}



void SHT20_get(void)
{
	sht20_data.temp = SHT20_get_temp();
	sht20_data.humidity = SHT20_get_humidity();
}
