 #include "uart.h"
#include "vision.h"
#include "vision_Task.h"

/* COD 开发板C型 串口1 初始化程序
  
 使用说明：
	1.C板上uart1 初始化函数为串口6，对应 3pin接口
    2.开启DMA功能，用于视觉通讯
    3.自行编写数据处理部分
	4.接口从左至右为 GND→TX→RX
*/    

uint8_t UART1_RX_BUF[UART1_MAX_RX_LEN]={0};//定义串口接收缓存大小为10
uint8_t UART1_TX_BUF[UART1_MAX_TX_LEN]={0};//定义串口发送缓存，大小为9
uint8_t UART2_RX_BUF[UART2_MAX_RX_LEN]={0};//定义串口接收缓存大小为10
uint8_t UART2_TX_BUF[UART2_MAX_TX_LEN]={0};//定义串口发送缓存，大小为9


void uart1_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOG,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);

    GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); 
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); 

//GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOG,&GPIO_InitStructure);

//USART
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART6, &USART_InitStructure);
    USART_Cmd(USART6, ENABLE);
    USART_ITConfig(USART6,USART_IT_RXNE,ENABLE);
    USART_ITConfig(USART6,USART_IT_IDLE,ENABLE);
    USART_ClearFlag(USART6, USART_FLAG_TC);
	USART_DMACmd(USART6, USART_DMAReq_Tx, ENABLE);
	USART_DMACmd(USART6, USART_DMAReq_Rx, ENABLE);

//DMA
    //接收DMA2数据流2通道5
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;//DMA通道5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART1_RX_BUF;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = UART1_MAX_RX_LEN;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA2_Stream2, &DMA_InitStructure);
    DMA_Cmd(DMA2_Stream2, ENABLE);
    //发送DMA2数据流6通道5
    DMA_InitStructure.DMA_Channel = DMA_Channel_5;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART6->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART1_TX_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = UART1_MAX_TX_LEN;
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
	DMA_Init(DMA2_Stream6, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream6,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream6, ENABLE);

//NVIC
    NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void DMA2_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream6, DMA_IT_TCIF6) == SET)
	{
		DMA_Cmd(DMA2_Stream6, DISABLE);
		DMA_SetCurrDataCounter(DMA2_Stream6, UART1_MAX_TX_LEN); 
	}
	DMA_ClearITPendingBit(DMA2_Stream6, DMA_IT_TCIF6);
}

uint8_t USART6_Clear_Flag =0 ;

void USART6_IRQHandler(void)                	//串口6中断服务程序
{
    if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)  
    {
		USART6_Clear_Flag = USART6->DR;
		USART6_Clear_Flag = USART6->SR;
        DMA_Cmd(DMA2_Stream2,DISABLE );
	//数据处理函数
        Vision_Check_Data(UART1_RX_BUF); 

        DMA_Cmd(DMA2_Stream2,ENABLE);
    }
} 

/* COD 开发板C型 串口2 初始化程序
  
 使用说明：
		1.C板上uart2对应初始化函数为串口1，4pin接口
		2.此串口只有发送功能，用来发送数据至上位机查看波形图
		3.在任务中调用printf函数即可，建议发送频率不超过500hz
		4.接口从左至右为 RX→TX→GND→5V
*/
void uart2_Init(u32 bound)
{
    GPIO_InitTypeDef    GPIO_InitStructure;
    USART_InitTypeDef   USART_InitStructure;
	DMA_InitTypeDef     DMA_InitStructure;
    NVIC_InitTypeDef    NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); 
//GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
//USART
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ClearFlag(USART1, USART_FLAG_TC);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);

//DMA
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)UART2_TX_BUF;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = UART2_MAX_TX_LEN;
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
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	DMA_Cmd(DMA2_Stream7, ENABLE);

//NVIC
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        
    NVIC_Init(&NVIC_InitStructure);
}

void DMA2_Stream7_IRQHandler()
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) == SET)
	{
		DMA_Cmd(DMA2_Stream7, DISABLE);
		DMA_SetCurrDataCounter(DMA2_Stream7, UART2_MAX_TX_LEN); 
	}
	DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
}

void myprintf(int a,int b)//此为自定发送函数 数组内容和变量类型自行定义
{
	UART2_TX_BUF[0] = 0xAA;
	UART2_TX_BUF[1] = 0xBB;//帧头
	UART2_TX_BUF[2] = (uint8_t)(a >> 8);
	UART2_TX_BUF[3] = (uint8_t)(a);
	UART2_TX_BUF[4] = (uint8_t)(b >> 8);
	UART2_TX_BUF[5] = (uint8_t)(b);

	DMA_Cmd(DMA2_Stream7,ENABLE);
}









