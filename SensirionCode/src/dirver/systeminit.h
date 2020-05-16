/*
 * systeminit.h
 *
 *  Created on: 2020Äê5ÔÂ15ÈÕ
 *      Author: Cenx
 */

#ifndef DIRVER_SYSTEMINIT_H_
#define DIRVER_SYSTEMINIT_H_

#include "derivative.h"

#define INTERCLK
//#define EXTERNCLK

void Clk_Internal() ;
void Clk_External() ;
void delay_s1(uint16_t y) ;

#endif /* DIRVER_SYSTEMINIT_H_ */
