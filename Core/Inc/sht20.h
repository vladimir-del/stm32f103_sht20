/*
 * sht20.h
 *
 *  Created on: Dec 21, 2020
 *      Author: sleep
 */

#ifndef INC_SHT20_H_
#define INC_SHT20_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define SHT20_ADDR 0x40
#define SHT20_RESET 0xFE
#define SHT20_Temp_HM 0xE3
#define SHT20_HUMIDITY_HM 0xE5


typedef struct {
	float temp;
	float humidity;
} SHT20_DATA;

SHT20_DATA sht20_data;



void SHT20_get(void);

void SHT20_soft_reset(void);

#endif /* INC_SHT20_H_ */
