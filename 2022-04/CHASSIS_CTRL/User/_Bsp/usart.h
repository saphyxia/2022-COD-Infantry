#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"

extern float Cap_V;

void uart3_Init(uint32_t bound);
void UART7_DMA_Init(void);
void uart7_Init(uint32_t bound);
void myprintf(int data1,int data2);
#endif


