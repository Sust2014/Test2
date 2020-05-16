/*
 * can.c
 *
 *  Created on: 2015年10月8日
 *      Author: Chen_Sky
 */

#include "can.h"
#include "string.h"
#include "stdlib.h"

uint8_t MSCAN_Send_Data[] = {0,0,0,0,0,0,0,0};

extern float	temperature;        // temperature [°C]
extern float	humidity;	        // relative humidity [%RH]

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
	MSCAN_Send_Data[0] = (uint8_t)temperature ;//最低位
	MSCAN_Send_Data[1] = (uint8_t)humidity ;

	CAN_Send(0X01,MSCAN_Send_Data,8,STANDARD);
}
