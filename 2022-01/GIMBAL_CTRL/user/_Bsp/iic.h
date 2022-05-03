#ifndef __IIC_H
#define __IIC_H
#include "stm32f4xx_i2c.h"





extern void I2C3_GPIO_INIT(void);
extern void IST8310_I2C_Read(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t readAddr, uint16_t NumByteToRead);
extern void IST8310_I2C_ByteWrite(uint8_t slaveAddr, uint8_t pBuffer, uint8_t writeAddr);

#endif



