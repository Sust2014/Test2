/*
 * powercheck.c
 *
 *  Created on: 2020年1月5日
 *      Author: Cenx
 */

#include "powercheck.h"

/*上电自检：
 * 电磁阀连接检测、电源电压检测、气压传感器输出电压检测
 * 控制器板温检测、电磁阀短路检测
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
