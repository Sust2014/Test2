/*
 * can.h
 *
 *  Created on: 2015Äê10ÔÂ8ÈÕ
 *      Author: Chen_Sky
 */

#ifndef DRIVERS_CAN_H_
#define DRIVERS_CAN_H_

#include "SKEAZ1284.h"

#define MSCAN_OSC_CLK 0
#define MSCAN_BUS_CLK 1

#define STANDARD     0
#define EXTENDED     1

#define MSCAN_SJW    3
#define MSCAN_BRP   19
#define MSCAN_TSEG1  4
#define MSCAN_TSEG2  3

typedef struct{
	int8_t year ;
	int8_t month ;
	int8_t day ;
	int8_t re_air ;
	int8_t ve_air ;
	int8_t fl_time ;
	int8_t version1 ;
	int8_t version2 ;
}sysTime;
sysTime sysCompileDate ;

void Fun_GetCompileTime(sysTime *Time);
extern uint8_t Temperature_conversion_Copper(uint16_t t) ;
void CAN_Init(uint8_t mscan_clk,uint8_t mscan_sjw,uint8_t mscan_brp,uint8_t mscan_tseg1,uint8_t mscan_tseg2);
void CAN_Send(uint32_t mscan_addr,const uint8_t * mscan_data,uint8_t mscan_dlr,uint8_t mscan_ide) ;
void flexcan_Trigger();

#endif /* DRIVERS_CAN_H_ */
