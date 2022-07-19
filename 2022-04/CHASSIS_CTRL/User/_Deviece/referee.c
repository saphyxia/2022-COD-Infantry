#include "referee.h"
#include "crc.h"
#include "referee_info.h"
#include "FreeRTOS_Init.h"


//串口6接收数据缓冲
uint8_t Usart_Referee_Rx_Buf[REFEREE_DATALENGTH];
int uart8_Clean_IDLE_Flag = 0;

void Referee_USART_Init(void)   //bound:???
{
		//GPIO????
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //??GPIOE??
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE); //??UART7??
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
		DMA_DeInit(DMA1_Stream6);
	
		//??6????????
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE7???UART7
		GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE8???UART7

		//USART7????
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOE7?GPIOE8
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //??50MHz
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //??????
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //??
		GPIO_Init(GPIOE,&GPIO_InitStructure); //???GPIOE7?GPIOE8

		//USART6 ?????
		USART_InitStructure.USART_BaudRate = 115200;//?????
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//???8?????
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
		USART_InitStructure.USART_Parity = USART_Parity_No;//??????
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//????????
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //????
		USART_Init(UART8, &USART_InitStructure); //?????7


		USART_ClearFlag(UART8, USART_FLAG_TC);

			//????????
		USART_ITConfig(UART8,USART_IT_RXNE,ENABLE);
		USART_ITConfig(UART8,USART_IT_IDLE,ENABLE);
		USART_Cmd(UART8, ENABLE);  //????7 
		
		//USART6 NVIC ??
		NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//??6????
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4;//?????3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //????3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQ????
		NVIC_Init(&NVIC_InitStructure);        //??????????VIC????
}

void Referee_USART_DMA_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	
		USART_DMACmd(UART8,USART_DMAReq_Rx,ENABLE);
		
		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART8->DR);
		DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Usart_Referee_Rx_Buf;
		DMA_InitStructure.DMA_BufferSize = REFEREE_DATALENGTH;	
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
		
		DMA_Init(DMA1_Stream6, &DMA_InitStructure);
		DMA_Cmd(DMA1_Stream6, ENABLE);
		
//		USART_DMACmd(UART8,USART_DMAReq_Tx,ENABLE);
//		
//		DMA_InitStructure.DMA_Channel = DMA_Channel_5;
//		DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (UART8->DR);
//		DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//		DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Usart_Referee_Rx_Buf;
//		DMA_InitStructure.DMA_BufferSize = REFEREE_DATALENGTH;	
//		DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//		DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//		DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//		DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
//		DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//		DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
//		DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
//		DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//		DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//		DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//		
//		DMA_Init(DMA1_Stream0, &DMA_InitStructure);
//		DMA_Cmd(DMA1_Stream0, ENABLE);
}

void Referee_Init()
{
	Referee_USART_Init();
	Referee_USART_DMA_Init();
}

void UART8_IRQHandler(void)
{
		if(USART_GetITStatus(UART8, USART_IT_IDLE) != RESET)	//判断是否为空闲中断
    {
			
			u16 index=0;	//当前数据序号
			u16 data_length;	//帧数据长度
			
      uart8_Clean_IDLE_Flag = UART8->SR;
			uart8_Clean_IDLE_Flag = UART8->DR;//读DR寄存器，清除标志位

			uart8_Clean_IDLE_Flag = REFEREE_DATALENGTH - DMA_GetCurrDataCounter(DMA1_Stream6);
			
			DMA_Cmd(DMA1_Stream6,DISABLE );
			
			
				//裁判系统接收数据选择处理
				while(Usart_Referee_Rx_Buf[index]==0XA5) //帧头SOF校验
				{
					if(Verify_CRC8_Check_Sum(&Usart_Referee_Rx_Buf[index],5) == 1)
					{
						data_length = Usart_Referee_Rx_Buf[index+2]<<8 | Usart_Referee_Rx_Buf[index+1] + 9;
						if(Verify_CRC16_Check_Sum(&Usart_Referee_Rx_Buf[index],data_length))	//CRC16校验（CRC8不必再校验）
						{
							//裁判系统数据解析
							RefereeInfo_Decode(&Usart_Referee_Rx_Buf[index]);	
						}			
					}
					index+=data_length;
				}
				
				//数据清空
				memset(Usart_Referee_Rx_Buf,0,REFEREE_DATALENGTH);
				
				//重新填充DMA数据量
				DMA_SetCurrDataCounter(DMA1_Stream6,REFEREE_DATALENGTH);
												
				DMA_Cmd(DMA1_Stream6,ENABLE);

			}
}



