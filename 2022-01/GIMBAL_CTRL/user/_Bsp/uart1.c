#include "uart1.h"
#include "vision.h"


/* COD 开发板C型 串口1 初始化程序
  
 使用说明：
	 1.初始化函数为串口6，对应C板上uart1  3pin接口
   2.开启DMA功能，用于视觉通讯
   3.自行编写数据处理部分
	 4.接口从左至右为 GND→TX→RX
*/    

uint8_t UART1_RX_BUF[UART1_MAX_RX_LEN]={0};//定义串口接收缓存大小为10
uint8_t UART1_TX_BUF[UART1_MAX_TX_LEN]={0};//定义串口发送缓存，大小为9


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

int USART6_IDLE_CLEAR_FLAG = 0;

void USART6_IRQHandler(void)                	//串口6中断服务程序
{
    if(USART_GetITStatus(USART6, USART_IT_IDLE) != RESET)  
    {
			USART6_IDLE_CLEAR_FLAG = USART6->SR;
			USART6_IDLE_CLEAR_FLAG = USART6->DR;
			
			DMA_Cmd(DMA2_Stream2,DISABLE );
			
			USART6_IDLE_CLEAR_FLAG=VISION_BUFFER_LEN - DMA_GetCurrDataCounter(DMA2_Stream2);

		//数据处理函数
 			Vision_Check_Data(UART1_RX_BUF);

			DMA_Cmd(DMA2_Stream2,ENABLE);
		
			USART_ClearITPendingBit(USART6,USART_IT_IDLE);
    }
}








