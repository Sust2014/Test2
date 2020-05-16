/*
 * utility_function.h
 *
 *  Created on: 2019��5��17��
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
P1��P2����:
1.����������Ϊ0-20bar
2.��Ӧ��ѹֵΪ��0.5-4.5V
3.7barʱ����ѹֵΪ1.9V,��ӦADֵΪ388; 1.88V 7.5: 1.97V 8: 2.07V
4.10barʱ,��ѹֵΪ2.5V����ӦADֵΪ511; 6: 1.68V 6.5: 1.77V
**********************************/
#define LED_R_ON      GPIOB_PDOR |= 1 << LED_R
#define LED_R_OFF     GPIOB_PDOR &= ~(1 << LED_R)
#define LED_R_TOG     GPIOB_PTOR |= 1 << LED_R

#define LED_G_TOG     GPIOA_PTOR |= 1 << LED_G
#define LED_G_ON      GPIOA_PDOR |= 1 << LED_G
#define LED_G_OFF     GPIOA_PDOR &= ~(1 << LED_G)

/*��ɫLED������־λ*/
uint8_t Led_R_OpenFlag ;

uint8_t Air_pressure_l_count;
uint8_t Air_pressure_l_count1;

/*��ǰ��ѹֵ*/
uint16_t CurrentPressure ;

/*5s����ѹֵ*/
uint16_t After5sPressure ;

/*��ѹ���������׶�*/
uint8_t PressureRise ;

/*��⵽���������ź�Ϊ�ߵ�ƽ��������������־*/
uint8_t ForceVentingFlag ;

uint32_t Adc_Detection_value_h ;
#define Air_pressure_up_value      1883//9.5 1942 ����ѹ�� 11 2185  11 2206 10 2047 9 1883
#define Air_pressure_up_value1      1552//6 1389 �ָ�����ѹ�� 8 1699 8.5 1780 8.5 1798 7 1552

/*���10sִ����ѹ����*/
bool reTiming ;
uint16_t reCount ;
bool veTiming ;
uint16_t veCount ;
/*����ȷ�Ͻ���ָ�����ģʽ��־λ*/
uint8_t RecoverMode ;
/*��������ָ�����ģʽ����*/
uint8_t RecoverCount ;
/*���������ڻָ�����״̬*/
uint8_t InTheRecover ;

/*����ȷ�ϸߵ�ƽʱ����ָ�����ģʽ��־λ*/
uint8_t h_RecovMode ;
/*��������ָ�����ģʽ����*/
uint8_t h_RecovCount ;

/*����ȷ�Ͻ�������ģʽ��־λ*/
uint8_t VentingMode ;
/*������������ģʽ����*/
uint8_t VentingCount ;
/*��������������״̬*/
uint8_t InTheVenting ;

/*����ȷ�ϸߵ�ƽʱ������ж��ģʽ��־λ*/
uint8_t h_unloadMode ;
/*�����ߵ�ƽʱ������ж��ģʽ����*/
uint8_t h_unloadCount ;

/*����ȷ�ϸߵ�ƽʱ������ж��ģʽ��־λ*/
uint8_t h_VentMode ;
/*�����ߵ�ƽʱ������ж��ģʽ����*/
uint8_t h_VentCount ;

/*����ȷ�Ͻ���ǿ�ƻ���ģʽ��־λ*/
uint8_t BackflowMode15 ;
/*��������ǿ�ƻ���ģʽ����*/
uint8_t BackflowCount15 ;

/*����ȷ�Ͻ���ǿ�ƻ���ģʽ��־λ*/
uint8_t BackflowMode30 ;
/*��������ǿ�ƻ���ģʽ����*/
uint8_t BackflowCount30 ;

#define Air_pressure_up      2180//11
#define Air_pressure_up1      1776//8.5

#define Force_Venting      1389//6

/*������ѹ����*/
#define Force_Backflow      1699//8
bool      TheFirstHighLevel ;//��һ�μ�⵽�ߵ�ƽ��־λ
uint16_t BackflowTime ;//����ʱ���ʱ

uint16_t AutoflowTime ;//����ʱ���ʱ

bool      Forcedloading ;//�ߵ�ƽ��־
bool      AirInflowFlag ;//����ѹ������ж�ɱ�־
bool      Low_level_flag ;//�͵�ƽ��־
bool      No_high_flag ;//��ⲻ���ߵ�ƽ
bool      Low_pressure_1 ;//��ѹ����1
#define Air_pressure_up_value1_4      1064
#define Air_pressure_up_value1_1      573
#define Air_pressure_up_value_error      2375
#define Air_pressure_up_value_error1      2459
#define Air_pressure_up_value1_error      1200
#define Backflow_up_value      1552//8 15��ǿ�ƻ���ѹ�� 7 1537 7 1552
#define Backflow_up_value1      1471//7.5 30��ǿ�ƻ���ѹ�� 6.5 1456 6.5 1471
//#define Adc_Detection_value_h      3276
#define Adc_Detection_value_l      163
#define Adc_Input_voltage_32      2296//ʵ�ʲ���32VΪ2317 �����Ϊ2383
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
