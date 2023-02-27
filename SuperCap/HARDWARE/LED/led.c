#include "led.h"
#include "stm32f10x_can.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	 //使能PB,PE端口时钟
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);//释放JTAG引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_5;				 //LED0-->PB.5 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
	GPIO_SetBits(GPIOB,GPIO_Pin_9);						 //PB.5 输出高
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOE2,3,4
	GPIO_SetBits(GPIOA,GPIO_PinSource15);
	GPIO_ResetBits(GPIOA,GPIO_PinSource15);
}

void CAN1_Init(void)
{ 
	GPIO_InitTypeDef 		GPIO_InitStructure; 
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
	NVIC_InitTypeDef  		NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PORTA时钟	                   											 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//使能CAN1时钟	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);			//初始化IO

	//CAN单元设置
	CAN_InitStructure.CAN_TTCM=DISABLE;			//非时间触发通信模式  
	CAN_InitStructure.CAN_ABOM=ENABLE;			//软件自动离线管理	 
	CAN_InitStructure.CAN_AWUM=DISABLE;			//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART=ENABLE;			//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM=DISABLE;		 	//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP=DISABLE;			//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode= CAN_Mode_Normal   ;	        //模式设置： mode:0,普通模式;1,回环模式; 
	//设置波特率
	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;				//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1=CAN_BS1_9tq; 			//Tbs1=tbs1+1个时间单位CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2=CAN_BS2_8tq;				//Tbs2=tbs2+1个时间单位CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler=2;        //分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);        	//初始化CAN1 

	CAN_FilterInitStructure.CAN_FilterNumber=0;	//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; 	//屏蔽位模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; 	//32位宽 
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;	//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;//激活过滤器0

	CAN_FilterInit(&CAN_FilterInitStructure);			//滤波器初始化
	
	CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);				//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}   


int8_t CAN1_DATA[3]={0,0,0};
int8_t CAN1_SEND[3]={0,0,0};


void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, 0, &RxMessage);

//	if(RxMessage.StdId==0x305)
//	{
//		CAN1_DATA[0]=RxMessage.Data[0];
//		CAN1_DATA[1]=RxMessage.Data[1]; 
//		CAN1_DATA[2]=RxMessage.Data[2];
//	}
}

CanTxMsg TxMessage;
void CAN1_TX(void)
{
	TxMessage.StdId=0x305;
	TxMessage.IDE=CAN_Id_Standard;
	TxMessage.RTR=CAN_RTR_Data;
	TxMessage.DLC=0X08;

	TxMessage.Data[0]=(uint8_t)CAN1_SEND[0];
	TxMessage.Data[1]=(uint8_t)CAN1_SEND[1];

	CAN_Transmit(CAN1, &TxMessage);   
}
