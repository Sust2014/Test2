/*
 * systick.h
 *
 *  Created on: 2019Äê11ÔÂ13ÈÕ
 *      Author: Cenx
 */

#ifndef DRIVER_SYSTICK_H_
#define DRIVER_SYSTICK_H_

#include <SKEAZ1284.h>

uint32_t SysTick_Config1(uint32_t ticks) ;
void Calculation();
void task_us(uint16_t count);
void task_ms(uint16_t count);
void task_ms1(uint16_t count) ;
void task_s(int8_t count);
void sdelay_us(uint32_t time) ;
void sdelay_ms(uint32_t time) ;

#endif /* DRIVER_SYSTICK_H_ */
