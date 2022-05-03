#include "ina219.h"
#include "myiic.h"
#include "delay.h"

INA219_GET_DATA_T INA219_GET={.voltage_mv=INA_GET_Voltage_MV,
															.current_ma=INA_GET_Current_MA,
															.power_mw  =INA_GET_Power_MW};

void INA_Read_Byte_s(unsigned char reg,unsigned char *data)	//����λ����
{
	IIC_Start();
	IIC_Send_Byte(INA219_ADDRESS);	//����INA219��ַs
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(INA219_ADDRESS+0x01);	//����iicΪ����ģʽ����bit0��1���任��ʱ�ź�
	IIC_Wait_Ack();
	*data=IIC_Read_Byte(1);
	data++;
	*data=IIC_Read_Byte(0);
	IIC_Stop();
}
void INA_REG_Write(unsigned char reg,unsigned int data)	//д�Ĵ���		���Գɹ�
{
	unsigned char data_temp[2];
	data_temp[0]=(unsigned char )(data>>8);
	data_temp[1]=(unsigned char )(data & 0xFF);
	IIC_Start();
	IIC_Send_Byte(INA219_ADDRESS);	//����INA219��ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);							//���ͼĴ�����ַ
	IIC_Wait_Ack();
	IIC_Send_Byte(data_temp[0]);						//���͸�8λ����
	IIC_Wait_Ack();
	data++;
	IIC_Send_Byte(data_temp[1])	;					//���͵�8λ����
	IIC_Wait_Ack();
	IIC_Stop();
}

void INA_Init(void )	
{
	IIC_Init();
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
}
static int16_t INA_GET_Voltage_MV(void)	//��ȡ��ѹ����λ��mv��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_BUSVOLTAGE,data_temp);
	return (int16_t)((((data_temp[0]<<8)+data_temp[1]) >> 3));	//����3Ϊȥ����bit2��CNVR��OVF��λ���ٳ��� 4MV (�ٷ��ĵ��涨)���õ���ǰ���ߵĵ�ѹֵ
}
static int16_t INA_GET_Current_MA(void)		//��ȡ��������λ��mA��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_CURRENT,data_temp);
	return (int16_t)((((data_temp[0]<<8)+data_temp[1])));		//�õ��Ĵ�����ֵ�ڳ���ÿλ��Ӧ��ֵ��IAN_I_LSB���õ�ʵ�ʵĵ���
}
static int16_t INA_GET_Power_MW(void)		//��ȡ��ǰ���ʣ���λ��mw��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_POWER,data_temp);
	return (int16_t)(((data_temp[0]<<8)+data_temp[1]));	//�õ��Ĵ�����ֵ�ڳ���ÿλ��Ӧ��ֵ��INA_Power_LSB���õ�ʵ�ʵĹ���
}
