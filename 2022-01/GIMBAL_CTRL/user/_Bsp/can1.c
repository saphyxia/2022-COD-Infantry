/* COD 开发板C型 CAN1 初始化程序
  
 使用说明：
        1.初始化CAN1，开启接收发送中断
        2.自行编写数据处理和发送
*/    
#include "can1.h"
#include "assist.h"
#include "gimbal.h"
#include "shoot.h"
#include "trigger.h"


float Chass_Gyro=0.f;

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
    CAN_InitStructure.CAN_BS1 = CAN_BS1_10tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
    CAN_InitStructure.CAN_Prescaler = 3;
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
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

CanRxMsg CAN1_RxMsg;

void CAN1_RX0_IRQHandler(void)
{
	CAN_Receive(CAN1, CAN_FIFO0, &CAN1_RxMsg);

//PITCH轴信息获取
    Gimbal[PITCH].get_Data(&CAN1_RxMsg);
		Gimbal[YAW].get_Data(&CAN1_RxMsg);
	
		if(CAN1_RxMsg.StdId==0x201){
        Shoot[L].Data.encoder = ((int16_t)CAN1_RxMsg.Data[0]<<8 | (int16_t)CAN1_RxMsg.Data[1]);
        Shoot[L].Data.velocity= ((int16_t)CAN1_RxMsg.Data[2]<<8 | (int16_t)CAN1_RxMsg.Data[3]);
        Shoot[L].Data.current = ((int16_t)CAN1_RxMsg.Data[4]<<8 | (int16_t)CAN1_RxMsg.Data[5]);
        Shoot[L].Data.temperature = CAN1_RxMsg.Data[6];
    }
		else if(CAN1_RxMsg.StdId==0x202){
        Shoot[R].Data.encoder = ((int16_t)CAN1_RxMsg.Data[0]<<8 | (int16_t)CAN1_RxMsg.Data[1]);
        Shoot[R].Data.velocity= ((int16_t)CAN1_RxMsg.Data[2]<<8 | (int16_t)CAN1_RxMsg.Data[3]);
        Shoot[R].Data.current = ((int16_t)CAN1_RxMsg.Data[4]<<8 | (int16_t)CAN1_RxMsg.Data[5]);
        Shoot[R].Data.temperature = CAN1_RxMsg.Data[6];
    }
		else if(CAN1_RxMsg.StdId==0x205){
        Trigger.Data.encoder = ((int16_t)CAN1_RxMsg.Data[0]<<8 | (int16_t)CAN1_RxMsg.Data[1]);
        Trigger.Data.velocity= ((int16_t)CAN1_RxMsg.Data[2]<<8 | (int16_t)CAN1_RxMsg.Data[3]);
        Trigger.Data.current = ((int16_t)CAN1_RxMsg.Data[4]<<8 | (int16_t)CAN1_RxMsg.Data[5]);
        Trigger.Data.temperature = CAN1_RxMsg.Data[6];
				 Trigger.Data.angle = Trigger_angle_AngleSum(&Trigger_critical,Trigger.Data.encoder);	
    }
//	if(CAN1_RxMsg.StdId==0x301)
//	{
//		Chass_Gyro=(float)((int16_t)CAN1_RxMsg.Data[0]<<8 |(int16_t)CAN1_RxMsg.Data[1])/1000;
//	}

}

CanTxMsg CAN1_TxMsg;
void CAN1_Tx(uint32_t stdId,uint8_t* data)
{
	CAN1_TxMsg.StdId=stdId;
	CAN1_TxMsg.IDE=CAN_ID_STD;
	CAN1_TxMsg.RTR=CAN_RTR_DATA;
	CAN1_TxMsg.DLC=0X08;
	
  memcpy(CAN1_TxMsg.Data,data,8);

	CAN_Transmit(CAN2, &CAN1_TxMsg); 
}
void CAN_Send_Msg(CAN_TypeDef *CAN,uint32_t id,int16_t send1,int16_t send2,int16_t send3)
{
	CanTxMsg TxMessage;
	TxMessage.StdId=id;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=0X08;

	TxMessage.Data[0]=(int8_t)(send1 >> 8);
	TxMessage.Data[1]=(int8_t)send1;
	TxMessage.Data[2]=(int8_t)(send2 >> 8);
	TxMessage.Data[3]=(int8_t)send2;
	TxMessage.Data[4]=(int8_t)(send3 >> 8);
	TxMessage.Data[5]=(int8_t)send3;
	
	CAN_Transmit(CAN, &TxMessage);    
}


