#ifndef __USART_H
#define __USART_H
#include "stm32f4xx.h"




void Usart3_Configuration(u32 bound);   //bound:²¨ÌØÂÊ
void USART6_Configuration(u32 bound);
void UART7_Configuration(u32 bound);
void UART7_DMA_Init(void);
void DMA1_Stream1_IRQHandler(void);
void Uart7_Send_Data(void);
void myprintf(int data1,int data2);
void UART8_Configuration(u32 bound);
#endif


