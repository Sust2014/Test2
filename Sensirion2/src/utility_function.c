/*
 * utility_function.c
 *
 *  Created on: 2019��5��17��
 *      Author: Cenx
 */

#include "utility_function.h"
#include "stdlib.h"
#include "check.h"

/*��ѹ����*/
uint32_t Air_pressure[20][2]={{1,573},{2,737},{3,901},{4,1065},{5,1229},{6,1392},{7,1556},{8,1720},
		{9,1883},{10,2043},{11,2211},{12,2375},{13,2539},{14,2703},{15,2867},{16,3030},{17,3194},
		{18,3358},{19,3522},{20,3686}
};

/*��¼��������ж��ʱ��ѹֵ*/
uint16_t airInterUnload ;

void Fun_Inflatable(uint8_t type){
	uint8_t count = 0 ;
	switch(type){
	case 1 :
		if(type1Flag){
			type1Flag = false ;
			Venting_On();
			while(Gpio_Input_Detection(Connect_gpio1,Input_voltage) == 1 && count < 30){
				task_ms(10);
				count ++ ;
			}
			count = 0 ;
			Venting_Off();
		}break;
	case 2 ://����������ж�ɺ� �����ѹ�Ƿ����� �ж�й©�ٶ� ����������ŷ�����
		//��¼��������ж��ʱ����ѹ �����ѹ�Ƿ�����
		if(Forcedloading){
			if(adc_value_Air_pressure1 > airInterUnload){
				if((adc_value_Air_pressure1 - airInterUnload) > 161
						&& Gpio_Input_Detection(Connect_gpio1,Input_voltage) == 1){
					Venting_On();
					delay_s(2);
					Venting_Off();
				}
			}
		}break;
	}
}

void Fun_ForceControl(uint8_t Open_t){
	if(Gpio_Input_Detection(Connect_gpio1,Input_voltage) == 1){//��⵽�����ź�Ϊ�ߵ�ƽ
		Low_level_flag = true ;
		if(adc_value_Air_pressure1 >= Force_Venting
				&& adc_value_Air_pressure1 <= Air_pressure_up_value){
			h_unloadMode = 1 ;
			delay_s1(210);
			h_unloadMode = 0 ;
			if(adc_value_Air_pressure1 >= Force_Venting && adc_value_Air_pressure1 <= Air_pressure_up_value
					&& Forcedloading == false && h_unloadCount >= 15){
				h_unloadCount = 0 ;
				Venting_On();
				////Fun_ShortCh();
				delay_s(2);
				Venting_Off();
				cur_check(2);
				delay_s1(10);
				Forcedloading = true ;
				second = 0 ;
				min = 0 ;

				/*��ѹ���������׶� ��һ�μ�⵽�ߵ�ƽ*/
				/*���ߵ�ƽ�����ص�ʱ�� ��⵽��ѹ�������޹��� ��Դ��ѹ�޹��ϣ�<14V��
				 * �������� ����ߵ�ƽ
				 * ����й��� ������ߵ�ƽ
				 * ��ѹ���������� ��Դ��ѹ�����Ѿ��ø��������ư岻����������
				 * �����Ѿ���ִ��
				 */
				if(PressureRise == 0){
					Backflow_On();
					Control_On();
					////Fun_ShortCh();
					delay_s1(20);//����ʱ�����¿����ж��ṩ��Ӧʱ��
					/*����8s*/
					while(Gpio_Input_Detection(Connect_gpio1,Input_voltage) == 1
							&& BackflowTime < 507 && adc_value_Air_pressure1 >= Air_pressure_up_value1){
						delay_s1(10);
						BackflowTime ++ ;
					}
					BackflowTime = 0 ;
					Backflow_Off();
					cur_check(3);
					delay_s1(10);
					Control_Off();

					PressureRise = 1 ;
				}
			}
			h_unloadCount = 0 ;
		}

		//��⵽�ߵ�ƽ����ѹ�����趨ֵ���ָ�������ŷ�����
		if(adc_value_Air_pressure1 < Air_pressure_up_value1){
			h_RecovMode = 1 ;
			delay_s1(210);
			h_RecovMode = 0 ;
			if(adc_value_Air_pressure1 < Air_pressure_up_value1 && h_RecovCount >= 15){
				h_RecovCount = 0 ;
				Recover_On();
				////Fun_ShortCh();
				delay_s(2);
				Recover_Off();
				cur_check(1);
				delay_s1(10);
				Forcedloading = false ;
			}
			h_RecovCount = 0 ;
		}

		//��⵽�ߵ�ƽ����ѹ�����趨ֵ��������ŷ�����
		if(adc_value_Air_pressure1 > Air_pressure_up_value){
			h_VentMode = 1 ;
			delay_s1(210);
			h_VentMode = 0 ;
			if(adc_value_Air_pressure1 > Air_pressure_up_value && h_VentCount >= 15){
				h_VentCount = 0 ;
				Venting_On();
				////Fun_ShortCh();
				delay_s(2);
				Venting_Off();
				cur_check(2);
				delay_s1(10);
				Forcedloading = false ;
			}
			h_VentCount = 0 ;
		}
	}

	if(Gpio_Inputl(Connect_gpio1,Input_voltage)){//���Ϊ�͵�ƽ
		Forcedloading = false ;
		PressureRise = 0 ;
		BackflowTime = 0 ;

		if(Low_level_flag == true)//�ߵ�ƽ��ת���͵�ƽ�ָ�����
		{
			Low_level_flag = false ;
			Recover_On();
			////Fun_ShortCh();
			delay_s(2);
			Recover_Off();
			cur_check(1);
			delay_s1(10);
		}

		Fun_AutoControl(2);
	}
}


/************************************
 * ��������Fun_AutoControl
 * ���ܣ�ȷ����ѹ״̬��ִ�ж�Ӧ����
 * ���룺Open_t��ŷ�����ʱ��
 * �������
 * ˵����
 *    ȷ����ѹ״̬��ִ�ж�Ӧ����
 *    ��ִ�лָ���������������ѹ<�ָ�����ѹ����10s������ִ�лָ���������
 ************************************/
void Fun_AutoControl(uint8_t Open_t)
{
	/*�ָ�����ѹ����8.5��0.1bar*/
	if(adc_value_Air_pressure1 <= Air_pressure_up_value1){
		RecoverMode = 1 ;
		delay_s1(210);
		RecoverMode = 0 ;
		//����⵽��ѹ�����趨ֵ���ָ�������ŷ�������5s�������ѹ�������趨ֵ���ָ�������ŷ����¶���
		if(adc_value_Air_pressure1 <= Air_pressure_up_value1
				&& reTiming == false && RecoverCount >= 15){
			RecoverCount = 0 ;
			InTheRecover = 1 ;
			Air_pressure_l_count1 = 1;

			Recover_On();
			delay_s(2);
			Recover_Off();
			Control_On();
			cur_check(1);
			delay_s1(10);

			reTiming = true ;
			second = 0 ;
			min = 0 ;
		}
		RecoverCount = 0 ;
	}

	//����⵽��ѹ�����趨ֵ�����ָ�������ŷ�������־=0
	if(adc_value_Air_pressure1 > Air_pressure_up_value1){
		reTiming = false ;
	}
	if(adc_value_Air_pressure1 < Air_pressure_up_value){
		veTiming = false ;
	}

	/*�ж�ѹ����11��0.2bar*/
	if(adc_value_Air_pressure1 >= Air_pressure_up_value){
		VentingMode = 1 ;
		delay_s1(210);
		VentingMode = 0 ;
		//����⵽��ѹ�����趨ֵ��������ŷ��ͻ�����ŷ�����
		if(adc_value_Air_pressure1 >= Air_pressure_up_value
				&& veTiming == false && VentingCount >= 15){
			VentingCount = 0 ;

			Venting_On();
			delay_s(2);
			Venting_Off();
			cur_check(2);
			delay_s1(25);

			Control_Off();

			Backflow_On();
			while(adc_value_Air_pressure1 >= (Air_pressure_up_value1-50)) ;
//			delay_s(15);//����ʱ�䣺20s
			Backflow_Off();
			cur_check(3);
			delay_s1(10);

			veTiming = true ;
			second = 0 ;
			min = 0 ;
			InTheRecover = 0 ;
		}

		VentingCount = 0 ;
	}
}

/*���������>15�֣�ͬʱ��ѹ>7bar������ǿ�ƻ���10s*/
/*���������>25�֣�ͬʱ��ѹ>6.5bar������ǿ�ƻ���10s*/
void Backflow_detection()
{
	if(Forcedbackflowflag_15 == true){
		Forcedbackflowflag_15 = false ;
		min = 16 ;
		if(adc_value_Air_pressure1 >= Backflow_up_value
				&& InTheRecover == 0 && Control_flag == false){

			BackflowMode15 = 1 ;
			delay_s1(210);
			BackflowMode15 = 0 ;

			if(BackflowCount15 >= 15){
				BackflowCount15 = 0 ;

				Venting_On();
				delay_s(2);
				Venting_Off();
				cur_check(2);

				delay(252777);

				Backflow_On();
				delay_s(10);
				Backflow_Off();
				cur_check(3);
				delay_s1(10);

				second = 0 ;
				min = 0 ;
			}

			BackflowCount15 = 0 ;
		}
	}

	if(Forcedbackflowflag_25 == true)
	{
		Forcedbackflowflag_25 = false ;
		min = 0 ;
		if(adc_value_Air_pressure1 >= Backflow_up_value1 && InTheRecover == 0
				&& Control_flag == false)
		{
			BackflowMode30 = 1 ;
			delay_s1(210);
			BackflowMode30 = 0 ;

			if(BackflowCount30 >= 15){
				BackflowCount30 = 0 ;

				Venting_On();
				delay_s(2);
				Venting_Off();
				cur_check(2);

				delay_s1(100);

				Backflow_On();
				delay_s(10);
				Backflow_Off();
				cur_check(3);
				delay_s1(10);
				second = 0 ;
				min = 0 ;
			}

			BackflowCount30 = 0 ;
		}
	}
}

uint8_t Air_pressure_led(uint32_t t)//���ADCֵ��Ӧ����ѹֵ
{
	uint32_t t_value=0;
	uint32_t t_value_l=0;uint32_t i = 1 ;
	t_value_l=abs(t-Air_pressure[0][1]);
	t_value=Air_pressure[0][0];
	for(i = 1 ; i < 20 ; i++)
	{
		if(abs(t-Air_pressure[i][1]) < t_value_l)
		{
			t_value_l=abs(t-Air_pressure[i][1]);
			t_value=Air_pressure[i][0];
		}
		else
			t_value+=0;
	}
	return t_value;
}

void Detect_high_level()
{
	Fun_AutoControl(2);//��ѹ���
//	Backflow_detection();//ǿ�ƻ������
}
