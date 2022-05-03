
/* COD 开发板C型 串口2 初始化程序
  
 使用说明：
		1.初始化函数为串口1，对应C板上uart2  4pin接口
		2.此串口只有发送功能，用来发送数据至上位机查看波形图
		3.在任务中调用printf函数即可，建议发送频率不超过500hz
		4.接口从左至右为 RX→TX→GND→5V
*/    
#include "uart2.h"
#include "JY901.h"
void uart2_Init(u32 bound)
{
//GPIO端口设置
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_USART1); 
	
	 //USART1口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	  GPIO_Init(GPIOB,&GPIO_InitStructure);


 //USART1初始化设置
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    USART_Init(USART1, &USART_InitStructure);
		

	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	USART_Cmd(USART1, ENABLE);  //使能串口7 
	

	//USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;//串口6中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        
	NVIC_Init(&NVIC_InitStructure);        //根据指定的参数初始化VIC寄存器、
	uart2_dma_Init();
}




#define TX_BUFF_SIZE 6//根据数据大小自行更改
uint8_t tx_buff[6];//此为DMA引用地址，把需要发送的数据进行赋值

void uart2_dma_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
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
	DMA_Init(DMA2_Stream7, &DMA_InitStructure);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);
	
	DMA_Cmd(DMA2_Stream7, ENABLE);
}

void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_Stream7, DMA_IT_TCIF7) == SET)
	{
		DMA_Cmd(DMA2_Stream7, DISABLE);
		DMA_SetCurrDataCounter(DMA2_Stream7, TX_BUFF_SIZE); 
	}
	DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_TCIF7);
}


void myprintf(int data,int data1)//此为自定发送函数 数组内容和变量类型自行定义
{
				tx_buff[0] = 0xAA;
				tx_buff[1] = 0xBB;//帧头
        tx_buff[2] = (uint8_t)(data >> 8);
				tx_buff[3] = (uint8_t)(data);
	      tx_buff[4] = (uint8_t)(data1 >> 8);
				tx_buff[5] = (uint8_t)(data1);
				DMA_Cmd(DMA2_Stream7,ENABLE);
}




