#include "ina219.h"
#include "myiic.h"
#include "delay.h"

INA219_GET_DATA_T INA219_GET={.voltage_mv=INA_GET_Voltage_MV,
															.current_ma=INA_GET_Current_MA,
															.power_mw  =INA_GET_Power_MW};

void INA_Read_Byte_s(unsigned char reg,unsigned char *data)	//读两位数据
{
	IIC_Start();
	IIC_Send_Byte(INA219_ADDRESS);	//发送INA219地址s
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(INA219_ADDRESS+0x01);	//设置iic为接收模式，将bit0置1将变换此时信号
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
	IIC_Send_Byte(INA219_ADDRESS);	//发送INA219地址
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
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
}
static int16_t INA_GET_Voltage_MV(void)	//获取电压（单位：mv）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_BUSVOLTAGE,data_temp);
	return (int16_t)((((data_temp[0]<<8)+data_temp[1]) >> 3));	//右移3为去掉：bit2，CNVR，OVF三位，再乘以 4MV (官方文档规定)，得到当前总线的电压值
}
static int16_t INA_GET_Current_MA(void)		//获取电流（单位：mA）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_CURRENT,data_temp);
	return (int16_t)((((data_temp[0]<<8)+data_temp[1])));		//得到寄存器的值在乘以每位对应的值（IAN_I_LSB）得到实际的电流
}
static int16_t INA_GET_Power_MW(void)		//获取当前功率（单位：mw）
{
	unsigned char data_temp[2];
	INA_REG_Write(INA219_REG_CONFIG,INA219_CONFIG_value);
	INA_REG_Write(INA219_REG_CALIBRATION,INA_CAL);
	delay_ms(4);
	INA_Read_Byte_s(INA219_REG_POWER,data_temp);
	return (int16_t)(((data_temp[0]<<8)+data_temp[1]));	//得到寄存器的值在乘以每位对应的值（INA_Power_LSB）得到实际的功率
}
