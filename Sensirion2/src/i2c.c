/*
 * i2c.c
 *
 *  Created on: 2019年7月22日
 *      Author: byxzwz
 */
#include "derivative.h" /* include peripheral declarations SSKEAZ128M4 */
#include "SKEAZ1284.h"
#include "i2c.h"
#include "ics.h"
#include "fault_led.h"

uint8_t t[256] = {0};
uint8_t i2c_start_data[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;
uint8_t i2c_start_address[17] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} ;

void delay1(volatile int cycles)
{
    /* Delay function - do nothing for a number of cycles */
    while(cycles--);//606666 0.1s 252777 0.1s
}

uint8_t I2C_Start(I2C_Type* pI2Cx)
{
    uint32_t u32Timeout;
    uint8_t u8ErrorStatus;

    u32Timeout = 0;
    u8ErrorStatus = 0x00;

    I2C_TxEnable(pI2Cx);
    pI2Cx->C1 |= I2C_C1_MST_MASK;

    while( (!I2C_IsBusy(pI2Cx)) && ( u32Timeout < I2C_WAIT_STATUS_TIMEOUT))
     {
        u32Timeout ++;
     }

     if( u32Timeout == I2C_WAIT_STATUS_TIMEOUT )
     {
        u8ErrorStatus |= I2C_ERROR_START_NO_BUSY_FLAG;
     }

     return u8ErrorStatus;
}

//******************************************************************************
//******************************************************************************
void Init_I2C(void)
{
//	ICS_ConfigType ICS_set={0};				/* Declaration of ICS_setup structure */
//	ICS_set.u8ClkMode=ICS_CLK_MODE_FEI;
//	ICS_set.bdiv=0;
//	ICS_Init(&ICS_set);             		/*Initialization of Core clock at 48 MHz, Bus Clock at 24 MHz*/

	SIM->SCGC |= SIM_SCGC_I2C1_MASK;
	I2C1->C1 |= 1<<7;//使能IIC。
	//I2C1->C1 |= 1<<6;//使能IIC。
	//I2C baud rate = I2C module clock speed (Hz)/(mul × SCL divider)
	I2C1->F = 0x1e;//设置IIC的波特率为100Khz

}
//******************************************************************************
//******************************************************************************
void WirteByte_I2C(unsigned char addr, unsigned char data)
{
	I2C_Start(I2C1);

	I2C1->D = 0xA0;//单个24C01做为从设备的地址
	while(!(I2C1->S1&0x02));//判断是否产生中断
	I2C1->S1 |= 1<<1;//清除中断标志

	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到

	I2C1->D = addr;//发送地址

	while(!(I2C1->S1&0x02));//判断是否产生中断

	I2C1->S1 |= 1<<1;//清除中断标志
	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到

	I2C1->D = data;//发送数据
	while(!(I2C1->S1&0x02));//判断是否产生中断

	I2C1->S1 |= 1<<1;//清除中断标志
	while(I2C1->S1&0x01);//判断应答信号 0 收到  1 未收到

	I2C1->S1 |= 1<<1;//清除中断标志
	I2C1->C1 &= ~I2C_C1_MST_MASK;//产生一个停止条件

//delay(37916);

}
//******************************************************************************
//******************************************************************************
unsigned char ReadByte_I2C(unsigned char addr)
{
	unsigned char temp;

	I2C_Start(I2C1);

	I2C1->D = 0xA0;//单个24C01做为从设备的地址

	while(!(I2C1->S1&0x02));//判断是否产生中断

	I2C1->S1 |= 1<<1;//清除中断标志


	while(I2C1->S1&0x01);//判断应答信号 0 收到 1 未收到

	I2C1->D = addr;
	while(!(I2C1->S1&0x02));//判断是否产生中断

	I2C1->S1 |= 1<<1;//清除中断标志
	while(I2C1->S1&0x01);//判断应答信号 0 收到 1 未收到

	I2C1->C1 |= 1<<2;//重启IIC

	I2C1->D = (0xA1);
	while(!(I2C1->S1&0x02));//判断是否产生中断

	I2C1->S1 |= 1<<1;//清除中断标志
	while(I2C1->S1&0x01);//判断应答信号 0 收到 1 未收到

	I2C1->C1 &= ~I2C_C1_TX_MASK;//更改为接收模式

	I2C1->C1 |= 1<<3;//发送应答使能

	temp = I2C1->D;
	while(!(I2C1->S1&0x02));

	I2C1->S1 |= 1<<1;//清除中断标志
	I2C1->C1 &= ~I2C_C1_MST_MASK;

	temp = I2C1->D;

	return(temp);
}

void I2C_address_detection()
{
	uint8_t i , j = 0 ;
	for(j = 0 ; j < 17 ; j ++)
	{
		for(i = 0 ; i < 14 ; i ++)
		{
			if(ReadByte_I2C(17*i+j) < 200)
			{
				i2c_start_address[j] = 17 * i + j ;
				delay1(25277);
				break ;
			}
			delay1(25277);
		}
	}
}

void I2C_data_inputvo()
{
	if(Input_voltage_24V_error == 1)
	{
		i2c_start_data[0] = ReadByte_I2C(i2c_start_address[0]) ;
		i2c_start_data[0] += 1 ;
		if(i2c_start_data[0] == 200)
			i2c_start_address[0] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[0] , i2c_start_data[0]) ;
		delay_s1(15);
	}

	if(Input_voltage_24V_error == 2)
	{
		i2c_start_data[1] = ReadByte_I2C(i2c_start_address[1]) ;
		i2c_start_data[1] += 1 ;
		if(i2c_start_data[1] == 200)
			i2c_start_address[1] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[1] , i2c_start_data[1]) ;
		delay_s1(15);
	}
}

void I2C_data_sensorvo()
{
	if(Sensor_voltage_error == 1)
	{
		i2c_start_data[2] = ReadByte_I2C(i2c_start_address[2]) ;
		i2c_start_data[2] += 1 ;
		if(i2c_start_data[2] == 200)
			i2c_start_address[2] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[2] , i2c_start_data[2]) ;
		delay_s1(15);
	}

	if(Sensor_voltage_error == 2)
	{
		i2c_start_data[3] = ReadByte_I2C(i2c_start_address[3]) ;
		i2c_start_data[3] += 1 ;
		if(i2c_start_data[3] == 200)
			i2c_start_address[3] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[3] , i2c_start_data[3]) ;
		delay_s1(15);
	}
}

void I2C_data_tvo()
{
	if(T_voltage_error == 1)
	{
		i2c_start_data[4] = ReadByte_I2C(i2c_start_address[4]) ;
		i2c_start_data[4] += 1 ;
		if(i2c_start_data[4] == 200)
			i2c_start_address[4] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[4] , i2c_start_data[4]) ;
		delay_s1(15);
	}

	if(T_voltage_error == 2)
	{
		i2c_start_data[5] = ReadByte_I2C(i2c_start_address[5]) ;
		i2c_start_data[5] += 1 ;
		if(i2c_start_data[5] == 200)
			i2c_start_address[5] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[5] , i2c_start_data[5]) ;
		delay_s1(15);
	}
}

void I2C_data_12_flag()
{
	if(LED_R_12_flag)
	{
		i2c_start_data[6] = ReadByte_I2C(i2c_start_address[6]) ;
		i2c_start_data[6] += 1 ;
		if(i2c_start_data[6] == 200)
			i2c_start_address[6] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[6] , i2c_start_data[6]) ;
		delay_s1(15);
	}
}
void I2C_data_detection()
{
	if(Input_voltage_24V_error == 1)
	{
		i2c_start_data[0] = ReadByte_I2C(i2c_start_address[0]) ;
		i2c_start_data[0] += 1 ;
		if(i2c_start_data[0] == 200)
			i2c_start_address[0] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[0] , i2c_start_data[0]) ;
		delay_s1(15);
	}

	if(Input_voltage_24V_error == 2)
	{
		i2c_start_data[1] = ReadByte_I2C(i2c_start_address[1]) ;
		i2c_start_data[1] += 1 ;
		if(i2c_start_data[1] == 200)
			i2c_start_address[1] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[1] , i2c_start_data[1]) ;
		delay_s1(15);
	}

	if(Sensor_voltage_error == 1)
	{
		i2c_start_data[2] = ReadByte_I2C(i2c_start_address[2]) ;
		i2c_start_data[2] += 1 ;
		if(i2c_start_data[2] == 200)
			i2c_start_address[2] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[2] , i2c_start_data[2]) ;
		delay_s1(15);
	}

	if(Sensor_voltage_error == 2)
	{
		i2c_start_data[3] = ReadByte_I2C(i2c_start_address[3]) ;
		i2c_start_data[3] += 1 ;
		if(i2c_start_data[3] == 200)
			i2c_start_address[3] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[3] , i2c_start_data[3]) ;
		delay_s1(15);
	}

	if(T_voltage_error == 1)
	{
		i2c_start_data[4] = ReadByte_I2C(i2c_start_address[4]) ;
		i2c_start_data[4] += 1 ;
		if(i2c_start_data[4] == 200)
			i2c_start_address[4] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[4] , i2c_start_data[4]) ;
		delay_s1(15);
	}

	if(T_voltage_error == 2)
	{
		i2c_start_data[5] = ReadByte_I2C(i2c_start_address[5]) ;
		i2c_start_data[5] += 1 ;
		if(i2c_start_data[5] == 200)
			i2c_start_address[5] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[5] , i2c_start_data[5]) ;
		delay_s1(15);
	}

	if(LED_R_12_flag)
	{
		i2c_start_data[6] = ReadByte_I2C(i2c_start_address[6]) ;
		i2c_start_data[6] += 1 ;
		if(i2c_start_data[6] == 200)
			i2c_start_address[6] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[6] , i2c_start_data[6]) ;
		delay_s1(15);
	}

	if(Air_pressure_l_count2 == 1)
	{
		i2c_start_data[7] = ReadByte_I2C(i2c_start_address[7]) ;
		i2c_start_data[7] += 1 ;
		if(i2c_start_data[7] == 200)
			i2c_start_address[7] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[7] , i2c_start_data[7]) ;
		delay_s1(15);
	}
//	delay_s1(15);
}

void I2C_solenoid_valve()
{
	if(Recover_error == 1 && Input_voltage_detaction2 == 1)
	{
		i2c_start_data[8] = ReadByte_I2C(i2c_start_address[8]) ;
		i2c_start_data[8] += 1 ;
		if(i2c_start_data[8] == 200)
			i2c_start_address[8] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[8] , i2c_start_data[8]) ;
		delay_s1(15);
	}

	if(Recover_error == 1 && Input_voltage_detaction2 == 0)
	{
		i2c_start_data[9] = ReadByte_I2C(i2c_start_address[9]) ;
		i2c_start_data[9] += 1 ;
		if(i2c_start_data[9] == 200)
			i2c_start_address[9] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[9] , i2c_start_data[9]) ;
		delay_s1(15);
	}

	if(Recover_error == 2)
	{
		i2c_start_data[10] = ReadByte_I2C(i2c_start_address[10]) ;
		i2c_start_data[10] += 1 ;
		if(i2c_start_data[10] == 200)
			i2c_start_address[10] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[10] , i2c_start_data[10]) ;
		delay_s1(15);
	}

	if(Venting_error == 1 && Input_voltage_detaction2 == 1)
	{
		i2c_start_data[11] = ReadByte_I2C(i2c_start_address[11]) ;
		i2c_start_data[11] += 1 ;
		if(i2c_start_data[11] == 200)
			i2c_start_address[11] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[11] , i2c_start_data[11]) ;
		delay_s1(15);
	}

	if(Venting_error == 1 && Input_voltage_detaction2 == 0)
	{
		i2c_start_data[12] = ReadByte_I2C(i2c_start_address[12]) ;
		i2c_start_data[12] += 1 ;
		if(i2c_start_data[12] == 200)
			i2c_start_address[12] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[12] , i2c_start_data[12]) ;
		delay_s1(15);
	}

	if(Venting_error == 2)
	{
		i2c_start_data[13] = ReadByte_I2C(i2c_start_address[13]) ;
		i2c_start_data[13] += 1 ;
		if(i2c_start_data[13] == 200)
			i2c_start_address[13] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[13] , i2c_start_data[13]) ;
		delay_s1(15);
	}

	if(Backflow_error == 1 && Input_voltage_detaction1 == 1)
	{
		i2c_start_data[14] = ReadByte_I2C(i2c_start_address[14]) ;
		i2c_start_data[14] += 1 ;
		if(i2c_start_data[14] == 200)
			i2c_start_address[14] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[14] , i2c_start_data[14]) ;
		delay_s1(15);
	}

	if(Backflow_error == 1 && Input_voltage_detaction2 == 0)
	{
		i2c_start_data[15] = ReadByte_I2C(i2c_start_address[15]) ;
		i2c_start_data[15] += 1 ;
		if(i2c_start_data[15] == 200)
			i2c_start_address[15] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[15] , i2c_start_data[15]) ;
		delay_s1(15);
	}

	if(Backflow_error == 2)
	{
		i2c_start_data[16] = ReadByte_I2C(i2c_start_address[16]) ;
		i2c_start_data[16] += 1 ;
		if(i2c_start_data[16] == 200)
			i2c_start_address[16] += 17 ;
		delay1(25277);
		WirteByte_I2C(i2c_start_address[16] , i2c_start_data[16]) ;
		delay_s1(15);
	}
//	delay_s1(15);
}

void I2C_Read()
{
	uint8_t i = 0 ;
	for(i = 0 ; i < 250 ; i ++)
	{
		t[i] = ReadByte_I2C(i);
		delay1(25277);
	}
}
