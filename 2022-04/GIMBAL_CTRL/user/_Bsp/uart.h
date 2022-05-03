#ifndef UART_H
#define UART_H
#include "main.h"

#define UART2_MAX_RX_LEN    10
#define UART2_MAX_TX_LEN    6

#define UART1_MAX_RX_LEN    12
#define UART1_MAX_TX_LEN    10

extern uint8_t UART1_RX_BUF[UART1_MAX_RX_LEN];//定义串口1接收缓存大小为10
extern uint8_t UART1_TX_BUF[UART1_MAX_TX_LEN];//定义串口1发送缓存，大小为9
extern uint8_t UART2_RX_BUF[UART2_MAX_RX_LEN];//定义串口2接收缓存大小为10
extern uint8_t UART2_TX_BUF[UART2_MAX_TX_LEN];//定义串口2发送缓存，大小为9


void uart1_Init(uint32_t bound);
void uart2_Init(uint32_t bound);
void myprintf(int a,int b);//此为自定发送函数 数组内容和变量类型自行定义

#endif


