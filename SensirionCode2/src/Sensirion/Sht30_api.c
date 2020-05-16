/*
 * Sht30_api.c
 *
 *  Created on: 2020年5月15日
 *      Author: Cenx
 */
#include "Sht30_api.h"
#include "systeminit.h"

#define SHT30ADDR   0x44
#define CMD_MEAS_CLOCKSTR_H   0x2C06
#define CMD_MEAS_CLOCKSTR_M   0x2C0D
#define CMD_MEAS_PERI_1_H     0x2130
#define CMD_FETCH_DATA        0xE000

#define CMD_MEAS_POLLING_H    0x2400

// Generator polynomial for CRC
#define POLYNOMIAL	0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

static uint8_t SenData[6] = {0,0,0,0,0,0} ;
static uint8_t SenDataPeriod[6] = {0,0,0,0,0,0} ;

static float test_data1 = 0.0 ;
static float test_data2 = 0.0 ;

float	temperature;        // temperature [°C]
float	humidity;	        // relative humidity [%RH]

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
    	return 0 ;

    return crc;
}

static float SHT3X_CalcTemperature(uint16_t rawValue)
{
    // calculate temperature [°C]
    // T = -45 + 175 * rawValue / (2^16-1)
    return 175.0f * (uint16_t)rawValue / 65535.0f - 45.0f;
}

static float SHT3X_CalcHumidity(uint16_t rawValue)
{
    // calculate relative humidity [%RH]
    // RH = rawValue / (2^16-1) * 100
    return 100.0f * (uint16_t)rawValue / 65535.0f;
}

void Sht30_Start(void)
{
	I2C_Start(I2C1) ;//I2C开始通信需要的条件
}

void Sht30_Stop(void)
{
	I2C_Stop(I2C1) ;
}

void Sht30_SendByte(uint8_t Byte)
{
//	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C_TxEnable(I2C1);//选择发送模式
	I2C_WriteDataReg(I2C1 , Byte) ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	while((I2C1->S1 & 0x01)) ;//等待应答信号
}

void Sht30_ReadByte(uint8_t *data , uint8_t ack)
{
	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	*data = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	if(ack)
		I2C_SendAck(I2C1) ;
	else
		I2C_SendNack(I2C1) ;
}

void Sht30_ReadByte1(uint8_t *data , uint8_t ack)
{
//	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	if(ack)
		I2C_SendAck(I2C1) ;
	else
		I2C_SendNack(I2C1) ;
	*data = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志
	*data = I2C1->D ;
//	if(ack)
//		I2C_SendAck(I2C1) ;
//	else
//		I2C_SendNack(I2C1) ;
}

void Sht30_Init(void)
{
	Sht30_Start() ;
	Sht30_SendByte(SHT30ADDR << 1) ;
	Sht30_SendByte(CMD_MEAS_CLOCKSTR_H >> 8) ;
	Sht30_SendByte(CMD_MEAS_CLOCKSTR_H & 0x00FF) ;
	Sht30_Stop() ;
	delay_s1(50) ;
}

void Sht30_InitPeriod(void)
{
	Sht30_Start() ;
	Sht30_SendByte(0x88) ;
	Sht30_SendByte(0x21) ;
	Sht30_SendByte(0x30) ;
	Sht30_Stop() ;
	delay_s1(150) ;
}

void Sht30_GetData(void)
{
	uint8_t check_data = 0 ;
    uint16_t	rawValueTemp; // temperature raw value from sensor
    uint16_t	rawValueHumi; // humidity raw value from sensor

	Sht30_Start() ;
	Sht30_SendByte(0x88) ;
	Sht30_SendByte(0xe0) ;
	Sht30_SendByte(0x00) ;
//	Sht30_Stop() ;
//	delay_s1(1) ;

	Sht30_Start() ;
	Sht30_SendByte(0x89) ;

	Sht30_ReadByte1(&SenDataPeriod[0] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[1] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[2] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[3] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[4] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[5] , 0) ;
	Sht30_Stop() ;

	check_data = SenDataPeriod[2] ;

//	if(SHT3X_CalcCrc(SenDataPeriod , 2 , check_data))
//	{
		rawValueTemp = ((SenDataPeriod[0] << 8) | SenDataPeriod[1]) ;
//	}

	check_data = SenDataPeriod[5] ;

//	if(SHT3X_CalcCrc(&SenDataPeriod[3] , 2 , check_data))
//	{
		rawValueHumi = ((SenDataPeriod[3] << 8) | SenDataPeriod[4]) ;
//	}

//	test_data1 = 175.0 * ((SenData[0] << 8) | SenData[1]) / 65535.0 - 45.0 ;
//	test_data2 = 100.0 * ((SenData[3] << 8) | SenData[4]) / 65535.0 ;

    temperature = SHT3X_CalcTemperature(rawValueTemp);
    humidity = SHT3X_CalcHumidity(rawValueHumi);

	uint8_t test_data = 0 ;
}

uint8_t test_count = 0 ;

void SHT3x_ReadSerialNumber(void)
{
	uint16_t serialNumWords[6] ;
	Sht30_Start() ;
	Sht30_SendByte(SHT30ADDR << 1) ;
	Sht30_SendByte(0x37) ;
	Sht30_SendByte(0x80) ;
	Sht30_Start() ;
	Sht30_SendByte((SHT30ADDR << 1) | 0x01) ;
	Sht30_ReadByte1(&serialNumWords[0] , 1) ;
	Sht30_ReadByte1(&serialNumWords[1] , 1) ;
	Sht30_ReadByte1(&serialNumWords[2] , 1) ;
	Sht30_ReadByte1(&serialNumWords[3] , 1) ;
	Sht30_ReadByte1(&serialNumWords[4] , 1) ;
	Sht30_ReadByte1(&serialNumWords[5] , 0) ;
	Sht30_Stop() ;
}

void Sht30_GetTempAndHumi(void)
{
	uint8_t check_data = 0 ;
    uint16_t	rawValueTemp; // temperature raw value from sensor
    uint16_t	rawValueHumi; // humidity raw value from sensor

	Sht30_Init() ;
	Sht30_Start() ;
	Sht30_SendByte((SHT30ADDR << 1) | 0x01) ;
//	delay_s1(50) ;

//	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
//	I2C_TxEnable(I2C1);//选择发送模式
//	I2C_WriteDataReg(I2C1 , (((SHT30ADDR << 1) | 0x01))) ;
//	while(!(I2C1->S1 & 0x02)) ;//中断挂起
//	I2C1->S1 |= 0x02 ;//清除中断标志
//	while(!(I2C1->S1 & 0x01)) ;//等待N应答信号
//
//	Sht30_Stop() ;
//	delay_s1(1) ;
//
//	Sht30_Start() ;
//	Sht30_SendByte((SHT30ADDR << 1) | 0x01) ;

	Sht30_ReadByte1(&SenData[0] , 1) ;
	Sht30_ReadByte1(&SenData[1] , 1) ;
	Sht30_ReadByte1(&SenData[2] , 1) ;
	Sht30_ReadByte1(&SenData[3] , 1) ;
	Sht30_ReadByte1(&SenData[4] , 1) ;
	Sht30_ReadByte1(&SenData[5] , 0) ;
	Sht30_Stop() ;
	delay_s1(150) ;

//	I2C1->S1 |= I2C_S1_ARBL_MASK ;

	check_data = SenData[2] ;

//	if(SHT3X_CalcCrc(SenData , 2 , check_data))
//	{
		rawValueTemp = ((SenData[0] << 8) | SenData[1]) ;
//	}

	check_data = SenData[5] ;

//	if(SHT3X_CalcCrc(&SenData[3] , 2 , check_data))
//	{
		rawValueHumi = ((SenData[3] << 8) | SenData[4]) ;
//	}

//	test_data1 = 175.0 * ((SenData[0] << 8) | SenData[1]) / 65535.0 - 45.0 ;
//	test_data2 = 100.0 * ((SenData[3] << 8) | SenData[4]) / 65535.0 ;

    temperature = SHT3X_CalcTemperature(rawValueTemp);
    humidity = SHT3X_CalcHumidity(rawValueHumi);

	uint8_t test_data = 0 ;
}

void SHT3X_GetTempAndHumiPolling(void)
{
	uint8_t check_data = 0 ;
    uint16_t	rawValueTemp; // temperature raw value from sensor
    uint16_t	rawValueHumi; // humidity raw value from sensor
    uint8_t test = 0 ;

	Sht30_Start() ;
	Sht30_SendByte(SHT30ADDR << 1) ;

	Sht30_SendByte(CMD_MEAS_POLLING_H >> 8) ;
	Sht30_SendByte(CMD_MEAS_POLLING_H & 0x00FF) ;

	Sht30_Stop() ;
	delay_s1(50) ;
	test = 50 ;

	uint8_t test1 = 0 ;

//	while(test--)
//	{
//		Sht30_Start() ;
//		test1 = I2C_WriteOneByte(I2C1,((SHT30ADDR << 1) | 0x01)) ;
////		Sht30_SendByte((SHT30ADDR << 1) | 0x01) ;
//		if(test1 == 0)
//			break ;
//		delay_s1(1) ;
//	}

	Sht30_Start() ;
	Sht30_SendByte((SHT30ADDR << 1) | 0x01) ;

	Sht30_ReadByte1(&SenDataPeriod[0] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[1] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[2] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[3] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[4] , 1) ;
	Sht30_ReadByte1(&SenDataPeriod[5] , 0) ;
	delay_s1(100) ;
	Sht30_Stop() ;

	check_data = SenDataPeriod[2] ;
	rawValueTemp = ((SenDataPeriod[0] << 8) | SenDataPeriod[1]) ;
	check_data = SenDataPeriod[5] ;
	rawValueHumi = ((SenDataPeriod[3] << 8) | SenDataPeriod[4]) ;

    temperature = SHT3X_CalcTemperature(rawValueTemp);
    humidity = SHT3X_CalcHumidity(rawValueHumi);

	uint8_t test_data = 0 ;
}

void WirteByte_I2C(unsigned char addr, unsigned char data)
{
	I2C_Start(I2C1);

	Sht30_SendByte(0xA0) ;
	Sht30_SendByte(addr) ;
	Sht30_SendByte(data) ;
	Sht30_Stop() ;
}

unsigned char ReadByte_I2C(unsigned char addr)
{
	unsigned char temp;

	I2C_Start(I2C1);

	Sht30_SendByte(0xA0) ;
	Sht30_SendByte(addr) ;

	I2C1->C1 |= 1<<2;//重启IIC

	Sht30_SendByte(0xA1) ;

//	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
//	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
////	I2C_SendAck(I2C1);
//	temp = I2C1->D ;
//	while(!(I2C1->S1 & 0x02)) ;//中断挂起
//	I2C1->S1 |= 0x02 ;//清除中断标志
//
//	I2C1->C1 &= ~I2C_C1_MST_MASK;
//
//	temp = I2C1->D;

	while(!(I2C1->S1 & 0x80)) ;//传输完成标志
	I2C1->C1 &= ~I2C_C1_TX_MASK;//选择接收模式
//	I2C_SendAck(I2C1);
	temp = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//中断挂起
	I2C1->S1 |= 0x02 ;//清除中断标志

	temp = I2C1->D;

	return(temp);
}

unsigned char ReadByte_I2C1(unsigned char addr)
{
unsigned char temp;

I2C_Start(I2C1);

I2C1->D = 0xA0;//单个24C01做为从设备的地址? ? ? ??

while(!(I2C1->S1&0x02));//判断是否产生中断? ??

I2C1->S1 |= 1<<1;//清除中断标志


while(I2C1->S1&0x01);//判断应答信号??0??收到? ?? ???1 未收到?

I2C1->D = addr;
while(!(I2C1->S1&0x02));//判断是否产生中断??

I2C1->S1 |= 1<<1;//清除中断标志
while(I2C1->S1&0x01);//判断应答信号??0??收到? ?? ???1 未收到??

I2C1->C1 |= 1<<2;//重启IIC

I2C1->D = (0xA1);
while(!(I2C1->S1&0x02));//判断是否产生中断? ???

I2C1->S1 |= 1<<1;//清除中断标志
while(I2C1->S1&0x01);//判断应答信号??0??收到? ?? ???1 未收到??

I2C1->C1 &= ~I2C_C1_TX_MASK;//更改为接收模式? ?

I2C1->C1 |= 1<<3;//发送应答使能

temp = I2C1->D;
while(!(I2C1->S1&0x02));

I2C1->S1 |= 1<<1;//清除中断标志
I2C1->C1 &= ~I2C_C1_MST_MASK;

temp = I2C1->D;

return(temp);
}

