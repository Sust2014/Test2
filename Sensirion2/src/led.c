/*
 * led.c
 *
 *  Created on: 2020��1��8��
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
			if(Hw[0].flag)//��Դ��ѹ����
				Fun_Led(errInVH , 3);
			else
				errCount ++ ;
			break ;
		case 2 :
			if(Hw[2].flag && Hw[18].flag == false)//��Դ��ѹ����
				Fun_Led(errInVL , 3);
			else
				errCount ++ ;
			break ;
		case 3 :
			if(Hw[2].flag && Hw[18].flag)//��Դ��ѹ����
				Fun_Led(errIn14VL , 3);
			else
				errCount ++ ;
			break ;
		case 4 :
			/* ����⵽��ѹ�����������ѹ���߻���� ��Ӧ��־λ��1 �������м�⵽��־λ��1
			 * ִ�лָ�����2s�� LEDѭ����˸����
			 */
			if(Hw[4].flag)//��ѹ�����������ѹ����
				Fun_Led(errSeVH , 3);
			else
				errCount ++ ;
			break ;
		case 5 :
			/* ����⵽��ѹ�����������ѹ���߻���� ��Ӧ��־λ��1 �������м�⵽��־λ��1
			 * ִ�лָ�����2s�� LEDѭ����˸����
			 */
			if(Hw[6].flag)//��ѹ�����������ѹ����
				Fun_Led(errSeVL , 3);
			else
				errCount ++ ;
			break ;
		case 6 :
			if(Hw[12].flag)//�������¶ȹ���
				Fun_Led(errTeVH , 3);
			else
				errCount ++ ;
			break ;
		case 7 :
			if(Hw[16].flag)//����źŵ�ѹ����
				Fun_Led(errIoVH , 3);
			else
				errCount ++ ;
			break ;
		case 8 :
			if(Hw[14].flag)//����źŵ�ѹ����
				Fun_Led(errIoVL , 3);
			else
				errCount ++ ;
			break ;
		case 9 :
			if(errorCon && RecoOpen){//����źŵ�ѹ����
				Fun_Led(errReOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 10 :
			if(errorCon && VentOpen){//����źŵ�ѹ����
				Fun_Led(errVeOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 11 :
			if(errorCon && st1_2Open){//����źŵ�ѹ����
				Fun_Led(errReVeOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 12 :
			if(BackOpen){//����źŵ�ѹ����
				Fun_Led(errFlOp , 4);
			}
			else
				errCount ++ ;
			break ;
		case 13 :
			if(errorCon && Out4Open){//����źŵ�ѹ����
				Fun_Led(errO4Op , 4);
			}
			else
				errCount ++ ;
			break ;
		case 14 :
			if(errorCon && st3_4Open){//����źŵ�ѹ����
				Fun_Led(errFlO4Op , 4);
			}
			else
				errCount ++ ;
			break ;
		case 15 :
			if(reShFlag){//����źŵ�ѹ����
				Fun_Led(errReSh , 4);
			}
			else
				errCount ++ ;
			break ;
		case 16 :
			if(veShFlag){//����źŵ�ѹ����
				Fun_Led(errVeSh , 4);
			}
			else
				errCount ++ ;
			break ;
		case 17 :
			if(flShFlag){//����źŵ�ѹ����
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
