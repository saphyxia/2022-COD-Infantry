#include "usart.h"
#include "sys.h"
#include "stdint.h"
#include "stm32f4xx_conf.h"
#include "stdlib.h"
#include "math.h"
                  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE
{ 
	int handle; 
}; 
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
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
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART6->SR&0X40)==0);//ѭ������,ֱ���������   
    USART6->DR = (u8) ch;      
	return ch;
}
#endif 


/**
  * @name   Usart3_Configuration
  * @brief  ����3��ʼ������(����ͼ)
  * @param
	*         bound��������:9600
  * @retval none
  * @attention none
*/
void Usart3_Configuration(u32 bound)
{
  //GPIO�˿�����
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
	
	USART_ITConfig(USART3, USART_IT_TXE, ENABLE);//�������ڷ����ж�
	
	USART_Cmd(USART3,ENABLE);
	
	//ʹ�ܴ��ڿ����ж�
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
  * @brief  ����6��ʼ������(������)
  * @param
  *         bound�������ʣ�921600
  * @retval none
  * @attention none
*/
void USART6_Configuration(u32 bound){
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_DMA2,ENABLE); //ʹ��GPIOGʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);//ʹ��USART6ʱ��

	//����6��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_USART6); //GPIOA9����ΪUSART6
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6); //GPIOA10����ΪUSART6

	//USART6�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_14; //GPIOG9��GPIOG14
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOG,&GPIO_InitStructure); //��ʼ��GPIOG9��GPIOG14

	//USART6 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
	USART_Init(USART6, &USART_InitStructure); //��ʼ������6


	USART_ClearFlag(USART6, USART_FLAG_TC);

	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//��������ж�
	USART_Cmd(USART6, ENABLE);  //ʹ�ܴ���6 
	

	//USART6 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//����6�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;      //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

/**
  * @name   UART7_Configuration
  * @brief  ����7��ʼ������(����ϵͳ)
  * @param
  *         bound�������ʣ�115200
  * @retval none
  * @attention none
*/
void UART7_Configuration(u32 bound)   //bound:������
{
	//GPIO�˿�����
	GPIO_InitTypeDef   GPIO_InitStructure;
	USART_InitTypeDef  USART_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7,ENABLE); //ʹ��UART7ʱ��

	//����7��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_UART7); //GPIOE7����ΪUART7
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_UART7); //GPIOE8����ΪUART7

	//USART7�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8; //GPIOE7��GPIOE8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��GPIOE7��GPIOE8

	//USART7 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
	USART_Init(UART7, &USART_InitStructure); //��ʼ������7


	USART_ClearFlag(UART7, USART_FLAG_TC);
	USART_ITConfig(UART7, USART_IT_RXNE, ENABLE);//��������ж�
	USART_Cmd(UART7, ENABLE);  //ʹ�ܴ���7 
	

	//USART6 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;//����6�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;                //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        
	NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	UART7_DMA_Init();
}

#define TX_BUFF_SIZE 6//�������ݴ�С���и���
uint8_t tx_buff[6];//��ΪDMA���õ�ַ������Ҫ���͵����ݽ��и�ֵ

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
//�����жϺ���
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
//��Ϊ�Զ����ͺ��� �������ݺͱ����������ж���
void myprintf(int data1,int data2)
{
				tx_buff[0] = 0xAA;
				tx_buff[1] = 0xBB;//֡ͷ
        tx_buff[2] = (uint8_t)(data1 >> 8);
				tx_buff[3] = (uint8_t)(data1);
	      tx_buff[4] = (uint8_t)(data2 >> 8);
				tx_buff[5] = (uint8_t)(data2);
				Uart7_Send_Data();
}
/**
  * @name   UART8_Configuration
  * @brief  ����8��ʼ������(��������)
  * @param
  *         bound�������ʣ�115200
  * @retval none
  * @attention none
*/
void UART8_Configuration(u32 bound)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE); //ʹ��GPIOEʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART8,ENABLE); //ʹ��UART8ʱ��

	//����8��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_UART8); //GPIOE7����ΪUART8
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_UART8); //GPIOE8����ΪUART8

	//USART8�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //GPIOE0��GPIOE1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;        //�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��GPIOE0��GPIOE1

	//USART8 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        //�շ�ģʽ
	USART_Init(UART8, &USART_InitStructure); //��ʼ������8


	USART_ClearFlag(UART8, USART_FLAG_TC);
	USART_Cmd(UART8, ENABLE);  //ʹ�ܴ���8 
	USART_ITConfig(UART8, USART_IT_RXNE, ENABLE);//��������ж�
	USART_ITConfig(UART8, USART_IT_TXE, ENABLE);//��������ж�

	//USART8 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = UART8_IRQn;//����8�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;      //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                        //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);        //����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}




