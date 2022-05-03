#include "Chassis_Task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "arm_math.h"
#include "can.h"
#include "usart.h"

//底盘状态，用作判断切换模式
CHASSIS_CTRL_MODE CHAS_ctrl = Ctrl_Err;

CHASSIS_INFO_T Chassis=
{
	.ctrl= RC_CTRL,
	.act = Act_Err,
	.midangle = 256,
};
void CHASSIS_TASK(void *pvParameters)
{
	uint32_t currentTime;
	
	CHASSIS_Reset(&Chassis);
	
	while(1)
	{
		//底盘控制
		CHASSIS_Ctrl();
    vTaskDelayUntil(&currentTime,CHASSIS_TASK_TIM);
	}
}

/**
 *	@brief	底盘控制
 */
void CHASSIS_Ctrl(void)
{
	//判断是否切换控制模式
	if(CHAS_ctrl != Chassis.ctrl)
	{	
		//重置底盘
		CHASSIS_Reset(&Chassis);
		//刷新控制模式
		CHAS_ctrl = Chassis.ctrl;
	}
	//模式切换
	if(CHAS_ctrl == RC_CTRL){
		CHASSIS_RcCtrl();
	}
	else if(CHAS_ctrl == KEY_CTRL){
		CHASSIS_KeyCtrl();
	}
	else{
		CHASSIS_Reset(&Chassis);
	}
	//变速曲线
	Chassis_AcclerateCurve(&Chassis.speed[_X],&Chassis.speed[_Y]);
	//底盘输出
	CHASSIS_Handler(Chassis.speed);
}

/**
 *	@brief	底盘重置,重新初始化pid
 */
void CHASSIS_Reset(CHASSIS_INFO_T *CHAS)
{
	//切换为卸力模式
	CHAS->act = INVA;
	//清空控制数据
	CHAS->CH2 = 0;
	CHAS->CH3 = 0;
	CHAS->key.V = 0;
	//重置PID
	CHASSIS_Set_PID(CHAS->ctrl);
	//底盘制动
	CHASSIS_Stop();
}
/**
 *	@brief	底盘测试
 */
void CHASSIS_Test(void)
{
	Chassis.speed[_X] = rc_ctrl.rc.ch[3] * RC_SEPPE_X_K;
	Chassis.speed[_Y] = rc_ctrl.rc.ch[2] * RC_SEPPE_Y_K;
	Chassis.speed[_W] = rc_ctrl.rc.ch[0] * 3;
}

float angle_yaw=0.f;
/**
 *	@brief	遥控器模式
 */
void CHASSIS_RcCtrl(void)
{
	
	float speed_k=0.f;

	Chassis.speed[_X] = Speed_X_deliver(Chassis.CH2 * RC_SEPPE_X_K,
																			Chassis.CH3 * RC_SEPPE_Y_K);
	Chassis.speed[_Y] = speed_Y_deliver(Chassis.CH2 * RC_SEPPE_X_K,
																			Chassis.CH3 * RC_SEPPE_Y_K);
	Chassis.speed[_W] = Speed_W_deliver(Chassis.act);

	#if IF_DR16_CONNECT
		CHASSIS_Test();
	#endif

	//小陀螺时重新分配速度
	if(ABS(Chassis.speed[_W]) >= 300)
	{
		speed_k = (CHAS_W_SPEED_MAX - ABS(Chassis.speed[_W]))/CHAS_W_SPEED_MAX;
		speed_k = powf(speed_k,1.5f);
		VAL_LIMIT(speed_k,0.f,1.f);
	}else{
		speed_k = 1.f;
	}
	Chassis.speed[_X] *= speed_k;
	Chassis.speed[_Y] *= speed_k;
}
/**
 *	@brief	键盘模式
 */
void CHASSIS_KeyCtrl(void)
{
	float speed_k=0.f;

	Chassis.speed[_X] = Speed_X_deliver(KEY_X_Ctrl() * KEY_SEPPE_X_K,KEY_Y_Ctrl() * KEY_SEPPE_Y_K);
	Chassis.speed[_Y] = speed_Y_deliver(KEY_X_Ctrl() * KEY_SEPPE_X_K,KEY_Y_Ctrl() * KEY_SEPPE_Y_K);
	Chassis.speed[_W] = Speed_W_deliver(Chassis.act);

	//小陀螺时重新分配速度
	if(ABS(Chassis.speed[_W]) >= 300)
	{
		speed_k = (CHAS_W_SPEED_MAX - ABS(Chassis.speed[_W]))/CHAS_W_SPEED_MAX;
		speed_k = powf(speed_k,1.2f);
		VAL_LIMIT(speed_k,0.f,1.f);
	}else{
		speed_k = 1.f;
	}
	Chassis.speed[_X] *= speed_k;
	Chassis.speed[_Y] *= speed_k;
}

/**
 *	@brief	获取底盘信息
 */
void get_Chassis_Info(CAN_TypeDef *CANx,CanRxMsg *RxMsg)
{
	if( CANx != CAN2)return ;

	if(RxMsg->StdId == 0x300)
	{
		Chassis.ctrl = (CHASSIS_CTRL_MODE)RxMsg->Data[0];//控制模式:异常,遥控器模式,键盘模式
		Chassis.act  = (CHASSIS_ACT_MODE) RxMsg->Data[1];//运动模式:异常,卸力,跟随云台,小陀螺
 
		Chassis.CH2 = (int16_t)RxMsg->Data[2]<<8 | (int16_t)RxMsg->Data[3];
		Chassis.CH3 = (int16_t)RxMsg->Data[4]<<8 | (int16_t)RxMsg->Data[5];

		Chassis.key.V = (uint16_t)RxMsg->Data[6]<<8 | (uint16_t)RxMsg->Data[7];//键盘
	}
}




