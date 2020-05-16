/*
 * check.c
 *
 *  Created on: 2019年12月27日
 *      Author: Cenx
 */

#include "check.h"

sysCh Hw[]={
		{0,0,false,false},//电压过高0
		{0,0,false,false},//电压过高恢复1
		{0,0,false,false},//电压过低2
		{0,0,false,false},//电压过低恢复3

		{0,0,false,false},//气压传感器输出电压过高4
		{0,0,false,false},//气压传感器输出电压过高恢复5
		{0,0,false,false},//气压传感器输出电压过低6
		{0,0,false,false},//气压传感器输出电压过低恢复7

		{0,0,false,false},//BTS724GST1/2引脚电压过低8
		{0,0,false,false},//BTS724GST1/2引脚电压过低恢复9
		{0,0,false,false},//BTS724GST1/2引脚电压过高10
		{0,0,false,false},//BTS724GST1/2引脚电压过高恢复11

		{0,0,false,false},//温度电压过高12
		{0,0,false,false},//温度电压过高恢复13

		{0,0,false,false},//输出信号电压过高14
		{0,0,false,false},//输出信号电压过高恢复15
		{0,0,false,false},//输出信号电压过低16
		{0,0,false,false},//输出信号电压过低恢复17

		{0,0,false,false},//电压过低14V

		{0,0,false,false},//恢复供气
		{0,0,false,false},//排气
};

error code[]={
		{0,0,0},
};

void Fun_InputVolH(){
	if(adc_value_Input_24V >= Input_32_Vol){
		Hw[0].start = true ;
		Hw[0].cnt ++ ;
	}
	if(Hw[0].start){
		Hw[0].cycle ++ ;
		if(Hw[0].cycle == 20){
			Hw[0].cycle = 0 ;
			if(Hw[0].cnt >= 15){
				Hw[0].cnt = 0 ;
				Hw[0].flag = true ;
				Hw[0].start = false ;
			}
			else{
				Hw[0].cnt = 0 ;
				Hw[0].start = false ;
			}
		}
	}
	if(Hw[0].flag){
		if(adc_value_Input_24V < Input_32_Vol){
			Hw[1].start = true ;
			Hw[1].cnt ++ ;
		}
		if(Hw[1].start){
			Hw[1].cycle ++ ;
			if(Hw[1].cycle == 20){
				Hw[1].cycle = 0 ;
				if(Hw[1].cnt >= 15){
					Hw[1].cnt = 0 ;
					Hw[1].start = false ;
					Hw[0].flag = false ;
				}
				else{
					Hw[1].cnt = 0 ;
					Hw[1].start = false ;
				}
			}
		}
	}
}

void Fun_InputVolL(){
	if(adc_value_Input_24V <= Input_18_Vol){
		Hw[2].start = true ;
		Hw[2].cnt ++ ;
	}
	if(Hw[2].start){
		Hw[2].cycle ++ ;
		if(Hw[2].cycle == 20){
			Hw[2].cycle = 0 ;
			if(Hw[2].cnt >= 15){
				Hw[2].cnt = 0 ;
				Hw[2].flag = true ;
				Hw[2].start = false ;
			}
			else{
				Hw[2].cnt = 0 ;
				Hw[2].start = false ;
			}
		}
	}

	if(adc_value_Input_24V <= Input_14_Vol){
		Hw[18].start = true ;
		Hw[18].cnt ++ ;
	}
	if(Hw[18].start){
		Hw[18].cycle ++ ;
		if(Hw[18].cycle == 20){
			Hw[18].cycle = 0 ;
			if(Hw[18].cnt >= 15){
				Hw[18].cnt = 0 ;
				Hw[18].flag = true ;
				Hw[18].start = false ;
			}
			else{
				Hw[18].cnt = 0 ;
				Hw[18].start = false ;
			}
		}
	}
	if(Hw[2].flag || Hw[18].flag){
		if(adc_value_Input_24V > Input_18_Vol){
			Hw[3].start = true ;
			Hw[3].cnt ++ ;
		}
		if(Hw[3].start){
			Hw[3].cycle ++ ;
			if(Hw[3].cycle == 20){
				Hw[3].cycle = 0 ;
				if(Hw[3].cnt >= 15){
					Hw[3].cnt = 0 ;
					Hw[3].start = false ;
					Hw[2].flag = false ;
					Hw[18].flag = false ;
				}
				else{
					Hw[3].cnt = 0 ;
					Hw[3].start = false ;
				}
			}
		}
	}
}

void Fun_sensorVolH(){
	if(adc_value_Air_pressure1 >= Sensor_VolH){
		Hw[4].start = true ;
		Hw[4].cnt ++ ;
	}
	if(Hw[4].start){
		Hw[4].cycle ++ ;
		if(Hw[4].cycle == 20){
			Hw[4].cycle = 0 ;
			if(Hw[4].cnt >= 15){
				Hw[4].cnt = 0 ;
				Hw[4].flag = true ;
				Hw[4].start = false ;
			}
			else{
				Hw[4].cnt = 0 ;
				Hw[4].start = false ;
			}
		}
	}

	if(Hw[4].flag){
		if(adc_value_Air_pressure1 < Sensor_VolRh){
			Hw[5].start = true ;
			Hw[5].cnt ++ ;
		}
		if(Hw[5].start){
			Hw[5].cycle ++ ;
			if(Hw[5].cycle == 20){
				Hw[5].cycle = 0 ;
				if(Hw[5].cnt >= 15){
					Hw[5].cnt = 0 ;
					Hw[5].start = false ;
					Hw[4].flag = false ;
				}
				else{
					Hw[5].cnt = 0 ;
					Hw[5].start = false ;
				}
			}
		}
	}
}

void Fun_sensorVolL(){
	if(adc_value_Air_pressure1 <= Sensor_VolL){
		Hw[6].start = true ;
		Hw[6].cnt ++ ;
	}
	if(Hw[6].start){
		Hw[6].cycle ++ ;
		if(Hw[6].cycle == 20){
			Hw[6].cycle = 0 ;
			if(Hw[6].cnt >= 15){
				Hw[6].cnt = 0 ;
				Hw[6].flag = true ;
				Hw[6].start = false ;
			}
			else{
				Hw[6].cnt = 0 ;
				Hw[6].start = false ;
			}
		}
	}

	if(Hw[6].flag){
		if(adc_value_Air_pressure1 > Sensor_VolRl){
			Hw[7].start = true ;
			Hw[7].cnt ++ ;
		}
		if(Hw[7].start){
			Hw[7].cycle ++ ;
			if(Hw[7].cycle == 20){
				Hw[7].cycle = 0 ;
				if(Hw[7].cnt >= 15){
					Hw[7].cnt = 0 ;
					Hw[7].start = false ;
					Hw[6].flag = false ;
				}
				else{
					Hw[7].cnt = 0 ;
					Hw[7].start = false ;
				}
			}
		}
	}
}

void Fun_valveConnect(){
	if(Recover_flag == false && Venting_flag == false
			&& Backflow_flag == false){
		if(bts724g_st1 <= Connect_VolL){
			Hw[8].start = true ;
			Hw[8].cnt ++ ;
		}
		if(Hw[8].start){
			Hw[8].cycle ++ ;
			if(Hw[8].cycle == 10){
				Hw[8].cycle = 0 ;
				if(Hw[8].cnt >= 7){
					Hw[8].cnt = 0 ;
					Hw[8].flag = true ;
					Hw[8].start = false ;
				}
				else{
					Hw[8].cnt = 0 ;
					Hw[8].start = false ;
				}
			}
		}

		if(Hw[8].flag){
			if(bts724g_st1 >= Connect_VolRl){
				Hw[9].start = true ;
				Hw[9].cnt ++ ;
			}
			if(Hw[9].start){
				Hw[9].cycle ++ ;
				if(Hw[9].cycle == 10){
					Hw[9].cycle = 0 ;
					if(Hw[9].cnt >= 7){
						Hw[9].cnt = 0 ;
						Hw[9].start = false ;
						Hw[8].flag = false ;
					}
					else{
						Hw[9].cnt = 0 ;
						Hw[9].start = false ;
					}
				}
			}
		}

		if(bts724g_st2 <= Connect_VolL){
			Hw[10].start = true ;
			Hw[10].cnt ++ ;
		}
		if(Hw[10].start){
			Hw[10].cycle ++ ;
			if(Hw[10].cycle == 10){
				Hw[10].cycle = 0 ;
				if(Hw[10].cnt >= 7){
					Hw[10].cnt = 0 ;
					Hw[10].flag = true ;
					Hw[10].start = false ;
				}
				else{
					Hw[10].cnt = 0 ;
					Hw[10].start = false ;
				}
			}
		}

		if(Hw[10].flag){
			if(bts724g_st2 >= Connect_VolRl){
				Hw[11].start = true ;
				Hw[11].cnt ++ ;
			}
			if(Hw[11].start){
				Hw[11].cycle ++ ;
				if(Hw[11].cycle == 10){
					Hw[11].cycle = 0 ;
					if(Hw[11].cnt >= 7){
						Hw[11].cnt = 0 ;
						Hw[11].start = false ;
						Hw[10].flag = false ;
					}
					else{
						Hw[11].cnt = 0 ;
						Hw[11].start = false ;
					}
				}
			}
		}
	}
}

void Fun_OpenValve(){
	if(Hw[8].flag){
		Recover_On();
		delay_s(1);
//		ADC_Init(15,2);
//		current = ADC_Read1(15) ;
		if(current >= Cur_VolL){
			VentOpen = true ;
		}
		if(current < Cur_VolL){
			if(bts724g_st1 <= Connect_VolL){
				st1_2Open = true ;
			}
			if(bts724g_st1 >= Connect_VolRl){
				RecoOpen = true ;
			}
		}
		delay_s(1);
		Recover_Off();
		delay_s1(11);//当输入为低后，BTS724G的ST引脚电压需要0.001s后变化 0.01s为检测时间
		while(Hw[8].flag){
			errorCon = true ;
		}
		errorCon = false ;
		RecoOpen = false ;
		VentOpen = false ;
		st1_2Open = false ;
	}

	if(Hw[10].flag){
		Backflow_On();
		delay_s1(100);
//		ADC_Init(15,2);
//		current = ADC_Read1(15) ;
		if(current >= Cur_VolL){
			Out4Open = true ;
		}
		if(current < Cur_VolL){
			if(bts724g_st2 <= Connect_VolL){
				st3_4Open = true ;
			}
			if(bts724g_st2 >= Connect_VolRl){
				BackOpen = true ;
			}
		}
		delay_s1(200);
		Backflow_Off();
		delay_s1(11);//当输入为低后，BTS724G的ST引脚电压需要0.001s后变化 0.01s为检测时间
		while(Hw[10].flag){
			errorCon = true ;
		}
		BackOpen = false ;
		Out4Open = false ;
		st3_4Open = false ;
		errorCon = false ;
	}

//	if(Hw[10].flag == false){
//		BackOpen = false ;
//	}
}

void Fun_TempH(){
	if(controlTemp >= Temp_VolH){
		Hw[12].start = true ;
		Hw[12].cnt ++ ;
	}
	if(Hw[12].start){
		Hw[12].cycle ++ ;
		if(Hw[12].cycle == 20){
			Hw[12].cycle = 0 ;
			if(Hw[12].cnt >= 15){
				Hw[12].cnt = 0 ;
				Hw[12].flag = true ;
				Hw[12].start = false ;
			}
			else{
				Hw[12].cnt = 0 ;
				Hw[12].start = false ;
			}
		}
	}

	if(Hw[12].flag){
		if(controlTemp <= Temp_VolRh){
			Hw[13].start = true ;
			Hw[13].cnt ++ ;
		}
		if(Hw[13].start){
			Hw[13].cycle ++ ;
			if(Hw[13].cycle == 20){
				Hw[13].cycle = 0 ;
				if(Hw[13].cnt >= 15){
					Hw[13].cnt = 0 ;
					Hw[13].start = false ;
					Hw[12].flag = false ;
				}
				else{
					Hw[13].cnt = 0 ;
					Hw[13].start = false ;
				}
			}
		}
	}
}

void Fun_ShortCh(){
	Disable_Interrupt(FTM2_IRQn);

	uint16_t i = 0 ;
	uint16_t stH = 0 ;
	uint16_t stL = 0 ;

	delay_s1(2);
	adc_cur();
	if(current >= Cur_VolL && current <= Cur_VolH){
		if(Recover_flag){
			RecoShort = false ;
		}
		if(Venting_flag){
			VentShort = false ;
		}
		if(Backflow_flag){
			BackShort = false ;
		}
	}
	if(current > Cur_VolH){
		delay_s1(4);//开启后0.02s
		for(i = 0 ; i < 300 ; i ++){
			ADC_Init(5,2);
			bts724g_st1 = ADC_Read(5);
			if(bts724g_st1 < Connect_VolL)
				stL ++ ;
			if(bts724g_st1 > Connect_VolRl)
				stH ++ ;
//			delay_us(50);
		}
//		Venting_Off();
		if(stH >= 10 && stL >= 200){
			if(Recover_flag){
				RecoShort = true ;
			}
			if(Venting_flag){
				VentShort = true ;
			}
			if(Backflow_flag){
				BackShort = true ;
			}
//			Venting_Off();
			stH = 0 ;
			stL = 0 ;
		}
	}

	Enable_Interrupt(FTM2_IRQn);
}

void Fun_OutputCh(){
	/*开启输出 检测输出电压信号是否低于4V*/
	if(Control_flag){
		if(outValue < Out_VolH){
			Hw[14].start = true ;
			Hw[14].cnt ++ ;
		}
		if(Hw[14].start){
			Hw[14].cycle ++ ;
			if(Hw[14].cycle == 20){
				Hw[14].cycle = 0 ;
				if(Hw[14].cnt >= 15){
					Hw[14].cnt = 0 ;
					Hw[14].flag = true ;
					Hw[14].start = false ;
				}
				else{
					Hw[14].cnt = 0 ;
					Hw[14].start = false ;
				}
			}
		}

		if(Hw[14].flag){
			if(outValue >= Out_VolH){
				Hw[15].start = true ;
				Hw[15].cnt ++ ;
			}
			if(Hw[15].start){
				Hw[15].cycle ++ ;
				if(Hw[15].cycle == 20){
					Hw[15].cycle = 0 ;
					if(Hw[15].cnt >= 15){
						Hw[15].cnt = 0 ;
						Hw[15].start = false ;
						Hw[14].flag = false ;
					}
					else{
						Hw[15].cnt = 0 ;
						Hw[15].start = false ;
					}
				}
			}
		}
	}

	/*关闭输出 检测输出电压信号是否大于2V*/
	if(Control_flag == false){
		if(outValue > Out_VolL){
			Hw[16].start = true ;
			Hw[16].cnt ++ ;
		}
		if(Hw[16].start){
			Hw[16].cycle ++ ;
			if(Hw[16].cycle == 20){
				Hw[16].cycle = 0 ;
				if(Hw[16].cnt >= 15){
					Hw[16].cnt = 0 ;
					Hw[16].flag = true ;
					Hw[16].start = false ;
				}
				else{
					Hw[16].cnt = 0 ;
					Hw[16].start = false ;
				}
			}
		}

		if(Hw[16].flag){
			if(outValue <= Out_VolL){
				Hw[17].start = true ;
				Hw[17].cnt ++ ;
			}
			if(Hw[17].start){
				Hw[17].cycle ++ ;
				if(Hw[17].cycle == 20){
					Hw[17].cycle = 0 ;
					if(Hw[17].cnt >= 15){
						Hw[17].cnt = 0 ;
						Hw[17].start = false ;
						Hw[16].flag = false ;
					}
					else{
						Hw[17].cnt = 0 ;
						Hw[17].start = false ;
					}
				}
			}
		}
	}
}

/*排出管内气体*/
void Fun_dischargeAir(){
	Venting_On();
	delay_s(2);
	Venting_Off();

	delay_s1(20);

	Recover_On();
	delay_s(2);
	Recover_Off();

	delay_s1(20);
}
void Fun_HwCheck(){
	Fun_InputVolH();
	Fun_InputVolL();
	Fun_sensorVolH();
	Fun_sensorVolL();
	Fun_valveConnect();
	Fun_TempH();
	//Fun_OutputCh();
}
void Fun_led(){
	/*电源电压过高*/
	if(Hw[0].flag){
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	/*电源电压过低*/
	if(Hw[2].flag && Hw[18].flag == false){
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	if(Hw[2].flag && Hw[18].flag){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	/*气压传感器输出电压过高*/
	if(Hw[4].flag){
		Recover_On();
		delay_s(2);
		Recover_Off();
		while(Hw[4].flag){
			LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
			LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
			delay_s1(500);
		}
	}

	/*气压传感器输出电压过低*/
	if(Hw[6].flag){
		Recover_On();
		delay_s(2);
		Recover_Off();
		while(Hw[6].flag){
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
		}
	}

	/*控制器温度过高*/
	if(Hw[12].flag){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	/*输出信号电压过高*/
	if(Hw[16].flag){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	/*输出信号电压过低*/
	if(Hw[14].flag){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	if(reShFlag){
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	if(veShFlag){
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	if(flShFlag){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}

	if(0){
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}
}

/* 故障输出函数
 * 作用：当检测到故障时，通过OUT4输出波形（cycle=0.5s h=0.1s l=0.4s）
 * 故障类型：
 *       	气压传感器输出电压过低Hw[6].flag
 *       	气压传感器输出电压过高Hw[4].flag
 *       	电源电压低于14VHw[18].flag
 *       	电磁阀连接故障errorCon
 * 当检测到故障恢复，OUT4输出低电平
 */
void Fun_ErrorLed(){
	if(Hw[6].flag || Hw[4].flag
			|| Hw[18].flag || errorCon || BackOpen){
		code[0].state = true ;
		if(code[0].cycle == 0){
			Control_On();
			code[0].cnt_h ++ ;
			if(code[0].cnt_h == 11){
				code[0].cnt_h = 0 ;
				code[0].cycle = 1 ;
			}
		}
		if(code[0].cycle == 1){
			Control_Off();
			code[0].cnt_l ++ ;
			if(code[0].cnt_l == 39){
				code[0].cnt_l = 0 ;
				code[0].cycle = 0 ;
			}
		}
	}
	if(Hw[6].flag == false && Hw[4].flag == false
			&& Hw[18].flag == false && errorCon == false && BackOpen == false){
		if(code[0].state){
			code[0].state = false ;
			Control_Off();
		}
		code[0].cycle = 0 ;
		code[0].cnt_h = 0 ;
		code[0].cnt_l = 0 ;
	}
}
