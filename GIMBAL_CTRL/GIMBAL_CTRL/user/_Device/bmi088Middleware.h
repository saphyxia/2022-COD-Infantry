#ifndef BMI088MIDDLEWARE_H
#define BMI088MIDDLEWARE_H
#include "main.h"
#include "ist8310.h"
#include "ist8310Middleware.h"

#define BMI088_USE_SPI 

void BMI088_delay_ms(uint16_t ms);
void BMI088_delay_us(uint16_t us);
uint8_t BMI088_read_write_byte(uint8_t txdata);

#if defined(BMI088_USE_SPI)

extern void BMI088_ACCEL_NS_L(void);
extern void BMI088_ACCEL_NS_H(void);

extern void BMI088_GYRO_NS_L(void);
extern void BMI088_GYRO_NS_H(void);

#endif

#endif

