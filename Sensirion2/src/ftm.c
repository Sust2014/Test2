/*
 * FTM.c
 *
 *  Created on: Sep 24, 2013
 *      Author: B46911
 */

#include "ftm.h"
#include "gpio_init.h"
#include "adc_init.h"
#include "fault_led.h"
#include "check.h"
#include "led.h"

extern uint8_t Air_pressure_12;uint16_t Air_pressure_12_count;
extern uint8_t Air_pressure_12_5_count;
extern uint8_t Air_pressure_5;
extern uint8_t Air_pressure_5_2_count;
uint16_t Air_pressure_5_count;
extern uint32_t Air_pressure_value1;
/***********************************************************************************************
 *
 * @brief    FTM_Init - Initialize the FTM counter and enable 1 interrupt (CH0)
 * @param    none
 * @return   none
 *
 ************************************************************************************************/
void FTM2_Init_Output_Cmp() {

	SIM_SCGC |= SIM_SCGC_FTM2_MASK; /* Enable Clock for FTM2 */

	FTM2_SC |= FTM_SC_PS(5); /* Select Preescaler in this case 128. 20 Mhz /128 =156.25 Khz. */
	/* Counter increase by one every 6.4 us */

	/* Enable Channle 0*/
	FTM2_C0SC |= FTM_CnSC_CHIE_MASK; /* Enable channel 0 interrupt */
	FTM2_C0SC |= FTM_CnSC_MSA_MASK; /* Channel as Output compare mode */

	/*Select interrupt frequency*/
	FTM2_C0V = 6250;
	FTM2_SC |= FTM_SC_CLKS(1); /*FTM2 use system clock*/

}

void Enable_Interrupt(uint8_t vector_number)
{

	NVIC_ClearPendingIRQ(vector_number); 		/* Clear pending interrupt register */
	NVIC_EnableIRQ(vector_number); 				/* Enable required Interrupt */

}

/***********************************************************************************************
*
* @brief    FTM2_IRQHandler - Interrupt for FTM channels, Call user function.
* @param    none
* @return   none
*
************************************************************************************************/
void FTM2_IRQHandler()
{
	FTM2_Interrupt();
}
/***********************************************************************************************
*
* @brief    FTM2_Interrupt(void). User Function called by the FTM module on each interrupt
* @param    none
* @return   none
*
************************************************************************************************/
void Fun_sysTime(){

	count++;
	test_count ++ ;
	if(test_count == 100)
	{
		test_count = 0 ;
//		flexcan_Trigger();
	}
	if(count %36 == 0)
	{
		WDOG_Feed();
	}
	if(count == 100)
	{
		second++;
		count=0;
	}
	if(second == 60)
	{
		min++;
		second=0;
	}

	if(min == 15)
	{
		Forcedbackflowflag_15 = true ;
	}
	if(min != 15)
	{
		Forcedbackflowflag_15 = false ;
	}

	if(min == 25)
	{
		Forcedbackflowflag_25 = true ;
	}
	if(min != 25)
	{
		Forcedbackflowflag_25 = false ;
	}

}

void Fun_airStatus(){
	/*恢复供气模式确认*/
	if(RecoverMode == 1){
		if(adc_value_Air_pressure1 <= Air_pressure_up_value1)
			RecoverCount ++ ;
		else
			RecoverCount = 0 ;
	}

	/*排气模式确认*/
	if(VentingMode == 1){
		if(adc_value_Air_pressure1 >= Air_pressure_up_value)
			VentingCount ++ ;
		else
			VentingCount = 0 ;
	}

	/*强制恢复供气模式确认*/
	if(h_RecovMode == 1){
		if(adc_value_Air_pressure1 < Air_pressure_up_value1)
			h_RecovCount ++ ;
		else
			h_RecovCount = 0 ;
	}

	/*强制内卸荷模式确认*/
	if(h_unloadMode == 1){
		if(adc_value_Air_pressure1 >= Force_Venting
				&& adc_value_Air_pressure1 <= Air_pressure_up_value)
			h_unloadCount ++ ;
		else
			h_unloadCount = 0 ;
	}

	/*强制排气模式确认*/
	if(h_VentMode == 1){
		if(adc_value_Air_pressure1 > Air_pressure_up_value)
			h_VentCount ++ ;
		else
			h_VentCount = 0 ;
	}

	if(BackflowMode15 == 1){
		if(adc_value_Air_pressure1 >= Backflow_up_value)
			BackflowCount15 ++ ;
		else
			BackflowCount15 = 0 ;
	}
	if(BackflowMode30 == 1){
		if(adc_value_Air_pressure1 >= Backflow_up_value)
			BackflowCount30 ++ ;
		else
			BackflowCount30 = 0 ;
	}
}

void Fun_interCount(){
	if(reTiming){
		reCount++;
		if(reCount == 1000){
			reTiming = false ;
			reCount = 0 ;
		}
	}
	if(reTiming == false){
		reCount = 0 ;
	}

	if(veTiming){
		veCount++;
		if(veCount == 1000){
			veTiming = false ;
			veCount = 0 ;
		}
	}
	if(veTiming == false){
		veCount = 0 ;
	}
}

void Fun_InflatableCount(){
	if(Forcedloading){
		type1Count ++ ;
		if(type1Count == 2000){
			type1Count = 0 ;
			type1Flag = true ;
		}
	}
	else{
		type1Flag = false ;
		type1Count = 0 ;
	}
}

void Fun_LedG(){
	if(Hw[0].flag == false && Hw[2].flag == false && Hw[18].flag == false){
		if(Hw[4].flag == false && Hw[6].flag == false){
			if(Hw[12].flag == false && Hw[8].flag == false && Hw[10].flag == false){
				if(Hw[16].flag == false && Hw[14].flag == false){
					if(reShFlag == false && veShFlag == false && flShFlag == false
							&& errorCon == false && BackOpen == false){
						LED_G_ON ;
						LED_R_OFF ;
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
	}
	else
		LED_G_OFF ;
}
void FTM2_Interrupt(void)
{
//	(void)FTM2_C0SC;  							/* Read to clear flag */
//	FTM2_C0SC ^= FTM_CnSC_CHF_MASK;  			/* Clear flag */
	FTM2_C0SC &= ~(1 << 7);//Interrupt之后将FTM2_C0SC的CHF设为0
	FTM2_C0V = FTM2_C0V + 6250 ; 				/* Refresh interrupt period */

	sys_count ++ ;
//	if(sys_count == 20)
//	{
//		sys_count = 0 ;
//		flexcan_Trigger();
//	}
//
//	Fun_sysTime();
//	Fun_interCount();
//
//	ADC_Detection();
////	Fun_HwCheck();
//
//	Fun_airStatus();
//	Fun_InflatableCount();

	/*故障输出 中车暂时这一批不需要故障输出*/
	//Fun_ErrorLed();

//	Fun_ErrLed();

//	Fun_LedG();
}
