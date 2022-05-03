#include "usart.h"
#include "sys.h"
#include "stdint.h"
#include "stm32f4xx_conf.h"
#include "stdlib.h"
#include "math.h"
                  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE
{ 
	int handle; 
}; 
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
FILE __stdout;       
//__use_no_semihosting was requested, but _ttywrch was 
void _ttywrch(int ch)
{
    ch = ch;
}
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART6->SR&0X40)==0);//循环发送,直到发送完毕   
    USART6->DR = (u8) ch;      
	return ch;
}
#endif 


/**
  * @name   Usart3_Configuration
  * @brief  串口3初始化函数(波形图)
  * @param
	*         bound：波特率:9600
  * @retval none
  * @attention none
*/
void Usart3_Configuration(u32 bound)
{
  //GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	
	USART_Init(USART3,&USART_InitStructure);
	
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//开启串口发送中断
	
	USART_Cmd(USART3,ENABLE);
	
	//使能串口空闲中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	

	NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @name   USART6_Configuration
  * @brief  串口6初始化函数(陀螺仪)
  * @param
  *         bound：波特率：921600
  * @retval none
  * @attention none
*/
void USART6_Configuration(u32 bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2,ENABLE); //使能GPIOG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//使能USART6时钟

	//串口6对应引脚复用映射
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9复用为USART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10复用为USART6

	//USART6端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9与GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOG,&GPIO_InitStructure); //初始化GPIOG9与GPIOG14

	//USART6 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口6


	USART_ClearFlag(USART6, USART_FLAG_TC);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_Cmd(USART6, ENABLE);  //使能串口6 
	

	//USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;      //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、
}

/**
  * @name   UART7_Configuration
  * @brief  串口7初始化函数(裁判系统)
  * @param
  *         bound：波特率：115200
  * @retval none
  * @attention none
*/
void UART7_Configuration(u32 bound)   //bound:波特率
{
	//GPIO端口设置
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE); //使能UART7时钟

	//串口7对应引脚复用映射
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7); //GPIOE7复用为UART7
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7); //GPIOE8复用为UART7

	//USART7端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; //GPIOE7与GPIOE8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化GPIOE7与GPIOE8

	//USART7 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(UART7, &USART_InitStructure); //初始化串口7


	USART_ClearFlag(UART7, USART_FLAG_TC);
	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_Cmd(UART7, ENABLE);  //使能串口7 
	

	//USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;//串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、
	UART7_DMA_Init();
}

#define TX_BUFF_SIZE 6//根据数据大小自行更改
uint8_t tx_buff[6];//此为DMA引用地址，把需要发送的数据进行赋值

void UART7_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART7->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)tx_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = TX_BUFF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream1, &DMA_InitStructure);
	USART_DMACmd(UART7, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
	
	
	DMA_Cmd(DMA1_Stream1, ENABLE);
}
//发送中断函数
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1) == SET)
	{
		DMA_Cmd(DMA1_Stream1, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Stream1, TX_BUFF_SIZE); 
	}
	DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
}

void Uart7_Send_Data(void)
{
		DMA_Cmd(DMA1_Stream1,ENABLE);
}
//此为自定发送函数 数组内容和变量类型自行定义
void myprintf(int data1,int data2)
{
				tx_buff[0] = 0xAA;
				tx_buff[1] = 0xBB;//帧头
        tx_buff[2] = (uint8_t)(data1 >> 8);
				tx_buff[3] = (uint8_t)(data1);
	      tx_buff[4] = (uint8_t)(data2 >> 8);
				tx_buff[5] = (uint8_t)(data2);
				Uart7_Send_Data();
}
/**
  * @name   UART8_Configuration
  * @brief  串口8初始化函数(超级电容)
  * @param
  *         bound：波特率：115200
  * @retval none
  * @attention none
*/
void UART8_Configuration(u32 bound)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //使能GPIOE时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE); //使能UART8时钟

	//串口8对应引脚复用映射
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE7复用为UART8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE8复用为UART8

	//USART8端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOE0与GPIOE1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化GPIOE0与GPIOE1

	//USART8 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //收发模式
	USART_Init(UART8, &USART_InitStructure); //初始化串口8


	USART_ClearFlag(UART8, USART_FLAG_TC);
	USART_Cmd(UART8, ENABLE);  //使能串口8 
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_ITConfig(UART8, USART_IT_TXE, ENABLE);//开启相关中断

	//USART8 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//串口8中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;      //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、
}




