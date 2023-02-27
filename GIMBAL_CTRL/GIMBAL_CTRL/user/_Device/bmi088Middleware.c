#include "bmi088Middleware.h"


void BMI088_ACCEL_NS_L(void)
{
    GPIO_ResetBits(GPIOA,GPIO_Pin_4);
}
void BMI088_ACCEL_NS_H(void)
{
    GPIO_SetBits(GPIOA,GPIO_Pin_4);
}
void BMI088_GYRO_NS_L(void)
{
    GPIO_ResetBits(GPIOB,GPIO_Pin_0);
}
void BMI088_GYRO_NS_H(void)
{
    GPIO_SetBits(GPIOB,GPIO_Pin_0);
}

uint8_t BMI088_read_write_byte(uint8_t txdata)
{
    uint8_t rx_data;
	//SPI的发送接收函数
    rx_data = SPI1_ReadWriteByte(txdata);
    return rx_data;
}

void BMI088_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        BMI088_delay_us(1000);
    }
}

void BMI088_delay_us(uint16_t us)
{

    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = us * 168;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}


