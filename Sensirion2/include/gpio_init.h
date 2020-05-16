/*
 * gpio_init.h
 *
 *  Created on: 2019年6月17日
 *      Author: Cenx
 */

#include <SKEAZ1284.h>
#include "utility_function.h"
#include <stdbool.h>

#ifndef GPIO_INIT_H_
#define GPIO_INIT_H_

#define LED_G   PTB5
#define PTB5      13

#define LED_R   PTE5
#define PTE5      5

#define Pressure_detection   PTF6
#define PTF6      14

#define Current_detection   PTF7
#define PTF7      15

#define Connect_gpio_1   A
#define Connect_gpio1   GPIOA
#define Connect_1   PTB0
#define PTB1      9

#define Connect_2   PTB1
#define PTB0      8

#define Voltage_24V   PTA7
#define PTA7      7

#define T_Voltage   PTA6
#define PTA6      6

#define Venting_solenoid_valve   PTD3
#define PTD3      27

#define Recover_air_supply_solenoid_valve   PTD2
#define PTD2      26

#define Control_solenoid_valve   PTF0
#define PTF0      8

#define Backflow_solenoid_valve   PTD4
#define PTD4      28

#define K1_OUT   PTA1
#define PTA1      1

#define Input_voltage   PTA0
#define PTA0      0

#define Input_voltage1   PTA1
#define PTA1      1

#define Can_tx   PTC7
#define PTC7      23

#define Can_rx   PTC6
#define PTC6      22

#define GPIO_INPUT_DETECTION(gpio,gpio_pin)       GPIO_DETECTION(gpio,gpio_pin)
#define GPIO_DETECTION(gpio,gpio_pin)             ((GPIO##gpio##_PDIR & 1 << gpio_pin) == 1) ? 0 : 1

#define Venting_ON      GPIOA_PDOR |= 1 << Venting_solenoid_valve
#define Venting_OFF     GPIOA_PDOR &= ~(1 << Venting_solenoid_valve)

#define Recover_ON      GPIOA_PDOR |= 1 << Recover_air_supply_solenoid_valve
#define Recover_OFF     GPIOA_PDOR &= ~(1 << Recover_air_supply_solenoid_valve)

#define Backflow_ON     GPIOA_PDOR |= 1 << Backflow_solenoid_valve
#define Backflow_OFF    GPIOA_PDOR &= ~(1 << Backflow_solenoid_valve)

bool Recover_flag;
bool Recover_init_flag;
bool Venting_flag;
bool Venting_init_flag;
bool Backflow_flag;
bool Backflow_init_flag;
bool Control_flag;

bool resh;
bool vesh;
bool flsh;

/*气压采样误差变量*/
uint8_t airerrorCount ;
uint8_t airerrorCount1 ;
bool airerror ;

uint8_t Recover_error;
uint8_t Recover_error_count;
uint8_t Venting_error;
uint8_t Venting_error_count;
uint8_t Backflow_error;
uint8_t Backflow_error_count;

uint8_t Input_voltage_24V_error1;
uint8_t Input_voltage_24V_error2;
uint8_t Input_voltage_24V_error;
uint8_t Input_voltage_detaction1;
uint8_t Input_voltage_detaction2;
uint8_t Sensor_voltage_error;
uint8_t Sensor_detection_error_count;
uint8_t Sensor_detection_error;
uint8_t T_voltage_error;
uint8_t T_voltage_error_count;

bool Input_vo;
bool Sensorvo;
bool Sensorvo1;
bool Solenoidvo;
bool Tvo;

uint8_t Air_pressure_l_count2;
uint16_t Inputvo_count;
uint16_t Sensorvo_count;
uint16_t Sensorvo_count1;
uint16_t Solenoidvo_count;
uint16_t Solenoidvo_count1;
uint16_t Tvo_count;

bool LED_R_12_flag;
bool LED_R_5_flag;

void Gpio_Init();
void delay(volatile int cycles);
void delay_s(uint8_t y);
void delay_s1(uint16_t y);
void delay_us(uint16_t y);
void Venting_On();
void Venting_Off();
void Recover_On();
void ForeceRecover();
void Recover_Off();
void Backflow_On();
void Backflow_Off();
void Control_On();
void Control_Off();
void Clk_Init();
void System_Init();
void BTS724G_detection();
void Self_inspection();
void VentingPipelinePressure();
uint8_t Gpio_Input_Detection(GPIO_Type * gpio , uint8_t gpio_pin);
uint8_t Gpio_Inputl(GPIO_Type * gpio , uint8_t gpio_pin);
void Solenoid_valve_Post();
void Valve_detection();
#endif /* GPIO_INIT_H_ */
