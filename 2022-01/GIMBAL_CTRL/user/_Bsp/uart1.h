#ifndef UART1_H
#define UART1_H
#include "main.h"

#define UART1_MAX_RX_LEN    10
#define UART1_MAX_TX_LEN    8


extern uint8_t UART1_RX_BUF[UART1_MAX_RX_LEN];//????1???????10
extern uint8_t UART1_TX_BUF[UART1_MAX_TX_LEN];//????1????,???9

typedef struct 
{
	int16_t	number;
	int16_t angle[4];

	int16_t pixel[2];
	int16_t ditance[2];
}camera_t;



void uart1_Init(u32 bound);
void uart1_dma_Init(void);

#endif


