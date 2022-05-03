#ifndef UART2_H
#define UART2_H
#include "sys.h"
#include "main.h"

#define PLOT_ENABLE 1   //波形图软件1使用，0关闭


void uart2_Init(u32 bound);
void uart2_dma_Init(void);
void myprintf(int data,int data1);
#endif


