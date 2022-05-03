#include "ina226.h"
#include "myiic.h"
#include "delay.h"


void INA_Read_Byte_s(unsigned char reg,unsigned char *data)	//读两位数据
{
	IIC_Start();
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0);	//发送INA219地址s
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0+0x01);	//设置iic为接收模式，将bit0置1将变换此时信号
	IIC_Wait_Ack();
	*data=IIC_Read_Byte(1);
	data++;
	*data=IIC_Read_Byte(0);
	IIC_Stop();
}
void INA_REG_Write(unsigned char reg,unsigned int data)	//写寄存器		测试成功
{
	unsigned char data_temp[2];
	data_temp[0]=(unsigned char )(data>>8);
	data_temp[1]=(unsigned char )(data & 0xFF);
	IIC_Start();
	IIC_Send_Byte(INA226_I2C_ADDRESS_CONF_0);	//发送INA219地址
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);							//发送寄存器地址
	IIC_Wait_Ack();
	IIC_Send_Byte(data_temp[0]);						//发送高8位数据
	IIC_Wait_Ack();
	data++;
	IIC_Send_Byte(data_temp[1])	;					//发送低8位数据
	IIC_Wait_Ack();
	IIC_Stop();
}

void INA_Init(void )	
{
	IIC_Init();
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
}
int16_t INA_GET_Voltage_MV(void)	//获取电压（单位：mv）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_BusVoltage_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//
}
int16_t INA_GET_Shunt_Voltage_MV(void)	//获取电压（单位：mv）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_ShuntVoltage_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//
}
int16_t INA_GET_Current_MA(void)		//获取电流（单位：mA）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_Current_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);		//得到寄存器的值在乘以每位对应的值（IAN_I_LSB）得到实际的电流
}
int16_t INA_GET_Power_MW(void)		//获取当前功率（单位：mw）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA226_Config_Reg_ADDR,INA226_CONFIG_value);
	INA_REG_Write(INA226_Calibration_Reg_ADDR,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA226_Power_Reg_ADDR,data_temp);
	return (int)((data_temp[0]<<8)+data_temp[1]);	//得到寄存器的值在乘以每位对应的值（INA_Power_LSB）得到实际的功率
}
