/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


#include "derivative.h" /* include peripheral declarations SSKEAZ128M4 */
#include "systeminit.h"
#include "Sht30.h"
#include "Sht30_api.h"
#include "can.h"

#define LED_G   PTB5
#define PTB5      13

#define LED_G_TOG     GPIOA_PTOR |= 1 << LED_G

I2C_Type* pI2C1 = (I2C_Type*)I2C1;
uint8_t test_data = 0 ;

int main(void)
{
//	I2C_ConfigType  I2C_Config = {{0}};
	Clk_Internal() ;
//	CAN_Init(MSCAN_BUS_CLK,MSCAN_SJW,MSCAN_BRP,MSCAN_TSEG1,MSCAN_TSEG2);
//	Clk_External() ;

	//    /* Initialize I2C module with poll mode */
	//  	I2C_Config.u16Slt = 0;
	//  	I2C_Config.u16F = 0x1E; 				/* Baud rate at 100 kbit/sec, MULT = 4 , ICR=60*/
	//    I2C_Config.sSetting.bIntEn = 0;
	//    I2C_Config.sSetting.bI2CEn = 1;
	//    I2C_Config.sSetting.bMSTEn = 1;
	//
	//    I2C_Init(pI2C1,&I2C_Config );

	SIM->SCGC |= SIM_SCGC_I2C1_MASK;
	I2C1->C1 |= 1<<7;//使能IIC。
	//I2C1->C1 |= 1<<6;//使能IIC。
	I2C1->F = 0x1E;//设置IIC的波特率为100Khz
//	I2C1->SMB |= I2C_SMB_FACK_MASK ;

	GPIOA_PDDR |= 1 << LED_G ;//LED_G设为输出

	delay_s1(100) ;

//	SHT3x_ReadSerialNumber() ;

	for(;;)
	{
		SHT3X_GetTempAndHumiPolling() ;
//		flexcan_Trigger() ;
		LED_G_TOG ;
		delay_s(1) ;
	}

//	Sht30_InitPeriod() ;
//
//	for(;;)
//	{
//		Sht30_GetData() ;
//	}

	for(;;)
	{
		Sht30_GetTempAndHumi() ;
		delay_s(1) ;
		LED_G_TOG ;
	}

	for(;;){
		SHT30_WorkProcess() ;
	}
	return 0;
}
