/*
 * led.c
 *
 *  Created on: 2020年1月8日
 *      Author: Cenx
 */

#include "led.h"

errorSt s_LED[] = {
		{0,0,50},
};

uint8_t errCount = 1 ;

uint8_t errInVH[3]={0,0,0} ;
uint8_t errInVL[3]={0,0,1} ;
uint8_t errIn14VL[3]={1,1,0} ;
uint8_t errSeVH[3]={0,1,1} ;
uint8_t errSeVL[3]={0,1,0} ;
uint8_t errTeVH[3]={1,0,0} ;
uint8_t errIoVH[3]={1,0,1} ;
uint8_t errIoVL[3]={1,1,0} ;

uint8_t errReOp[4]={0,0,0,0} ;
uint8_t errVeOp[4]={0,0,0,1} ;
uint8_t errReVeOp[4]={0,0,1,0} ;

uint8_t errFlOp[4]={0,0,1,1} ;
uint8_t errO4Op[4]={0,1,0,0} ;
uint8_t errFlO4Op[4]={0,1,0,1} ;

uint8_t errReSh[4]={0,1,1,0} ;
uint8_t errVeSh[4]={0,1,1,1} ;
uint8_t errFlSh[4]={1,0,0,0} ;

uint16_t errCnt = 0 ;

uint8_t errIndex = 0 ;

void Fun_Led(uint8_t * x , uint8_t length){
	if(errIndex < length){
		if((*(x+errIndex)) == 0){
			errCnt ++ ;
			if(errCnt >= 0 && errCnt <= 20){
				LED_R_ON ;
			}
			if(errCnt > 20 && errCnt <= 40){
				LED_R_OFF ;
			}
			if(errCnt > 40){
				errCnt = 0 ;
				errIndex ++ ;
			}
		}

		if((*(x+errIndex)) == 1){
			errCnt ++ ;
			if(errCnt < 40){
				LED_R_ON ;
			}
			if(errCnt >= 40 && errCnt < 60){
				LED_R_OFF ;
			}
			if(errCnt == 60){
				errCnt = 0 ;
				errIndex ++ ;
			}
		}
	}

	if(errIndex == length){
		errCnt ++ ;
		if(errCnt == 50)
		{
			errCnt = 0 ;
			errIndex = 0 ;

			errCount ++ ;
//			if(errCount == 19){
//				errCount = 1 ;
//			}
		}
	}
}

void Fun_ErrLed(){
	switch(errCount){
		case 1 :
			if(Hw[0].flag)//电源电压过高
				Fun_Led(errInVH , 3);
			else
				errCount ++ ;
			break ;
		case 2 :
			if(Hw[2].flag && Hw[18].flag == false)//电源电压过低
				Fun_Led(errInVL , 3);
			else
				errCount ++ ;
			break ;
		case 3 :
			if(Hw[2].flag && Hw[18].flag)//电源电压过低
				Fun_Led(errIn14VL , 3);
			else
				errCount ++ ;
			break ;
		case 4 :
			/* 当检测到气压传感器输出电压过高或过低 对应标志位置1 主程序中检测到标志位置1
			 * 执行恢复供气2s后 LED循环闪烁故障
			 */
			if(Hw[4].flag)//气压传感器输出电压过高
				Fun_Led(errSeVH , 3);
			else
				errCount ++ ;
			break ;
		case 5 :
			/* 当检测到气压传感器输出电压过高或过低 对应标志位置1 主程序中检测到标志位置1
			 * 执行恢复供气2s后 LED循环闪烁故障
			 */
			if(Hw[6].flag)//气压传感器输出电压过低
				Fun_Led(errSeVL , 3);
			else
				errCount ++ ;
			break ;
		case 6 :
			if(Hw[12].flag)//控制器温度过高
				Fun_Led(errTeVH , 3);
			else
				errCount ++ ;
			break ;
		case 7 :
			if(Hw[16].flag)//输出信号电压过高
				Fun_Led(errIoVH , 3);
			else
				errCount ++ ;
			break ;
		case 8 :
			if(Hw[14].flag)//输出信号电压过低
				Fun_Led(errIoVL , 3);
			else
				errCount ++ ;
			break ;
		case 9 :
			if(errorCon && RecoOpen){//输出信号电压过低
				Fun_Led(errReOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 10 :
			if(errorCon && VentOpen){//输出信号电压过低
				Fun_Led(errVeOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 11 :
			if(errorCon && st1_2Open){//输出信号电压过低
				Fun_Led(errReVeOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 12 :
			if(BackOpen){//输出信号电压过低
				Fun_Led(errFlOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 13 :
			if(errorCon && Out4Open){//输出信号电压过低
				Fun_Led(errO4Op , 4);
			}
			else
				errCount ++ ;
			break ;
		case 14 :
			if(errorCon && st3_4Open){//输出信号电压过低
				Fun_Led(errFlO4Op , 4);
			}
			else
				errCount ++ ;
			break ;
		case 15 :
			if(reShFlag){//输出信号电压过低
				Fun_Led(errReSh , 4);
			}
			else
				errCount ++ ;
			break ;
		case 16 :
			if(veShFlag){//输出信号电压过低
				Fun_Led(errVeSh , 4);
			}
			else
				errCount ++ ;
			break ;
		case 17 :
			if(flShFlag){//输出信号电压过低
				Fun_Led(errFlSh , 4);
			}
			else
				errCount ++ ;
			break ;
		case 18 :
			errCnt = 0 ;
			errIndex = 0 ;
			errCount = 1 ;
			break ;
	}
}
