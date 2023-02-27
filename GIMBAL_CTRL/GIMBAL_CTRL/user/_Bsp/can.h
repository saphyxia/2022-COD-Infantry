#ifndef CAN_H
#define CAN_H
#include "main.h"

enum{
    _CAN1,
    _CAN2,
    CAN_PORT_NUM,
};

enum{
    _0x200,
    _0x1ff,
    _0x300,
    stdID_NUM,
};

extern int L[8];
extern int H[8];

extern CanTxMsg CAN_TxMsg[CAN_PORT_NUM][stdID_NUM];

void CAN1_Init(void);
void CAN2_Init(void);


#endif




