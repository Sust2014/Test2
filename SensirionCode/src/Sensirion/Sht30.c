/*
 * Sht30.c
 *
 *  Created on: 2020年5月12日
 *      Author: ACER
 */
#include "Sht30.h"

#define SHT30ADDR   0x44
#define CMD_MEAS_CLOCKSTR_H   0x2C06
#define CMD_MEAS_PERI_1_H     0x2130
#define CMD_FETCH_DATA        0xE000

// Generator polynomial for CRC
#define POLYNOMIAL	0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

static uint8_t SenData[6] = {0,0,0,0,0,0} ;

static uint8_t SHT3X_CalcCrc(uint8_t data[], uint8_t nbrOfBytes , uint8_t checksum)
{
	uint8_t bit;	// bit mask
	uint8_t crc = 0xFF; // calculated checksum
	uint8_t byteCtr;	// byte counter

    // calculates 8-Bit checksum with given polynomial
    for(byteCtr = 0; byteCtr < nbrOfBytes; byteCtr++)
    {
      crc ^= (data[byteCtr]);
		for(bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80)
				crc = (crc << 1) ^ POLYNOMIAL;
            else
				crc = (crc << 1);
        }
    }

    if(crc != checksum)
    	return 1 ;

    return crc;
}

void SHT3X_StartWriteAccess(void)
{
	I2C_Start(I2C1) ;//I2C开始通信需要的条件

	I2C1->D = (SHT30ADDR << 1);//单个24C01做为从设备的地址
	while(!(I2C1->S1&0x02));//判断是否产生中断
	I2C1->S1 |= 1<<1;//清除中断标志

	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到
}

void SHT3X_StartReadAccess()
{
	I2C_Start(I2C1) ;

	I2C1->D = ((SHT30ADDR << 1) | 0x01);//单个24C01做为从设备的地址
	while(!(I2C1->S1&0x02));//判断是否产生中断
	I2C1->S1 |= 1<<1;//清除中断标志

	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到
}

void SHT3X_WriteCommand(uint16_t command)
{
	// write the upper 8 bits of the command to the sensor
	I2C1->D = (command >> 8);//单个24C01做为从设备的地址
	while(!(I2C1->S1&0x02));//判断是否产生中断
	I2C1->S1 |= 1<<1;//清除中断标志

	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到

	// write the lower 8 bits of the command to the sensor
	I2C1->D = (command & 0x00FF);//单个24C01做为从设备的地址
	while(!(I2C1->S1&0x02));//判断是否产生中断
	I2C1->S1 |= 1<<1;//清除中断标志

	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到
}

void SHT3X_Read2BytesAndCrc()
{
	unsigned char temp_h;
	unsigned char temp_l;

	I2C1->C1 &= ~I2C_C1_TX_MASK;//更改为接收模式

	I2C1->C1 |= 1<<3;//发送应答使能

	temp_h = I2C1->D;
	while(!(I2C1->S1&0x02));

	I2C1->S1 |= 1<<1;//清除中断标志
	I2C1->C1 &= ~I2C_C1_MST_MASK;

	temp_h = I2C1->D;
}

void SHT3X_Work(void)
{
	SHT3X_StartWriteAccess() ;
	SHT3X_WriteCommand(CMD_MEAS_CLOCKSTR_H) ;
	SHT3X_StartReadAccess() ;
	SHT3X_Read2BytesAndCrc() ;
}

void SHT30_WorkProcess(void)
{
	uint8_t check_data = 0 ;

	I2C_Start(I2C1) ;//I2C开始通信需要的条件
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	/******************写数据******************/
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , (CMD_MEAS_CLOCKSTR_H >> 8)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_CLOCKSTR_H) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号
	I2C_Stop(I2C1) ;
	delay_s1(50) ;

	I2C_Start(I2C1) ;//I2C开始通信需要的条件
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x01)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	/******************读数据******************/
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	SenData[0] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	I2C_SendAck(I2C1);

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	SenData[1] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	I2C_SendAck(I2C1);

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	SenData[2] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	I2C_SendAck(I2C1);

	check_data = SenData[2] ;

	SHT3X_CalcCrc(SenData , 2 , check_data) ;

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[3] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[4] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendNack(I2C1);
	SenData[5] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	I2C_Stop(I2C1) ;

	float test_data1 = 0.0 ;
	float test_data2 = 0.0 ;

	test_data1 = 175.0 * ((SenData[0] << 8) | SenData[1]) / 65535.0 - 45.0 ;
	test_data2 = 100.0 * ((SenData[3] << 8) | SenData[4]) / 65535.0 ;

	delay_s1(50) ;
}

void SHT30_WorkProcess1(void)
{
	I2C_Start(I2C1) ;//I2C开始通信需要的条件
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	/******************写数据******************/
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_PERI_1_H >> 8) & 0xFF)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_PERI_1_H) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号
	I2C_Stop(I2C1) ;
	delay_s1(150) ;

	I2C_Start(I2C1) ;//I2C开始通信需要的条件
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((CMD_FETCH_DATA >> 8) & 0xFF)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((CMD_FETCH_DATA) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	I2C_Start(I2C1) ;//I2C开始通信需要的条件
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x01)) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号

	/******************读数据******************/
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[0] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[1] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[2] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[3] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendAck(I2C1);
	SenData[4] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
	I2C_SendNack(I2C1);
	SenData[5] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	I2C_Stop(I2C1) ;
}
