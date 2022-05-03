/* COD 开发板C型 CAN1 初始化程序
  
 使用说明：
        1.初始化CAN1，开启接收发送中断
        2.自行编写数据处理和发送
*/    
#include "can.h"
#include "motor.h"
#include "systemState_Task.h"

#define SEND_ENABLE     1

int H[8] = { 0,2,4,6 , 0,2,4,6 };
int L[8] = { 1,3,5,7 , 1,3,5,7 };

CanTxMsg CAN_TxMsg[CAN_PORT_NUM][stdID_NUM]= 
{
	[_CAN1]={
				[_0x200]={
							.StdId=0x200,
							.IDE=CAN_ID_STD,
							.RTR=CAN_RTR_DATA,
							.DLC=8,
						},

				[_0x1ff]={
							.StdId=0x1ff,
							.IDE=CAN_ID_STD,
							.RTR=CAN_RTR_DATA,
							.DLC=8,
						},
			},
	[_CAN2]={
				[_0x1ff]={
							.StdId=0x1ff,
							.IDE=CAN_ID_STD,
							.RTR=CAN_RTR_DATA,
							.DLC=8,
						},

				[_0x300]={
							.StdId=0x300,
							.IDE=CAN_ID_STD,
							.RTR=CAN_RTR_DATA,
							.DLC=8,
						},
			},
};


void CAN1_Init(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN1, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = ENABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
    
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
    CAN_ITConfig(CAN1, CAN_IT_TME,  ENABLE);	

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void CAN2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_CAN2, DISABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_CAN2);

    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN2, &CAN_InitStructure);

    CAN_FilterInitStructure.CAN_FilterNumber = 14;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    CAN_ITConfig(CAN2, CAN_IT_FMP0, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

CanRxMsg CAN1_RxMsg;
void CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMsg);

//PITCH轴电机信息获取
    get_Motor_Data(CAN1,&CAN1_RxMsg,&Gimbal[PITCH]);
//摩擦轮电机信息获取
    get_Motor_Data(CAN1,&CAN1_RxMsg,&Gimbal[SHOOTL]);
    get_Motor_Data(CAN1,&CAN1_RxMsg,&Gimbal[SHOOTR]);
//拨弹盘电机信息获取
    get_Motor_Data(CAN1,&CAN1_RxMsg,&Gimbal[TRIGGER]);

#if SEND_ENABLE
    CAN_Transmit(CAN1,&CAN_TxMsg[_CAN1][_0x200]);
    CAN_Transmit(CAN1,&CAN_TxMsg[_CAN1][_0x1ff]);
#endif
}

CanRxMsg CAN2_RxMsg;
void CAN2_RX0_IRQHandler(void)
{
    CAN_Receive(CAN2, CAN_FIFO0, &CAN2_RxMsg);
//YAW轴信息获取
    get_Motor_Data(CAN2,&CAN2_RxMsg,&Gimbal[YAW]);
//底盘信息获取
    Referee_get_Data(CAN2,&CAN2_RxMsg);
#if SEND_ENABLE
    CAN_Transmit(CAN2,&CAN_TxMsg[_CAN2][_0x300]);
    CAN_Transmit(CAN2,&CAN_TxMsg[_CAN2][_0x1ff]);
#endif
}



