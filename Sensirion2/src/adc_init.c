/*
 * ADC.c
 *
 *  Created on: 2019.7.21
 *      Author: byxzwz
 */

#include "adc_init.h"
#include "utility_function.h"
#include "string.h"
#include "stdlib.h"

uint32_t ADC_average_value[10] = {0,0,0,0,0,0,0,0,0,0} ;

void ADC_Init(uint8_t channel, uint8_t mode) {
	SIM_SCGC |= SIM_SCGC_ADC_MASK; /* Enable bus clock in ADC*/
	ADC_SC3 |= ADC_SC3_ADICLK(0b00); /* Bus clock selected*/
	ADC_SC2 |= 0x00; /* Software Conversion trigger, disable compare function*/
	ADC_SC1 = 0; /* Enable ADC by setting ADCH bits as low*/
	ADC_SC1 &= ~(1 << 5); /* Continuous mode operation */
	//ADC_SC1|= ADC_SC1_AIEN_MASK;  				/* ADC Interrupt Enabled */
	ADC_APCTL1 |= ADC_APCTL1_ADPC(1 << channel); /* Channel selection */
	ADC_SC3 |= ADC_SC3_MODE(mode); /* 8,10,12 bit mode operation */
}

uint32_t ADC_average_detection()
{
    uint32_t num[10]={0,0,0,0,0,0,0,0,0,0} ;
    uint32_t i,imax,imin,imaxp,iminp;
    uint32_t adc_value = 0 ;
    for(i=0;i<10;i++)
    {
    	num[i] = ADC_average_value[i] ;
    	adc_value += ADC_average_value[i] ;
    }
    imaxp=0;
    iminp=0;
    imax=num[0];
    imin=num[0];
    for(i=1;i<10;i++)
    {

        if(num[i]>imax)
        {
            imax=num[i];
            imaxp=i;
        }
        if(num[i]<imin)
        {
            imin=num[i];
            iminp=i;
        }
}
    return (adc_value - num[imaxp] - num[iminp]);
}
/***********************************************************************************************
 *
 * @brief    ADC_Read - Read the selected ADC channel
 * @param    ch - channel to read
 * @return   result
 *
 ************************************************************************************************/
uint16_t ADC_Read1(uint8_t channel) {
	uint32_t adc_value = 0 ;
	uint8_t i = 0 ;
	for(i = 0 ; i < 10 ; i++){
	ADC_average_value[i] = 0 ;
	}
	for(i = 0 ; i < 10 ; i++){
	ADC_SC1 |= ADC_SC1_ADCH(channel); /* Select channel to read */
	while (!(ADC_SC1 & ADC_SC1_COCO_MASK)); /* Wait conversion to complete */
	ADC_average_value[i] = ADC_R ;
	}
	adc_value = ADC_average_detection() >> 3 ;
	return adc_value; /* Return adc value */

}

uint16_t ADC_Read(uint8_t channel) {
	uint32_t adc_value = 0 ;
	uint8_t i = 0 ;
	for(i = 0 ; i < 10 ; i++){
		ADC_SC1 |= ADC_SC1_ADCH(channel); /* Select channel to read */
		while (!(ADC_SC1 & ADC_SC1_COCO_MASK)); /* Wait conversion to complete */
		adc_value += ADC_R ;
	}
	adc_value /= 10 ;
	return adc_value; /* Return adc value */

}

int i1 = 4 ;

void ADC_connect_1()
{
	ADC_Init(5,2);
	adc_value_Input_detection2 = ADC_Read(5);
	if(adc_value_Input_detection2 < 2047)
		Input_voltage_detaction2 = 1 ;
	else
		Input_voltage_detaction2 = 0 ;
}

void ADC_connect_2()
{
	ADC_Init(4,2);
	adc_value_Input_detection1 = ADC_Read(4);
	if(adc_value_Input_detection1 < 2047)
		Input_voltage_detaction1 = 1 ;
	else
		Input_voltage_detaction1 = 0 ;
}

void adc_input()
{
	ADC_Init(3,2);
	adc_value_Input_24V=ADC_Read(3);
}

void ADC_input1()
{
	ADC_Init(3,2);
	adc_value_Input_24V=ADC_Read(3);
	if(adc_value_Input_24V < Adc_Input_voltage_l )
		Input_voltage_24V_error = 1 ;
	if(adc_value_Input_24V > Adc_Input_voltage_h )
		Input_voltage_24V_error = 2 ;
	if(adc_value_Input_24V < Adc_Input_voltage_h && adc_value_Input_24V > Adc_Input_voltage_l)
		Input_voltage_24V_error = 0 ;
}

void ADC_BTS724G()
{
		ADC_Init(4,2);
		adc_value_Input_detection1 = ADC_Read(4);
		if(adc_value_Input_detection1 < 1638)
			Input_voltage_detaction1 = 1 ;
		if(adc_value_Input_detection1 > 2046)
			Input_voltage_detaction1 = 0 ;

		ADC_Init(5,2);
		adc_value_Input_detection2 = ADC_Read(5);
		if(adc_value_Input_detection2 < 1638)
			Input_voltage_detaction2 = 1 ;
		if(adc_value_Input_detection2 > 2046)
			Input_voltage_detaction2 = 0 ;
}

void adc_btsConnect()
{
	ADC_Init(5,2);
	bts724g_st1 = ADC_Read(5);

	ADC_Init(4,2);
	bts724g_st2 = ADC_Read(4);
}

void adc_pressure()
{
	ADC_Init(14,2);
	adc_value_Air_pressure1=ADC_Read(14);
}

void adc_pressure1()
{
	ADC_Init(6,2);
	adc_value_Air_pressure2 = ADC_Read(6);

	if(abs(adc_value_Air_pressure1 - adc_value_Air_pressure2) >= 161){
		airerrorCount ++ ;
	}
	if(abs(adc_value_Air_pressure1 - adc_value_Air_pressure2) < 161){
		if(airerrorCount != 0){
			airerrorCount -- ;
		}
		else{
			airerrorCount = 0 ;
		}
	}
	if(airerrorCount >= 30){
		airerrorCount = 0 ;
		airerrorCount1 ++ ;
		if(airerrorCount1 >= 30){
			airerrorCount1 = 0 ;
			airerror = true ;
		}
	}
}

void ADC_pressure1()
{
	ADC_Init(6,2);
	adc_value_Air_pressure2=ADC_Read(6);
	if(adc_value_Air_pressure1 > Adc_Sensor_voltage_h || adc_value_Air_pressure2 > Adc_Sensor_voltage_h)
		Sensor_voltage_error = 1 ;
	if(adc_value_Air_pressure1 < Adc_Sensor_voltage_l || adc_value_Air_pressure2 < Adc_Sensor_voltage_l)
		Sensor_voltage_error = 2 ;
	if((adc_value_Air_pressure1 < Adc_Sensor_voltage_h && adc_value_Air_pressure1 > Adc_Sensor_voltage_l)
			&& (adc_value_Air_pressure2 < Adc_Sensor_voltage_h && adc_value_Air_pressure2 > Adc_Sensor_voltage_l))
		Sensor_voltage_error = 0 ;
	if(abs(adc_value_Air_pressure1 - adc_value_Air_pressure2) > 161)
		{
			Sensor_detection_error_count ++ ;
		}
	if(abs(adc_value_Air_pressure1 - adc_value_Air_pressure2) < 161)
		{
			if(Sensor_detection_error_count != 0)
				Sensor_detection_error_count -- ;
			else
				Sensor_detection_error_count = 0 ;
		}
	if(Sensor_detection_error_count >= 30)
	{
		Sensor_detection_error = 1 ;
		Sensor_detection_error_count = 30 ;
	}
	if(Sensor_detection_error_count < 15)
	{
		Sensor_detection_error = 0 ;
	}
}

void ADC_valve()
{
	if(Recover_flag || Venting_flag || Backflow_flag){
		if(electricity_count %2 == 1 && electricity_count <= 9)
		{
		ADC_Init(15,2);
		adc_value_electricity1 = ADC_Read1(15) ;
//		electricity_count++;
		if(adc_value_Input_24V < Adc_Input_voltage_l)
		{
            Adc_Detection_value_h   =   3276 ;
		}
		if(adc_value_Input_24V > Adc_Input_voltage_l && adc_value_Input_24V < Adc_Input_voltage_24)
		{
            Adc_Detection_value_h   =   3276 ;
		}
		if(adc_value_Input_24V > Adc_Input_voltage_24 && adc_value_Input_24V < Adc_Input_voltage_h)
		{
            Adc_Detection_value_h   =   3276 ;
		}
		if(adc_value_electricity1 > 3276)
		{
			if(Recover_flag)
			{
				Recover_error = 2 ;
				Recover_Off();
			}
			if(Venting_flag)
			{
				Venting_error = 2 ;
				Venting_Off();
			}
			if(Backflow_flag)
			{
				Backflow_error = 2 ;
				Backflow_Off();
			}
		}
		else
		{
		adc_value_electricity+=adc_value_electricity1;
		if(electricity_count == 9)
		{
			adc_value_electricity /= 5 ;
		if(adc_value_electricity < Adc_Detection_value_l)
		{
			if(Recover_flag)
				Recover_error = 1 ;
			if(Venting_flag)
				Venting_error = 1 ;
			if(Backflow_flag)
				Backflow_error = 1 ;
		}
		if(adc_value_electricity < Adc_Detection_value_h && adc_value_electricity > Adc_Detection_value_l)
		{
			if(Recover_flag)
				Recover_error = 0 ;
			if(Venting_flag)
				Venting_error = 0 ;
			if(Backflow_flag)
				Backflow_error = 0 ;
		}
		adc_value_electricity = 0 ;
		}
	}
	}
		electricity_count++;
	}

	if(Recover_flag == 0 && Venting_flag == 0 && Backflow_flag == 0)
	{
		electricity_count = 0 ;
	}
}

uint8_t stL = 0 ;
uint8_t stH = 0 ;
uint8_t stX = 0 ;

void adc_cur(){
	ADC_Init(15,2);
	current = ADC_Read1(15) ;
	if(Recover_flag || Venting_flag || Backflow_flag){
		ADC_Init(15,2);
		current = ADC_Read1(15) ;
		if(current >= 1879){//´óÓÚ3A
			if(Recover_flag){
				if(bts724g_st1 <= 1634){
					reShCnt ++ ;
				}
				if(bts724g_st1 >= 3269){
					reShCnt += 5 ;
				}
			}
			if(Venting_flag){
				if(bts724g_st1 <= 1634){
					veShCnt ++ ;
				}
				if(bts724g_st1 >= 3269){
					veShCnt += 5 ;
				}
			}
			if(Backflow_flag){
				if(bts724g_st2 <= 1634){
					flShCnt ++ ;
				}
				if(bts724g_st1 >= 3269){
					flShCnt += 5 ;
				}
			}
		}

	}
}

void cur_check(uint8_t type){
	switch(type){
		case 1 :
			if(reShCnt >= 10){
				reShCnt = 0 ;
				reShFlag = true ;
			}
			else{
				reShCnt = 0 ;
				reShFlag = false ;
			}break ;
		case 2 :
			if(veShCnt >= 10){
				veShCnt = 0 ;
				veShFlag = true ;
			}
			else{
				veShCnt = 0 ;
				veShFlag = false ;
			}break ;
		case 3 :
			if(flShCnt >= 10){
				flShCnt = 0 ;
				flShFlag = true ;
			}
			else{
				flShCnt = 0 ;
				flShFlag = false ;
			}break ;
	}
}
void ADC_temp()
{
	ADC_Init(2,2);
	adc_value_t=ADC_Read(2);
	if(adc_value_t > Adc_T_voltage_h)
		T_voltage_error = 1 ;
	if(adc_value_t < Adc_T_voltage_80)
		T_voltage_error = 0 ;
}

void adc_tempch(){
	ADC_Init(2,2) ;
	controlTemp = ADC_Read(2) ;
}

void adc_outch(){
	ADC_Init(7,2) ;
	outValue = ADC_Read(7) ;
}

void ADC_Detection()
{
//	adc_btsConnect();
	adc_pressure();
	adc_tempch();
	adc_input();
//	ADC_valve();
	adc_cur();
//	ADC_pressure1();
//	adc_outch();
}
