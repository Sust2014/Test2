/*
 * can.c
 *
 *  Created on: 2015年10月8日
 *      Author: Chen_Sky
 */

#include "can.h"
#include "string.h"
#include "stdlib.h"
#include "utility_function.h"
#include "fault_led.h"
#include "check.h"


/******************************NTC100K***********************************/
float NTC100K[][2]={
		{0,3989},{1,3981},{2,3973},{3,3965},{4,3956},{5,3946},{6,3936},{7,3926},{8,3915},{9,3903},{10,3891},
		{11,3878},{12,3864},{13,3850},{14,3835},{15,3819},{16,3803},{17,3786},{18,3768},{19,3749},{20,3730},
		{21,3709},{22,3688},{23,3666},{24,3643},{25,3619},{26,3594},{27,3568},{28,3541},{29,3514},{30,3485},
		{31,3456},{32,3425},{33,3394},{34,3362},{35,3329},{36,3295},{37,3260},{38,3224},{39,3187},{40,3150},
		{41,3111},{42,3072},{43,3032},{44,2992},{45,2950},{46,2909},{47,2866},{48,2823},{49,2779},{50,2735},
		{51,2691},{52,2646},{53,2600},{54,2555},{55,2509},{56,2463},{57,2417},{58,2371},{59,2324},{60,2278},
		{61,2232},{62,2185},{63,2139},{64,2094},{65,2048},{66,2003},{67,1958},{68,1913},{69,1869},{70,1825},
		{71,1782},{72,1739},{73,1696},{74,1655},{75,1613},{76,1573},{77,1533},{78,1494},{79,1455},{80,1417},
		{81,1380},{82,1343},{83,1307},{84,1272},{85,1238},{86,1204},{87,1171},{88,1139},{89,1107},{90,1076},
		{91,1046},{92,1017},{93,988},{94,960},{95,933},{96,906},{97,881},{98,855},{99,831},{100,807},
		{101,784},{102,761},{103,739},{104,718},{105,697},{106,677},{107,657},{108,638},{109,620},{110,602},
		{111,585},{112,568},{113,551},{114,535},{115,520},{116,505},{117,490},{118,476},{119,462},{120,449},
		{121,436},{122,424},{123,412},{124,400},{125,389},{126,377},{127,367},{128,356},{129,346},{130,337},
		{131,327},{132,318},{133,309},{134,300},{135,292},{136,284},{137,276},{138,268},{139,261},{140,254},
		{141,247},{142,240},{143,234},{144,227},{145,221},{146,215},{147,209},{148,204},{149,198},{150,193},
		{151,188},{152,183},{153,178},{154,173},{155,169},{156,164},{157,160},{158,156},{159,152},{160,148},
		{161,144},{162,141},{163,137},{164,134},{165,130}
};

uint8_t MSCAN_Send_Data[] = {0,0,0,0,0,0,0,0};
uint8_t MSCAN_Send_Data1[] = {0,0,0,0,0,0,0,0};

uint8_t SYS_DATE[] = {0,0,0,0,0,0,0,0};

uint8_t buffer100k[4];
uint8_t buffer10k[4];
uint8_t bufferpt1000[4];

//void test()
//{
//	uint16_t a = 929 ;uint16_t b = 0 ;
//	uint8_t data1[4] = {0,0,0,0};
//
//	data1[0] = data & 0x0f ;
//	data1[1] = data & 0xf0 ;
//	b = data & 0xff00 ;
//	data1[2] = (data >> 8) & 0x0f ;
//	data1[2] = (data & 0xff00)>>8 ;
//}

float Temperature_conversion(float t)
{
	uint8_t t_value=0;
	uint16_t t_value_l=0;
	uint32_t i = 0 ;
	t_value_l=abs(t-NTC100K[0][1]);
	t_value=NTC100K[0][0];
	for(i = 1 ; i < 126 ; i++)
	{
		if(abs(t-NTC100K[i][1]) < t_value_l)
		{
			t_value_l=abs(t-NTC100K[i][1]);
			t_value=NTC100K[i][0];
		}
		else
			t_value+=0;
	}
	return t_value;
}

/***********************************************************************************************************/
//mscan_clk	 :MSCAN clock source
//mscan_sjw	 :SJW value
//mscan_brp	 :BRP value
//mscan_tseg1:TSEG1 value
//mscan_tseg2:TSEG2 value
/***********************************************************************************************************/
void CAN_Init(uint8_t mscan_clk,uint8_t mscan_sjw,uint8_t mscan_brp,uint8_t mscan_tseg1,uint8_t mscan_tseg2)
{
	//select CAN pin,CAN_TX on PTC7, CAN_RX on PTC6.
	SIM_PINSEL1 &= ~SIM_PINSEL1_MSCANPS_MASK;
	//Bus clock to the MSCAN module is enabled.
	SIM_SCGC |= SIM_SCGC_MSCAN_MASK;

	//CAN enable,MSCAN clock source is the oscillator clock,listen only mode no operation.
	MSCAN_CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;
	if(1 == mscan_clk)
		MSCAN_CANCTL1 |= MSCAN_CANCTL1_CLKSRC_MASK;
	else
		MSCAN_CANCTL1 &= ~MSCAN_CANCTL1_CLKSRC_MASK;
	MSCAN_CANCTL1 &= ~MSCAN_CANCTL1_LISTEN_MASK;

	//MSCAN in initialization mode.
	MSCAN_CANCTL0 |= MSCAN_CANCTL0_INITRQ_MASK;
	//Wait MSCAN run in initialization mode
	while(!MSCAN_CANCTL1 & MSCAN_CANCTL1_INITAK_MASK);

	//Filter closed
	MSCAN_CANIDAC |=  MSCAN_CANIDAC_IDAM_MASK;

	//Initialize baud rate : 100KHz
	MSCAN_CANBTR0 = MSCAN_CANBTR0_SJW(mscan_sjw) | MSCAN_CANBTR0_BRP(mscan_brp);
	MSCAN_CANBTR1 = MSCAN_CANBTR1_TSEG1(mscan_tseg1)|MSCAN_CANBTR1_TSEG2(mscan_tseg2);
	MSCAN_CANBTR1 &= ~MSCAN_CANBTR1_SAMP_MASK;

	//exit initialization mode
	MSCAN_CANCTL0 &= ~MSCAN_CANCTL0_INITRQ_MASK;
	//wait exit
	while(MSCAN_CANCTL1 & MSCAN_CANCTL1_INITAK_MASK);

	//CAN enable
	MSCAN_CANCTL1 |= MSCAN_CANCTL1_CANE_MASK;
}

/***********************************************************************************************************/
//mscan_addr : MSCAN address,28 bit
//mscan_data : data pointer,1 byte ~ 8 byte
//mscan_dlr  : data length
/***********************************************************************************************************/
//void CAN_Send(uint32_t mscan_addr,const uint8_t * mscan_data,uint8_t mscan_dlr)
//{
//	uint8_t u8TransmitterEmptyFlag;
//	//read MSCAN_CANTFLG_TXE register ==> able use cache
//	u8TransmitterEmptyFlag = MSCAN_CANTFLG & MSCAN_CANTFLG_TXE_MASK;
//	//select transmitter channel
//	MSCAN_CANTBSEL = MSCAN_CANTBSEL_TX(u8TransmitterEmptyFlag);
//	//allocation address
//	MSCAN_TEIDR0 = ((mscan_addr>>21)&0xff);
//	MSCAN_TEIDR1 = ((mscan_addr>>13)&0xe0)|0x18|((mscan_addr>>15)&0x07);
//	MSCAN_TEIDR2 = ((mscan_addr>>7)&0xff);
//	MSCAN_TEIDR3 = ((mscan_addr<<1)&0xfe);
//	//allocation data
//	memcpy((void *)&MSCAN_TEDSR0,(void *)mscan_data,mscan_dlr);
//	MSCAN_TDLR	 = mscan_dlr;
//	MSCAN_TBPR   = 0;
//	MSCAN_CANTFLG = (MSCAN_CANTBSEL&MSCAN_CANTBSEL_TX_MASK)&0x07;
//}

void CAN_Send(uint32_t mscan_addr,const uint8_t * mscan_data,uint8_t mscan_dlr,uint8_t mscan_ide)
{
	uint8_t u8TransmitterEmptyFlag;
	//read MSCAN_CANTFLG_TXE register ==> able use cache
	u8TransmitterEmptyFlag = MSCAN_CANTFLG & MSCAN_CANTFLG_TXE_MASK;
	//select transmitter channel
	MSCAN_CANTBSEL = MSCAN_CANTBSEL_TX(u8TransmitterEmptyFlag);
	if(mscan_ide == 1){
	//allocation address
	MSCAN_TEIDR0 = ((mscan_addr>>21)&0xff);
	MSCAN_TEIDR1 = ((mscan_addr>>13)&0xe0)|0x18|((mscan_addr>>15)&0x07);
	MSCAN_TEIDR2 = ((mscan_addr>>7)&0xff);
	MSCAN_TEIDR3 = ((mscan_addr<<1)&0xfe);
	}

	if(mscan_ide == 0){
		MSCAN_TSIDR0 = (mscan_addr >> 3) & 0xff ;//前3位数据不要 TSIDR1：10-3 数据>>3 & 0xff（&为了准确赋值寄存器）
		MSCAN_TSIDR1 = (mscan_addr << 5) & 0xe0 ;//前3位数据放到TSIDR0的后3位 TSIDR0：2-0  数据<<5 & 0xe0（&为了准确赋值寄存器）
	}
	//allocation data
	memcpy((void *)&MSCAN_TEDSR0,(void *)mscan_data,mscan_dlr);
	MSCAN_TDLR	 = mscan_dlr;
	MSCAN_TBPR   = 0;
	MSCAN_CANTFLG = (MSCAN_CANTBSEL&MSCAN_CANTBSEL_TX_MASK)&0x07;
}

void flexcan_Trigger()
{
	/***********adc_value_Air_pressure1***********/
//	MSCAN_Send_Data[0] = adc_value_Air_pressure1 % 16 ;//最低位
//	MSCAN_Send_Data[1] = (adc_value_Air_pressure1 >> 4) % 16 ;
//	MSCAN_Send_Data[2] = (adc_value_Air_pressure1 >> 8) % 16 ;
//	MSCAN_Send_Data[3] = (adc_value_Air_pressure1 >> 12) % 16 ;
//
//	/***********controlTemp***********/
//	MSCAN_Send_Data[4] = controlTemp%16 ;//最低位
//	MSCAN_Send_Data[5] = (controlTemp >> 4) % 16 ;
//	MSCAN_Send_Data[6] = (controlTemp >> 8) % 16 ;
//	MSCAN_Send_Data[7] = (controlTemp >> 12) % 16 ;
//
//	/***********current***********/
//	MSCAN_Send_Data1[0] = current%16 ;//最低位
//	MSCAN_Send_Data1[1] = (current >> 4) % 16 ;
//	MSCAN_Send_Data1[2] = (current >> 8) % 16 ;
//	MSCAN_Send_Data1[3] = (current >> 12) % 16 ;
//
//	/***********adc_value_Input_24V***********/
//	MSCAN_Send_Data1[4] = adc_value_Input_24V%16 ;//最低位
//	MSCAN_Send_Data1[5] = (adc_value_Input_24V >> 4) % 16 ;
//	MSCAN_Send_Data1[6] = (adc_value_Input_24V >> 8) % 16 ;
//	MSCAN_Send_Data1[7] = (adc_value_Input_24V >> 12) % 16 ;

	/***********adc_value_Air_pressure1***********/
//	MSCAN_Send_Data[0] = adc_value_Air_pressure1 &0x0f ;//最低位
//	MSCAN_Send_Data[1] = adc_value_Air_pressure1 &0xf0 ;
//	MSCAN_Send_Data[2] = (adc_value_Air_pressure1 >> 8) & 0x0f ;
//	MSCAN_Send_Data[3] = 0 ;
//
//	/***********controlTemp***********/
//	MSCAN_Send_Data[4] = controlTemp & 0x0f ;//最低位
//	MSCAN_Send_Data[5] = (controlTemp>>4) & 0x0f ;
//	MSCAN_Send_Data[6] = (controlTemp >> 8) & 0x0f ;
//	MSCAN_Send_Data[7] = 0 ;

	/***********current***********/
//	MSCAN_Send_Data1[0] = current & 0x0f ;//最低位
//	MSCAN_Send_Data1[1] = (current >> 4) & 0x0f ;
//	MSCAN_Send_Data1[2] = (current >> 8) & 0x0f ;
//	MSCAN_Send_Data1[3] = 0 ;

	/***********adc_value_Input_24V***********/
//	MSCAN_Send_Data1[4] = adc_value_Input_24V & 0x0f ;//最低位
//	MSCAN_Send_Data1[5] = (adc_value_Input_24V >> 4 ) & 0x0f ;
//	MSCAN_Send_Data1[6] = (adc_value_Input_24V >> 8) & 0x0f ;
//	MSCAN_Send_Data1[7] = 0 ;

//	test_data100 = (float)controlTemp / 4095.0 ;
//	test_data100k = (float)(test_data100 * 5.0 * 100) / (5.0 - test_data100 * 5.0) ;
//	MSCAN_Send_Data[1] = Temperature_conversion(test_data100k);

//	MSCAN_Send_Data[1] = controlTemp ;
//
//	test_data10 = (float)current / 4095.0 ;
//	test_data10k = (float)(test_data10 * 5.0 * 10) / (5.0 - test_data10 * 5.0) ;
//	MSCAN_Send_Data[0] = Temperature_conversion(current);
//
//	MSCAN_Send_Data[2] = current ;
//
//	MSCAN_Send_Data[3] = adc_value_Input_24V ;
//	buffer[0] = adc_value_Input_24V & (0x000f) ;
//	buffer[1] = adc_value_Input_24V & (0x00f0) ;
//	buffer[2] = adc_value_Input_24V & (0x0f00) ;
//	buffer[3] = adc_value_Input_24V & (0xf000) ;
////	To_Hex(adc_value_Input_24V,buffer,3);
//	convertDecimalToBinary(adc_value_Input_24V);
	MSCAN_Send_Data[0] = Temperature_conversion(adc_value_Input_24V);

//	CAN_Send(0X01,MSCAN_Send_Data,8);
	CAN_Send(0x02,MSCAN_Send_Data,8,STANDARD);
}
