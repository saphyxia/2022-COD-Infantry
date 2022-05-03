#include "CAN_Receive.h"
#include "state_task.h"
#include "resolve.h"
#include "can.h"
#include "imu.h"


float rx_power[4],rx_p=0,f;
CHASSIS_Info Chassis_Ctrl;
int8_t CAN1_DATA[3]={0};

void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN1, 0, &RxMessage);

	if(RxMessage.StdId==Chassis[Left_Font].Data.ID){
		Chassis[Left_Font].Data.encoder		=(int16_t)RxMessage.Data[0]<<8 |(int16_t)RxMessage.Data[1];
		Chassis[Left_Font].Data.velocity	=(int16_t)RxMessage.Data[2]<<8 |(int16_t)RxMessage.Data[3];
		Chassis[Left_Font].Data.current		=(int16_t)RxMessage.Data[4]<<8 |(int16_t)RxMessage.Data[5];
		Chassis[Left_Font].Data.temperature	=(int8_t)RxMessage.Data[6];
		System_State._Re_Time.CAN1_RE_Time[Left_Font] =xTaskGetTickCount()+100;
	}

	else if(RxMessage.StdId==Chassis[Righ_Font].Data.ID){
		Chassis[Righ_Font].Data.encoder		=(int16_t)RxMessage.Data[0]<<8 |(int16_t)RxMessage.Data[1];
		Chassis[Righ_Font].Data.velocity	=(int16_t)RxMessage.Data[2]<<8 |(int16_t)RxMessage.Data[3];
		Chassis[Righ_Font].Data.current		=(int16_t)RxMessage.Data[4]<<8 |(int16_t)RxMessage.Data[5];	
		Chassis[Righ_Font].Data.temperature	=(int8_t)RxMessage.Data[6];
		System_State._Re_Time.CAN1_RE_Time[Righ_Font] =xTaskGetTickCount()+100;
	}

	else if(RxMessage.StdId==Chassis[Left_Rear].Data.ID){
		Chassis[Left_Rear].Data.encoder		=(int16_t)RxMessage.Data[0]<<8 |(int16_t)RxMessage.Data[1];
		Chassis[Left_Rear].Data.velocity	=(int16_t)RxMessage.Data[2]<<8 |(int16_t)RxMessage.Data[3];
		Chassis[Left_Rear].Data.current		=(int16_t)RxMessage.Data[4]<<8 |(int16_t)RxMessage.Data[5];
		Chassis[Left_Rear].Data.temperature	=(int8_t)RxMessage.Data[6];
		System_State._Re_Time.CAN1_RE_Time[Left_Rear] =xTaskGetTickCount()+100;
	}

	else if(RxMessage.StdId==Chassis[Righ_Rear].Data.ID){
		Chassis[Righ_Rear].Data.encoder		=(int16_t)RxMessage.Data[0]<<8 |(int16_t)RxMessage.Data[1];
		Chassis[Righ_Rear].Data.velocity	=(int16_t)RxMessage.Data[2]<<8 |(int16_t)RxMessage.Data[3];
		Chassis[Righ_Rear].Data.current		=(int16_t)RxMessage.Data[4]<<8 |(int16_t)RxMessage.Data[5];	
		Chassis[Righ_Rear].Data.temperature	=(int8_t)RxMessage.Data[6];
		System_State._Re_Time.CAN1_RE_Time[Righ_Rear] =xTaskGetTickCount()+100;
	}

	else if(RxMessage.StdId==0x205)
	{
		CAN1_DATA[0]=RxMessage.Data[0];
		CAN1_DATA[1]=RxMessage.Data[1]; 
		CAN1_DATA[2]=RxMessage.Data[2];
	}

}

void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	CAN_Receive(CAN2, 0, &RxMessage);

	if(RxMessage.StdId==0x300)
	{
			Chassis_Ctrl.ctrl_Flag=RxMessage.Data[0];
			Chassis_Ctrl.mode_Flag=RxMessage.Data[1];
			Chassis_Ctrl.v[X]=((int16_t)RxMessage.Data[2]<<8 |(int16_t)RxMessage.Data[3]);
			Chassis_Ctrl.v[Y]=(int16_t)RxMessage.Data[4]<<8 |(int16_t)RxMessage.Data[5];
			Chassis_Ctrl.v[W]=(int16_t)RxMessage.Data[6]<<8 |(int16_t)RxMessage.Data[7];
			System_State._Re_Time.Remote_Re_Time=xTaskGetTickCount()+100;
	}
	
	else if(RxMessage.StdId==0x205){
		W_Temp_PID.Yaw_angle	=	(int16_t)RxMessage.Data[0]<<8 |(int16_t)RxMessage.Data[1];
	}
}



void CAN2_Send_Msg(void)
{
	CanTxMsg TxMessage;
	int16_t Send_Gyro=(int16_t)(MPU6050_Real_Data.Gyro_Z*1000);
	TxMessage.StdId=0x301;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=0X08;

	TxMessage.Data[0]=(int8_t)(Send_Gyro>>8);
	TxMessage.Data[1]=(int8_t)Send_Gyro;

	CAN_Transmit(CAN2, &TxMessage);   
}


void CAN_Send_Msg(CAN_TypeDef* CAN,int32_t CAN_StdID,int16_t *P)
{
	CanTxMsg TxMessage;
	TxMessage.StdId=CAN_StdID;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=0X08;

	TxMessage.Data[0]=(int8_t)(P[Left_Font]>>8);
	TxMessage.Data[1]=(int8_t)P[Left_Font];
	TxMessage.Data[2]=(int8_t)(P[Righ_Font]>>8);
	TxMessage.Data[3]=(int8_t)P[Righ_Font];
	TxMessage.Data[4]=(int8_t)(P[Left_Rear]>>8);
	TxMessage.Data[5]=(int8_t)P[Left_Rear];
	TxMessage.Data[6]=(int8_t)(P[Righ_Rear]>>8);
	TxMessage.Data[7]=(int8_t)P[Righ_Rear];

	CAN_Transmit(CAN, &TxMessage);   
}

//CAN 发送 0x700的ID的数据，会引发M3508进入快速设置ID模式
void CAN_CMD_CHASSIS_RESET_ID(void)
{

    CanTxMsg TxMessage;
    TxMessage.StdId = 0x700;
    TxMessage.IDE = CAN_ID_STD;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.DLC = 0x08;
	
    TxMessage.Data[0] = 0;
    TxMessage.Data[1] = 0;
    TxMessage.Data[2] = 0;
    TxMessage.Data[3] = 0;
    TxMessage.Data[4] = 0;
    TxMessage.Data[5] = 0;
    TxMessage.Data[6] = 0;
    TxMessage.Data[7] = 0;

    CAN_Transmit(CAN1, &TxMessage);
}


void CAN_Send_SetPower(CAN_TypeDef* CAN,int32_t CAN_StdID,int16_t tx_power)
{
	CanTxMsg TxMessage;
	TxMessage.StdId=CAN_StdID;
	TxMessage.IDE=CAN_ID_STD;
	TxMessage.RTR=CAN_RTR_DATA;
	TxMessage.DLC=0X08;

	TxMessage.Data[0]=(int8_t)(tx_power>>8);
	TxMessage.Data[1]=(int8_t)tx_power;
	
	CAN_Transmit(CAN, &TxMessage);   
}
