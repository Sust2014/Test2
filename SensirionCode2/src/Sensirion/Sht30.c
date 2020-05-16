/*
 * Sht30.c
 *
 *  Created on: 2020��5��12��
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
	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������

	I2C1->D = (SHT30ADDR << 1);//����24C01��Ϊ���豸�ĵ�ַ
	while(!(I2C1->S1&0x02));//�ж��Ƿ�����ж�
	I2C1->S1 |= 1<<1;//����жϱ�־

	while(I2C1->S1&0x01);//�ж�Ӧ���ź� 0 �յ�  1 δ�յ�
}

void SHT3X_StartReadAccess()
{
	I2C_Start(I2C1) ;

	I2C1->D = ((SHT30ADDR << 1) | 0x01);//����24C01��Ϊ���豸�ĵ�ַ
	while(!(I2C1->S1&0x02));//�ж��Ƿ�����ж�
	I2C1->S1 |= 1<<1;//����жϱ�־

	while(I2C1->S1&0x01);//�ж�Ӧ���ź� 0 �յ�  1 δ�յ�
}

void SHT3X_WriteCommand(uint16_t command)
{
	// write the upper 8 bits of the command to the sensor
	I2C1->D = (command >> 8);//����24C01��Ϊ���豸�ĵ�ַ
	while(!(I2C1->S1&0x02));//�ж��Ƿ�����ж�
	I2C1->S1 |= 1<<1;//����жϱ�־

	while(I2C1->S1&0x01);//�ж�Ӧ���ź� 0 �յ�  1 δ�յ�

	// write the lower 8 bits of the command to the sensor
	I2C1->D = (command & 0x00FF);//����24C01��Ϊ���豸�ĵ�ַ
	while(!(I2C1->S1&0x02));//�ж��Ƿ�����ж�
	I2C1->S1 |= 1<<1;//����жϱ�־

	while(I2C1->S1&0x01);//�ж�Ӧ���ź� 0 �յ�  1 δ�յ�
}

void SHT3X_Read2BytesAndCrc()
{
	unsigned char temp_h;
	unsigned char temp_l;

	I2C1->C1 &= ~I2C_C1_TX_MASK;//����Ϊ����ģʽ

	I2C1->C1 |= 1<<3;//����Ӧ��ʹ��

	temp_h = I2C1->D;
	while(!(I2C1->S1&0x02));

	I2C1->S1 |= 1<<1;//����жϱ�־
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

	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	/******************д����******************/
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , (CMD_MEAS_CLOCKSTR_H >> 8)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_CLOCKSTR_H) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�
	I2C_Stop(I2C1) ;
	delay_s1(50) ;

	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x01)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	/******************������******************/
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
//	I2C_SendAck(I2C1);
	SenData[0] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	I2C_SendAck(I2C1);

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
//	I2C_SendAck(I2C1);
	SenData[1] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	I2C_SendAck(I2C1);

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
//	I2C_SendAck(I2C1);
	SenData[2] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	I2C_SendAck(I2C1);

	check_data = SenData[2] ;

	SHT3X_CalcCrc(SenData , 2 , check_data) ;

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[3] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[4] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendNack(I2C1);
	SenData[5] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	I2C_Stop(I2C1) ;

	float test_data1 = 0.0 ;
	float test_data2 = 0.0 ;

	test_data1 = 175.0 * ((SenData[0] << 8) | SenData[1]) / 65535.0 - 45.0 ;
	test_data2 = 100.0 * ((SenData[3] << 8) | SenData[4]) / 65535.0 ;

	delay_s1(50) ;
}

void SHT30_WorkProcess1(void)
{
	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	/******************д����******************/
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_PERI_1_H >> 8) & 0xFF)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((CMD_MEAS_PERI_1_H) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�
	I2C_Stop(I2C1) ;
	delay_s1(150) ;

	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x00)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((CMD_FETCH_DATA >> 8) & 0xFF)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((CMD_FETCH_DATA) & 0x00FF)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	I2C_Start(I2C1) ;//I2C��ʼͨ����Ҫ������
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C_TxEnable(I2C1);//ѡ����ģʽ
	I2C_WriteDataReg(I2C1 , ((SHT30ADDR << 1) | 0x01)) ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	while((I2C1->S1 & 0x01)) ;//�ȴ�Ӧ���ź�

	/******************������******************/
	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[0] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[1] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[2] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[3] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendAck(I2C1);
	SenData[4] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־

	while(!(I2C1->S1 & 0x80)) ;//������ɱ�־
	I2C1->C1 &= ~I2C_C1_TX_MASK;//ѡ�����ģʽ
	I2C_SendNack(I2C1);
	SenData[5] = I2C1->D ;
	while(!(I2C1->S1 & 0x02)) ;//�жϹ���
	I2C1->S1 |= 0x02 ;//����жϱ�־
	I2C_Stop(I2C1) ;
}
