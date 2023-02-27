#include "ina226.h"
#include "myiic.h"
#include "delay.h"


void INA_Read_Byte_s(unsigned char reg,unsigned char *data)	//����λ����
{
	IIC_Start();
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0);	//����INA219��ַs
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0+0x01);	//����iicΪ����ģʽ����bit0��1���任��ʱ�ź�
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
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0);	//����INA219��ַ
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
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
}
int16_t INA_GET_Voltage_MV(void)	//��ȡ��ѹ����λ��mv��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_BusVoltage_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//
}
int16_t INA_GET_Shunt_Voltage_MV(void)	//��ȡ��ѹ����λ��mv��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_ShuntVoltage_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//
}
int16_t INA_GET_Current_MA(void)		//��ȡ��������λ��mA��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_Current_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);		//�õ��Ĵ�����ֵ�ڳ���ÿλ��Ӧ��ֵ��IAN_I_LSB���õ�ʵ�ʵĵ���
}
int16_t INA_GET_Power_MW(void)		//��ȡ��ǰ���ʣ���λ��mw��
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_Power_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//�õ��Ĵ�����ֵ�ڳ���ÿλ��Ӧ��ֵ��INA_Power_LSB���õ�ʵ�ʵĹ���
}
