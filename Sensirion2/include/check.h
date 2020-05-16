/*
 * check.h
 *
 *  Created on: 2019年12月27日
 *      Author: Cenx
 */

#ifndef CHECK_H_
#define CHECK_H_

#include "adc_init.h"
#include "utility_function.h"
#include "gpio_init.h"
#include "systick.h"

#define Input_32_Vol      2327//验证
#define Input_18_Vol      1286//验证
#define Input_14_Vol      996//验证

#define Sensor_VolH       3759//4.6V
#define Sensor_VolRh      3596//4.4V
#define Sensor_VolL       162//0.2V
#define Sensor_VolRl      350//0.43V

#define Connect_VolL      1634//2V
#define Connect_VolRl     3269//4V

/*电磁阀开启电流上限值设为3A
24V时电磁阀的开启电流为0.6A*/
#define Cur_VolL          162//0.2V 0.26A
#define Cur_VolH          1879//2.30V 3A

#define Temp_VolH         3574//125℃
#define Temp_VolRh        3509//120℃

#define Out_VolH          3269//4V
#define Out_VolL          1625//2V

bool RecoOpen ;
bool RecoShort ;

bool VentOpen ;
bool VentShort ;

bool st1_2Open ;

bool BackOpen ;
bool BackShort ;

bool Out4Open ;

bool st3_4Open ;

/*电磁阀连接故障*/
bool errorCon ;

bool inHcycle ;
uint8_t inHcnt ;
uint8_t inHcount ;
bool sysInH ;

typedef struct{
	uint8_t cycle ;
	uint8_t cnt ;

	bool start ;
	bool flag ;
}sysCh;

typedef struct{
	uint8_t cycle ;
	uint8_t cnt_h ;
	uint8_t cnt_l ;
	bool state ;
}error;

extern sysCh Hw[] ;

void Fun_InputVolH();
void Fun_InputVolL();
void Fun_sensorVolH();
void Fun_sensorVolL();
void Fun_valveConnect();
void Fun_OpenValve();
void Fun_TempH();
void Fun_ShortCh();
void Fun_dischargeAir();
void Fun_HwCheck();
void Fun_led();
void Fun_ErrorLed();
#endif /* CHECK_H_ */
