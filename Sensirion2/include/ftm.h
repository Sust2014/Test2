/*
 * FTM.h
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#ifndef ftm_H_
#define ftm_H_
/*******************************************************************************
* Includes
********************************************************************************/
#include "derivative.h"
#include <stdbool.h>

bool      Forcedbackflowflag_15 ;
bool      Forcedbackflowflag_25 ;
uint8_t second;uint8_t second1;uint8_t second2;
uint8_t min;
uint8_t hour;
uint8_t count;uint16_t count_l;uint8_t count_9;
uint16_t test_count ;
uint8_t count1;//当LED_FLAG=1的时候计时 0.15LED闪烁一次
uint8_t count2;//LED闪烁计数 当闪烁次数=气压值 LEDno闪烁
uint8_t count3;//LED闪烁计数 当闪烁次数=气压值 LEDno闪烁
uint8_t count6;//LED闪烁计数 当闪烁次数=气压值 LEDno闪烁
uint8_t count_LED_R;
uint8_t count_LED_R_Recover;uint8_t count_value;
uint8_t count_LED_R_Venting;uint8_t Open_count;
uint8_t count_LED_R_Backflow;uint8_t count_LED_value9;uint8_t count_voltage;uint8_t count_LED_value9_voltage;
uint8_t count_LED_value;uint8_t count_LED_value1;uint8_t count_LED_value2;uint8_t count_LED_value3;

uint16_t sys_count ;

/*充气方式1：定时充气
 * 20s开启排气电磁阀0.3s
 * 同时检测整车输入信号是否为高电平，若整车输入信号切换为低电平
 * 退出充气，干燥器恢复供气
 */
uint16_t type1Count ;
bool type1Flag ;
/*******************************************************************************
* Constants
********************************************************************************/


/*******************************************************************************
* Macros
********************************************************************************/

                
/*******************************************************************************
* Types
********************************************************************************/


/*******************************************************************************
* Variables
********************************************************************************/


/*******************************************************************************
* Global Variables
********************************************************************************/


/*******************************************************************************
* Global Functions
********************************************************************************/
/***********************************************************************************************
*
* @brief    FTM_Init - Initialize the FTM
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_Init_Output_Cmp();
void FTM2_Init_EPWM();
void FTM2_SetPeriod(uint8_t ch, uint32_t period);
void Enable_Interrupt(uint8_t vector_number);
void FTM2_IRQHandler();
void FTM2_Interrupt(void);

#endif /* FTM_H_ */
