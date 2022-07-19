#include "usart.h"
#include "sys.h"
#include "stdint.h"
#include "stdlib.h"
#include "math.h"


void uart3_Init(u32 bound)
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
	
//	USART_ClearFlag(USART3,USART_FLAG_TXE);
//	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//开启串口发送中断
	
	USART_Cmd(USART3,ENABLE);
	
	//使能串口空闲中断
	USART_ClearFlag(USART3,USART_FLAG_RXNE);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
//	USART_ClearFlag(USART3,USART_FLAG_IDLE);
//	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	

	NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
float Cap_V = 0.f;
uint8_t USART3_RX[8];
void USART3_IRQHandler(void)
{
	static int i = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
		USART3_RX[i] = USART_ReceiveData(USART3);
		i++;

		if(USART3_RX[i]==0x44&&USART3_RX[(i+3)]==0x13)
		{
			Cap_V = ((int16_t)USART3_RX[(i+1)] << 8 | (int16_t)USART3_RX[(i+2)])*1.25f/1000.f;
		}
		
		if(i>=8)
		{
			i=0;
		}
	}
}

/**
  * @name   UART7_Configuration
  * @brief  ´®¿Ú7³õÊ¼»¯º¯Êý(²ÃÅÐÏµÍ³)
  * @param
  *         bound£º²¨ÌØÂÊ£º115200
  * @retval none
  * @attention none
*/
void uart7_Init(u32 bound)   
{
	//GPIO¶Ë¿ÚÉèÖÃ
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //Ê¹ÄÜGPIOEÊ±ÖÓ
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE); //Ê¹ÄÜUART7Ê±ÖÓ

	//´®¿Ú7¶ÔÓ¦Òý½Å¸´ÓÃÓ³Éä
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7); //GPIOE7¸´ÓÃÎªUART7
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7); //GPIOE8¸´ÓÃÎªUART7

	//USART7¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; //GPIOE7ÓëGPIOE8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//¸´ÓÃ¹¦ÄÜ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //ËÙ¶È50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì¸´ÓÃÊä³ö
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //ÉÏÀ­
	GPIO_Init(GPIOE,&GPIO_InitStructure); //³õÊ¼»¯GPIOE7ÓëGPIOE8

	//USART7 ³õÊ¼»¯ÉèÖÃ
	USART_InitStructure.USART_BaudRate = bound;//²¨ÌØÂÊÉèÖÃ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//×Ö³¤Îª8Î»Êý¾Ý¸ñÊ½
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//Ò»¸öÍ£Ö¹Î»
	USART_InitStructure.USART_Parity = USART_Parity_No;//ÎÞÆæÅ¼Ð£ÑéÎ»
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//ÎÞÓ²¼þÊý¾ÝÁ÷¿ØÖÆ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //ÊÕ·¢Ä£Ê½
	USART_Init(UART7, &USART_InitStructure); //³õÊ¼»¯´®¿Ú7


	USART_ClearFlag(UART7, USART_FLAG_TC);
	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);//¿ªÆôÏà¹ØÖÐ¶Ï
	USART_Cmd(UART7, ENABLE);  //Ê¹ÄÜ´®¿Ú7 
	

	//USART6 NVIC ÅäÖÃ
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;//´®¿Ú6ÖÐ¶ÏÍ¨µÀ
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//ÇÀÕ¼ÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //×ÓÓÅÏÈ¼¶3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        
	NVIC_Init(&NVIC_InitStructure);        //¸ù¾ÝÖ¸¶¨µÄ²ÎÊý³õÊ¼»¯VIC¼Ä´æÆ÷¡¢
	UART7_DMA_Init();
}

#define TX_BUFF_SIZE 6//¸ù¾ÝÊý¾Ý´óÐ¡×ÔÐÐ¸ü¸Ä
uint8_t tx_buff[6];//´ËÎªDMAÒýÓÃµØÖ·£¬°ÑÐèÒª·¢ËÍµÄÊý¾Ý½øÐÐ¸³Öµ

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
//·¢ËÍÖÐ¶Ïº¯Êý
void DMA1_Stream1_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1) == SET)
	{
		DMA_Cmd(DMA1_Stream1, DISABLE);
		DMA_SetCurrDataCounter(DMA1_Stream1, TX_BUFF_SIZE); 
	}
	DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
}

void uart7_Send_Data(void)
{
		DMA_Cmd(DMA1_Stream1,ENABLE);
}
//´ËÎª×Ô¶¨·¢ËÍº¯Êý Êý×éÄÚÈÝºÍ±äÁ¿ÀàÐÍ×ÔÐÐ¶¨Òå
void myprintf(int data1,int data2)
{
				tx_buff[0] = 0xAA;
				tx_buff[1] = 0xBB;//Ö¡Í·
        tx_buff[2] = (uint8_t)(data1 >> 8);
				tx_buff[3] = (uint8_t)(data1);
	      tx_buff[4] = (uint8_t)(data2 >> 8);
				tx_buff[5] = (uint8_t)(data2);
				uart7_Send_Data();
}
