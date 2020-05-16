/*
 * led.h
 *
 *  Created on: 2020Äê1ÔÂ8ÈÕ
 *      Author: Cenx
 */

#ifndef LED_H_
#define LED_H_

#include "check.h"

typedef struct{
	uint8_t t_on ;
	uint8_t t_off ;
	uint8_t t_interval ;
}errorSt;

void Fun_ErrLed();
#endif /* LED_H_ */
