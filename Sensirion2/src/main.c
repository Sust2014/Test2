/*
 * Created on: 2019.7.29
 * Author: byxzwz
 * Version : Dryer1 - 29WDOG
 * ��һ�汾Ϊ Dryer1 - 24WDOG
 * �������ݣ�
 * 1. ����⵽���� ��Ӧ�Ĺ��ϼ���++ �� �����ϼ���> �� �� LEDѭ����˸���жϹ��ϼ����Ƿ�����> �� �� ����⵽���ϼ���< �� ��
 * �˳�LEDѭ����˸
 * 2. ��ŷ����������жϺ͹��������ж�һ�����ж��Ǹ��Ż��ǵ�ŷ�����޵���
 * 3. �޸ĵ��������򣬵���⵽�޵������ж��Ƿ������ڵ�ŶϿ�����ģ�����ǵ�ŷ��Ͽ�������ִ�к�ɫLED��˸��������ִ��
 * �����ŷ���������������ִ�к�ɫLED��˸�����¿�����ŷ��ȵ���ŷ����������������������ִ��
 * ����⵽��ŷ������������󣬶Ͽ���Ӧ��ŷ�����ɫLED��˸
 *
 */

#include "utility_function.h"
#include "fault_led.h"
#include "can.h"
#include "i2c.h"
#include "check.h"
#include "powercheck.h"
#include "systick.h"
#include "sht3x.h"

/* �ж�ѹ����11��0.2bar
 * �ָ�����ѹ����8.5��0.1bar
 * ����ʱ�䣺20s
 * ���������>15�֣�ͬʱ��ѹ>7bar������ǿ�ƻ���10s
 * ���������>25�֣�ͬʱ��ѹ>6.5bar������ǿ�ƻ���10s
 * ���������ݣ�
 * 			�����/��ѹ��50W/24V DC
 * 			�����������¶ȣ�7���6��
 * 			�������Ͽ��¶ȣ�29.5���3��
 * 			����ʱ������2A��������ʱ0.03A
 */
//#define      VOLTAGE_CONTROL
#define      STANDARD_CONTROL

uint8_t test_data = 0 ;
uint8_t test_data_1 = 0 ;

int main(void)
{
    ft	temperature;        // temperature [��C]
    ft	humidity;	        // relative humidity [%RH]
    etError   error;        // error code

	System_Init();//ϵͳInit
	Calculation() ;
	SHT3X_Init(0x44);
	task_ms(50);
	/*�ϵ��Լ�*/
	for(;;)
	{
		//Fun_AutoControl(2);
		SHT3X_GetTempAndHumi(&temperature, &humidity, REPEATAB_HIGH, MODE_CLKSTRETCH, 100);
	}
	return 0;
}

