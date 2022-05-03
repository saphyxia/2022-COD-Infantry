#ifndef CAN2_H
#define CAN2_H
#include "main.h"


void CAN2_Init(void);
void CAN2_Tx(uint32_t stdId,uint8_t* data);

#endif


