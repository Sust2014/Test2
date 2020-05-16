/*
 * ADC.h
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#ifndef adc_init_H_
#define adc_init_H_
/*******************************************************************************
* Includes
********************************************************************************/

#include "derivative.h"
#include <stdbool.h>

uint32_t adc_value_Air_pressure1;
uint32_t adc_value_Air_pressure2;
uint32_t adc_value_electricity;uint32_t electricity_count;uint32_t adc_value_electricity1;
uint32_t adc_value_t;
uint16_t adc_value_Input_24V;uint32_t adc_value_Input_detection1;uint32_t adc_value_Input_detection2;
uint32_t adc_value_1;

uint16_t bts724g_st1 ;
uint16_t bts724g_st2 ;
uint16_t current ;
uint16_t controlTemp ;
uint16_t outValue ;

uint8_t reShCnt ;
bool reShFlag ;

uint8_t veShCnt ;
bool veShFlag ;

uint8_t flShCnt ;
bool flShFlag ;
	
/*******************************************************************************
* Constants
********************************************************************************/


/*******************************************************************************
* Macros
********************************************************************************/
#define EIGHT_BIT 	0
#define TEN_BIT 	1
#define TWELVE_BIT 	2
                
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
* @brief    ADC_Init - Initiates the Channeln to read the value of the ADC channel
* 			 
* @param    Channel to init and resolution
* @return   none
*
************************************************************************************************/  
void ADC_Init(uint8_t channel, uint8_t mode);

/***********************************************************************************************
*
* @brief    ADC_Read - Read the selected ADC channel
* @param    ch - channel to read
* @return   result
*
************************************************************************************************/
uint16_t ADC_Read1(uint8_t channel);
uint16_t ADC_Read(uint8_t channel);
void ADC_connect_1();
void ADC_connect_2();
void adc_cur();
void cur_check(uint8_t type);
void ADC_Detection();

#endif /* ADC_H_ */
