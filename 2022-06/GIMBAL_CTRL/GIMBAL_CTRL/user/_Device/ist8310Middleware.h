#ifndef IST8310MIDDLEWARE_H
#define IST8310MIDDLEWARE_H
#include "main.h"






void ist8310_RST_H(void);
void ist8310_RST_L(void);
void ist8310_delay_ms(uint16_t ms);
void ist8310_delay_us(uint16_t us);
uint8_t ist8310_IIC_read_single_reg(uint8_t reg);
void ist8310_IIC_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len);
void ist8310_IIC_write_single_reg(uint8_t reg, uint8_t data);
void ist8310_IIC_write_muli_reg(uint8_t reg, uint8_t *data, uint8_t len);



#endif


