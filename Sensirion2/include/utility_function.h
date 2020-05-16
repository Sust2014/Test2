/*
 * utility_function.h
 *
 *  Created on: 2019年5月17日
 *      Author: Cenx
 */

#ifndef UTILITY_FUNCTION_H_
#define UTILITY_FUNCTION_H_

#include "derivative.h" /* include peripheral declarations SSKEAZ64M4 */
#include "gpio_init.h"
#include "ftm.h"
#include "adc_init.h"
#include "wdog.h"
#include "fault_led.h"
#include "can.h"
#include "i2c.h"

/***************************
P1、P2定义:
1.传感器量程为0-20bar
2.对应电压值为：0.5-4.5V
3.7bar时，电压值为1.9V,对应AD值为388; 1.88V 7.5: 1.97V 8: 2.07V
4.10bar时,电压值为2.5V，对应AD值为511; 6: 1.68V 6.5: 1.77V
**********************************/
#define LED_R_ON      GPIOB_PDOR |= 1 << LED_R
#define LED_R_OFF     GPIOB_PDOR &= ~(1 << LED_R)
#define LED_R_TOG     GPIOB_PTOR |= 1 << LED_R

#define LED_G_TOG     GPIOA_PTOR |= 1 << LED_G
#define LED_G_ON      GPIOA_PDOR |= 1 << LED_G
#define LED_G_OFF     GPIOA_PDOR &= ~(1 << LED_G)

/*红色LED开启标志位*/
uint8_t Led_R_OpenFlag ;

uint8_t Air_pressure_l_count;
uint8_t Air_pressure_l_count1;

/*当前气压值*/
uint16_t CurrentPressure ;

/*5s后气压值*/
uint16_t After5sPressure ;

/*气压处于上升阶段*/
uint8_t PressureRise ;

/*检测到整车输入信号为高电平，干燥器排气标志*/
uint8_t ForceVentingFlag ;

uint32_t Adc_Detection_value_h ;
#define Air_pressure_up_value      1883//9.5 1942 排气压力 11 2185  11 2206 10 2047 9 1883
#define Air_pressure_up_value1      1552//6 1389 恢复供气压力 8 1699 8.5 1780 8.5 1798 7 1552

/*间隔10s执行气压操作*/
bool reTiming ;
uint16_t reCount ;
bool veTiming ;
uint16_t veCount ;
/*开启确认进入恢复供气模式标志位*/
uint8_t RecoverMode ;
/*开启进入恢复供气模式计数*/
uint8_t RecoverCount ;
/*干燥器处于恢复供气状态*/
uint8_t InTheRecover ;

/*开启确认高电平时进入恢复供气模式标志位*/
uint8_t h_RecovMode ;
/*开启进入恢复供气模式计数*/
uint8_t h_RecovCount ;

/*开启确认进入排气模式标志位*/
uint8_t VentingMode ;
/*开启进入排气模式计数*/
uint8_t VentingCount ;
/*干燥器处于排气状态*/
uint8_t InTheVenting ;

/*开启确认高电平时进入内卸荷模式标志位*/
uint8_t h_unloadMode ;
/*开启高电平时进入内卸荷模式计数*/
uint8_t h_unloadCount ;

/*开启确认高电平时进入内卸荷模式标志位*/
uint8_t h_VentMode ;
/*开启高电平时进入内卸荷模式计数*/
uint8_t h_VentCount ;

/*开启确认进入强制回流模式标志位*/
uint8_t BackflowMode15 ;
/*开启进入强制回流模式计数*/
uint8_t BackflowCount15 ;

/*开启确认进入强制回流模式标志位*/
uint8_t BackflowMode30 ;
/*开启进入强制回流模式计数*/
uint8_t BackflowCount30 ;

#define Air_pressure_up      2180//11
#define Air_pressure_up1      1776//8.5

#define Force_Venting      1389//6

/*回流气压条件*/
#define Force_Backflow      1699//8
bool      TheFirstHighLevel ;//第一次检测到高电平标志位
uint16_t BackflowTime ;//回流时间计时

uint16_t AutoflowTime ;//回流时间计时

bool      Forcedloading ;//高电平标志
bool      AirInflowFlag ;//低气压进入内卸荷标志
bool      Low_level_flag ;//低电平标志
bool      No_high_flag ;//检测不到高电平
bool      Low_pressure_1 ;//气压低于1
#define Air_pressure_up_value1_4      1064
#define Air_pressure_up_value1_1      573
#define Air_pressure_up_value_error      2375
#define Air_pressure_up_value_error1      2459
#define Air_pressure_up_value1_error      1200
#define Backflow_up_value      1552//8 15分强制回流压力 7 1537 7 1552
#define Backflow_up_value1      1471//7.5 30分强制回流压力 6.5 1456 6.5 1471
//#define Adc_Detection_value_h      3276
#define Adc_Detection_value_l      163
#define Adc_Input_voltage_32      2296//实际测量32V为2317 表格中为2383
#define Adc_Input_voltage_h      2520//35V
#define Adc_Input_voltage_l      1123 //16V
#define Adc_Input_voltage_24      1717 //16V
#define Adc_Sensor_voltage_h      3715
#define Adc_Sensor_voltage_l      364
#define Adc_T_voltage_h      3509
#define Adc_T_voltage_l      27

#define Adc_T_voltage_80      3155
#define Adc_T_voltage_l20      27

uint8_t LED_FLAG;uint8_t LED_FLAG_1;uint8_t LED_FLAG_2;

void Fun_ForceControl(uint8_t Open_t);
void Fun_AutoControl(uint8_t Open_t);
void Backflow_detection();
uint8_t Air_pressure_led(uint32_t t);
void Detect_high_level();
void Detect_Air_pressure();
void Error_detection();
void WDOG_structure_init();
#endif /* UTILITY_FUNCTION_H_ */
