/*
 * fault_led.c
 *
 *  Created on: 2019年6月24日
 *      Author: Cenx
 */

#include "fault_led.h"
#include "i2c.h"

extern uint8_t Air_pressure_12;//气压值达到12
void LED_G_Flicker()
{
//	if(Led_R_OpenFlag == 0)
//	{
		if(Input_voltage_detaction1 == 0 && Input_voltage_detaction2 == 0)
		{
			if(Input_voltage_24V_error == 0 && Sensor_voltage_error == 0)
			{
				if(Sensor_detection_error == 0 && T_voltage_error == 0)
				{
					if(Recover_error == 0 && Venting_error == 0 && Backflow_error == 0)
					{
						if(LED_R_12_flag == 0 && Air_pressure_l_count2 == 0 && Low_pressure_1 == 0)
							LED_G_ON ;
						else
							LED_G_OFF ;
					}
					else
						LED_G_OFF ;
				}
				else
					LED_G_OFF ;
			}
			else
				LED_G_OFF ;
		}
		else
			LED_G_OFF ;
//	}
}

void LED_R_Flicker()
{
	I2C_data_12_flag();
//	uint8_t i = 0 ;
	//当检测到压力连续超过1.2Mpa约5s时，同时通过电磁阀的电流检测确认电磁阀为打开状态，即红灯闪烁，报故障。
	while(LED_R_12_flag)
	{
//		Led_R_OpenFlag = 1 ;
			LED_R_ON;
			delay_s1(500);
			LED_R_OFF;
			delay_s1(500);

			if(adc_value_Air_pressure1 < Air_pressure_up_value_error)
			{
				LED_R_12_flag = false ;
				Air_pressure_12 = 0 ;
				LED_R_OFF ;
//				Led_R_OpenFlag = 0 ;
			}
	}

//	if(Air_pressure_l_count2 == 1 && count_9 == 0)
//	{
////		Led_R_OpenFlag = 1 ;
//		count_9 = 1 ;
//			LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
//			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
//			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
//			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
////			Led_R_OpenFlag = 0 ;
//	}
}

void Input_voltage_Detection()
{
	I2C_data_inputvo();
	if(Input_voltage_24V_error)
	{
//		Led_R_OpenFlag = 1 ;
		while(Input_voltage_24V_error == 1)
		{
				Input_vo = true ;
			if(Inputvo_count %300 == 0)
			{
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			}
		}

		while(Input_voltage_24V_error == 2)
		{
				Input_vo = true ;
			if(Inputvo_count %300 == 0)
			{
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
			}
		}
	}
//	Led_R_OpenFlag = 0 ;
	Input_vo = false ;
	Inputvo_count = 0 ;
}

void Sensor_Detection()
{
	if(Sensor_detection_error)
	{
//		Led_R_OpenFlag = 1 ;
		ForeceRecover();
		while(Sensor_detection_error)
		{
			Sensorvo1 = true ;
			if(Sensorvo_count1 % 300 == 0)
			{
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			}
		}
	}

//	Led_R_OpenFlag = 0 ;

	Sensorvo1 = false ;
	Sensorvo_count1 = 0 ;
}
void sensor_Voltage_Detection()
{
	I2C_data_sensorvo();
	if(Sensor_voltage_error)
	{
//		Led_R_OpenFlag = 1 ;
		ForeceRecover();

		while(Sensor_voltage_error == 1)
		{
			Sensorvo = true ;
			if(Sensorvo_count %300 == 0)
			{
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			}
		}

		while(Sensor_voltage_error == 2)
		{
			Sensorvo = true ;
			if(Sensorvo_count %300 == 0)
			{
					LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
			}
		}
	}
//	Led_R_OpenFlag = 0 ;
	Sensorvo = false ;
	Sensorvo_count = 0 ;
}

void solenoid_Voltage_Detection()
{
	if(Recover_error)
	{
		if(Recover_error == 1)
		{
			Recover_Off();
			delay_s1(1);
			ADC_connect_1();
			I2C_solenoid_valve();
			if(Input_voltage_detaction2 == 0)
				while(Recover_error == 1)
				{
						Solenoidvo = true ;
						if(Solenoidvo_count % 300 == 0)
						{
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							Recover_On();
							delay_s1(200);
							Recover_Off();
						}
				}
			Recover_flag = false ;

			while(Recover_error == 1)
			{
					Solenoidvo = true ;
					if(Solenoidvo_count % 300 == 0)
					{
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					}
			}
			Recover_flag = false ;
		}

		if(Recover_error == 2)
		{
			Recover_Off();
			I2C_solenoid_valve();
			while(Recover_error == 2)
			{
					Solenoidvo = true ;
					if(Solenoidvo_count % 300 == 0)
					{
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
					}
			}
			Recover_flag = false ;
		}
	}

	if(Venting_error)
	{
		if(Venting_error == 1)
		{
			Venting_Off();
			delay_s1(1);
			ADC_connect_1();
			I2C_solenoid_valve();
			if(Input_voltage_detaction2 == 0)
			{
				while(Venting_error == 1)
				{
						Solenoidvo = true ;
						if(Solenoidvo_count % 300 == 0)
						{
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							Venting_On();
							delay_s1(200);
							Venting_Off();
						}
				}
			}
			Venting_flag = false ;

			if(Input_voltage_detaction2 == 1)
			{
				ForeceRecover();
				while(Venting_error == 1)
				{
						Solenoidvo = true ;
						if(Solenoidvo_count % 300 == 0)
						{
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						}
				}
			}
			Venting_flag = false ;
		}

		if(Venting_error == 2)
		{
			Venting_Off();
			I2C_solenoid_valve();
			ForeceRecover();
			while(Venting_error == 2)
			{
				Solenoidvo = true ;
				if(Solenoidvo_count % 300 == 0)
				{
					LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				}
			}
			Venting_flag = false ;
		}
	}

	if(Backflow_error)
	{
		if(Backflow_error == 1)
		{
			Backflow_Off();
			delay_s1(1);
			ADC_connect_2();
			I2C_solenoid_valve();
			if(Input_voltage_detaction1 == 0)
			{
				while(Backflow_error == 1)
				{
						Solenoidvo = true ;
						if(Solenoidvo_count % 300 == 0)
						{
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							Backflow_On();
							delay_s1(200);
							Backflow_Off();
						}
				}
			}
			Backflow_flag = false ;

			if(Input_voltage_detaction1 == 0)
			{
				while(Backflow_error == 1)
				{
						Solenoidvo = true ;
						if(Solenoidvo_count % 300 == 0)
						{
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
							LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
						}
				}
			}
			Backflow_flag = false ;
		}

		if(Backflow_error == 2)
		{
			Backflow_Off();
			I2C_solenoid_valve();
			ForeceRecover();
			while(Backflow_error == 2)
			{
				Solenoidvo = true ;
				if(Solenoidvo_count % 300 == 0)
				{
					LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
					LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				}
			}
			Backflow_flag = false ;
		}
	}
	Solenoidvo = false ;
	Solenoidvo_count = 0 ;
}

void T_voltage_Detection()
{
	I2C_data_tvo();
	if(T_voltage_error)
	{
//		Led_R_OpenFlag = 1 ;
		ForeceRecover();

		while(T_voltage_error==1)
		{
			Tvo = true ;
			if(Tvo_count %300 == 0)
			{
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			}
		}

		while(T_voltage_error==2)
		{
			Tvo = true ;
			if(Tvo_count %300 == 0)
			{
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
				LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
			}
		}
	}
//	Led_R_OpenFlag = 0 ;
	Tvo = false ;
	Tvo_count = 0 ;
}
