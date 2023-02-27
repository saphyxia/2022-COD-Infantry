#ifndef _INA226_H_
#define _INA226_H_
#include "stm32f4xx.h"
#include "stdbool.h"
/*
#define INA226_I2C_ADDRESS_CONF_0               (u8)(0x40 << 1)     // A0 = GND, A1 = GND
#define INA226_I2C_ADDRESS_CONF_1               (u8)(0x41 << 1)     // A0 = VS+, A1 = GND
#define INA226_I2C_ADDRESS_CONF_2               (u8)(0x42 << 1)     // A0 = SDA, A1 = GND
#define INA226_I2C_ADDRESS_CONF_3               (u8)(0x43 << 1)     // A0 = SCL, A1 = GND
#define INA226_I2C_ADDRESS_CONF_4               (u8)(0x44 << 1)     // A0 = GND, A1 = VS+
#define INA226_I2C_ADDRESS_CONF_5               (u8)(0x45 << 1)     // A0 = VS+, A1 = VS+
#define INA226_I2C_ADDRESS_CONF_6               (u8)(0x46 << 1)     // A0 = SDA, A1 = VS+
#define INA226_I2C_ADDRESS_CONF_7               (u8)(0x47 << 1)     // A0 = SCL, A1 = VS+
#define INA226_I2C_ADDRESS_CONF_8               (u8)(0x48 << 1)     // A0 = GND, A1 = SDA
#define INA226_I2C_ADDRESS_CONF_9               (u8)(0x49 << 1)     // A0 = VS+, A1 = SDA
#define INA226_I2C_ADDRESS_CONF_A               (u8)(0x4A << 1)     // A0 = SDA, A1 = SDA
#define INA226_I2C_ADDRESS_CONF_B               (u8)(0x4B << 1)     // A0 = SCL, A1 = SDA
#define INA226_I2C_ADDRESS_CONF_C               (u8)(0x4C << 1)     // A0 = GND, A1 = SCL
#define INA226_I2C_ADDRESS_CONF_D               (u8)(0x4D << 1)     // A0 = VS+, A1 = SCL
#define INA226_I2C_ADDRESS_CONF_E               (u8)(0x4E << 1)     // A0 = SDA, A1 = SCL
#define INA226_I2C_ADDRESS_CONF_F               (u8)(0x4F << 1)     // A0 = SCL, A1 = SCL
*/

#define INA226_I2C_ADDRESS_CONF_0               (u8)(0x40 << 1)     // A0 = GND, A1 = GND

// 寄存器地址
#define INA226_Config_Reg_ADDR        (u8)0x00  //配置寄存器
#define INA226_ShuntVoltage_Reg_ADDR  (u8)0x01  //分流电压寄存器
#define INA226_BusVoltage_Reg_ADDR    (u8)0x02  //总线电压寄存器
#define INA226_Power_Reg_ADDR         (u8)0x03  //电源功率寄存器
#define INA226_Current_Reg_ADDR       (u8)0x04  //电流寄存器
#define INA226_Calibration_Reg_ADDR   (u8)0x05  //校准寄存器

//配置寄存器（0x00）参数定义
#define INA226_CONFIG_RESET                    (0x8000) // Reset Bit                     (1000 0000 0000 0000)
#define INA226_Averaging_Mode                  (0x4000) //number of Average 512          (0100 0000 0000 0000)
#define INA226_BusV_Conversion_Time            (0x0100) //Bus Voltage Conversion Time    (0000 0001 0000 0000)
#define INA226_ShuntV_Conversion_Time          (0x0020) //Shunt Voltage Conversion Time  (0000 0000 0010 0000)
#define INA226_Operating_Mode                  (0x0007) //Operating Mode                 (0000 0000 0000 0111)

//INA配置的参数
#define INA_R           0.005f
#define INA_I_MAX	    	3.2768f
#define IAN_I_LSB	    	0.1f
#define INA_V_LSB       1.25f
#define INA_SV_LSB      0.0025f
#define INA_Power_LSB   2.5f
#define INA_CAL         10240
//根据上面定义设置 配置寄存器（0x00）的值
#define INA226_CONFIG_value	INA226_Averaging_Mode|INA226_BusV_Conversion_Time|INA226_ShuntV_Conversion_Time|INA226_Operating_Mode


void INA_Init(void );
int16_t INA_GET_Voltage_MV(void);//获取电压（单位：mv）
extern int16_t INA_GET_Shunt_Voltage_MV(void);	//获取电压（单位：mv）
int16_t INA_GET_Current_MA(void);//获取电流（单位：mA）
int16_t INA_GET_Power_MW(void);	//获取当前功率（单位：mw）
int16_t INA_GET_R_MV(void);
#endif
