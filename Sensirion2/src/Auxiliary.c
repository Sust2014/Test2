/*
 * Auxiliary.c
 *
 *  Created on: 2020年1月3日
 *      Author: Cenx
 */

#include "Auxiliary.h"

/*判断干燥器卸压阀是否密封正常*/
void PressureReliefValve(){
	if(InTheRecover){
		Unload.Ulsec ++ ;
		if(Unload.Ulsec == 60000){
			Unload.Ulsec = 0 ;
			Unload.Ulmin ++ ;
		}
		if(Unload.Ulmin == 60){
			Unload.Ulmin = 0 ;
			Unload.Ulhour ++ ;
		}
	}

	if(InTheRecover == 0){
		Unload.Ulsec = 0 ;
		Unload.Ulmin = 0 ;
		Unload.Ulhour = 0 ;
	}

	if(Unload.Ulmin == 50){
		Unload.Ulmin = 0 ;
		UnloadValve = true ;
	}
}

void AuxiliaryLed(){
	if(UnloadValve){
		UnloadValve = false ;
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(200);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		LED_R_ON ;delay_s1(500);LED_R_OFF ;delay_s1(200);
		delay_s1(500);
	}
}

uint8_t dutyCount = 0 ;
uint8_t x = 0 ;

void Auxiliary_Output(){
	if(x == 1){
		dutyCount ++ ;
		if(dutyCount == 10){
			dutyCount = 0 ;
			x = 0 ;
		}
	}
}






