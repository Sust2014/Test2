/*
 * powercheck.c
 *
 *  Created on: 2020��1��5��
 *      Author: Cenx
 */

#include "powercheck.h"

/*�ϵ��Լ죺
 * ��ŷ����Ӽ�⡢��Դ��ѹ��⡢��ѹ�����������ѹ���
 * ���������¼�⡢��ŷ���·���
 */
void Fun_HwSelfCh(){
//	delay_s1(210);
	if(Hw[4].flag){
		Recover_On();
		delay_s(2);
		Recover_Off();
		while(Hw[4].flag);
	}

	if(Hw[6].flag){
		Recover_On();
		delay_s(2);
		Recover_Off();
		while(Hw[6].flag);
	}
	Fun_OpenValve();

	Venting_On();
	delay_s1(10);
	Venting_Off();

	Backflow_On();
	delay_s1(10);
	Backflow_Off();

	Recover_On();
	delay_s1(10);
	Recover_Off();

	delay_s1(20);
}
