#ifndef CAN_H
#define CAN_H
#include "main.h"
#include "stm32f4xx.h"
#include "motor.h"
#include "stm32f4xx_can.h"

#define CAN1_NVIC 3       //CAN1中断控制优先级
#define CAN2_NVIC 3       //CAN2中断控制优先级

enum{
    _CAN1,
    _CAN2,
    CAN_PORT_NUM,
};

enum{
    _0x200,
    _0x1ff,
    _0x300,
    _0x301,
    _0x302,
    _0x303,
    stdID_NUM,
};

extern CanTxMsg CAN_TxMsg[CAN_PORT_NUM][stdID_NUM];
extern float Super_Cap_V;
extern int L[8];
extern int H[8];


void CAN1_Init(void);
void CAN2_Init(void);
#endif
