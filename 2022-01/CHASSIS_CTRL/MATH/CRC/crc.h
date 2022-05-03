#ifndef __crc_H
#define __crc_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

//超级电容
typedef struct
{
    double current_U;
    double current_I;
    double set_U;
    double set_I;
    int get_count; //监视获得数据的次数
}_POWER,*P_POWER;

extern int8_t flag_uart8_conversion_rw;
extern int16_t Receive_Uart8_Buff[10];
extern float Super_C_Power;
extern _POWER power;

extern uint16_t N_CRC16(uint8_t *updata, int len);
extern void ProduceCRC(uint8_t *srcArray, int b);
extern void UART8_SendByte_r(void);
extern void UART8_SendByte_w(float set_U, float set_I);
#endif 


