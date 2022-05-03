#ifndef CAN1_H
#define CAN1_H
#include "main.h"

extern float Chass_Gyro;

void CAN1_Init(void);
void CAN1_Tx(uint32_t id,uint8_t* data);
void CAN_Send_Msg(CAN_TypeDef *CAN,uint32_t id,int16_t send1,int16_t send2,int16_t send3);
#endif




