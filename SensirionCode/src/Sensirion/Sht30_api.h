/*
 * Sht30_api.h
 *
 *  Created on: 2020Äê5ÔÂ15ÈÕ
 *      Author: Cenx
 */

#ifndef SENSIRION_SHT30_API_H_
#define SENSIRION_SHT30_API_H_

#include "i2c.h"

void Sht30_Init1(void) ;
extern void Sht30_InitPeriod(void) ;
extern void Sht30_GetData(void) ;
extern void Sht30_GetTempAndHumi(void) ;
extern void Sht30_GetTempAndHumi1(void) ;
void WirteByte_I2C(unsigned char addr, unsigned char data);
unsigned char ReadByte_I2C(unsigned char addr);
unsigned char ReadByte_I2C1(unsigned char addr);

#endif /* SENSIRION_SHT30_API_H_ */
