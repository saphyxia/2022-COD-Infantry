#include "chassis_task.h"
#include "CAN_Receive.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "usart.h"
#include "state_task.h"
#include "task.h"
#include "motor.h"
#include "arm_math.h"
#include "CAN_Receive.h"
#include "usart.h"
#include "Remote_Control.h"
#include "supercap_task.h"
						

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t chassis_high_water;
#endif

int16_t Middle_angle=1700;//YAW轴机械中值
int16_t Spin_speed=400;//自旋速度
float Expect_W[Chassis_Motor_NUM];
int32_t GIMBAL_[2],CHASSIS_[3],acspeed_v[3];//控制输入
RAMP_C CHASSIS_RAMP[3];//斜坡曲线结构体
int chas_flag;

int16_t uart_send;

void chassis_task(void *pvParameters)
{
	uint32_t currentTime;

	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{		
//		chas_flag++;
		currentTime=xTaskGetTickCount();//当前系统时间
		
			Remote_Chassis();//遥控器模式
//		myprintf(acspeed_v[Y],Chassis[Left_Font].Data.velocity);
		myprintf(uart_send,0);
    vTaskDelayUntil(&currentTime,TIME_STAMP_1MS);
#if INCLUDE_uxTaskGetStackHighWaterMark
      chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
	}
}

//小陀螺平移底盘速度解算
void Chassis_Speed_Conversion(float Yaw_Err,int32_t *CHASSIS,int32_t *GIMBAL)
{
	float sin_yaw = 0.0f, cos_yaw = 0.0f,
	relative_angle=ABS(Yaw_Err)>4096 ? (Yaw_Err-Yaw_Err/ABS(Yaw_Err)*8192)*2.0f*PI/8192:Yaw_Err*2*PI/8192;
	sin_yaw = arm_sin_f32(relative_angle);
	cos_yaw = arm_cos_f32(relative_angle);

	CHASSIS[X] = cos_yaw * GIMBAL[X] - sin_yaw * GIMBAL[Y];
	CHASSIS[Y] = sin_yaw * GIMBAL[X] + cos_yaw * GIMBAL[Y];
}

		int16_t CAN1_Send_Data[Chassis_Motor_NUM]={0};

//麦克纳姆轮运动解算
void McNhamWheel_Solve(int32_t *p)
{
//	if(!System_State.Remote[Remote_StateNow])//若与云台通信正常
	{

		/*麦克纳姆轮运动公式计算*/
		Expect_W[Left_Font] = 1.0f/M_R*( p[X]+p[Y]+(M_L+M_LW)*p[W]/2);
		Expect_W[Righ_Font] = 1.0f/M_R*( p[X]-p[Y]+(M_L+M_LW)*p[W]/2);
		Expect_W[Left_Rear] = 1.0f/M_R*(-p[X]+p[Y]+(M_L+M_LW)*p[W]/2);
		Expect_W[Righ_Rear] = 1.0f/M_R*(-p[X]-p[Y]+(M_L+M_LW)*p[W]/2);
		//底盘电机状态更新
		Chassis[Left_Font].state=REMOTE;
		Chassis[Righ_Font].state=REMOTE;
		Chassis[Left_Rear].state=REMOTE;
		Chassis[Righ_Rear].state=REMOTE;
		
		/*速度环pid解算*/
		CAN1_Send_Data[Left_Font]=Chassis[Left_Font].speed_Ctrl(Left_Font,Chassis[Left_Font].state,(Expect_W[Left_Font]-Chassis[Left_Font].Data.velocity));
		CAN1_Send_Data[Righ_Font]=Chassis[Righ_Font].speed_Ctrl(Righ_Font,Chassis[Righ_Font].state,(Expect_W[Righ_Font]-Chassis[Righ_Font].Data.velocity));
		CAN1_Send_Data[Left_Rear]=Chassis[Left_Rear].speed_Ctrl(Left_Rear,Chassis[Left_Rear].state,(Expect_W[Left_Rear]-Chassis[Left_Rear].Data.velocity));
		CAN1_Send_Data[Righ_Rear]=Chassis[Righ_Rear].speed_Ctrl(Righ_Rear,Chassis[Righ_Rear].state,(Expect_W[Righ_Rear]-Chassis[Righ_Rear].Data.velocity));

		CAN_Send_Msg(CAN1,0x200,CAN1_Send_Data);//底盘控制CAN发送
	}
}

/*--------------------------------  底盘最终调用逻辑函数  -----------------------------*/
//遥控器模式
void Remote_Chassis(void)
{
	
	GIMBAL_[X]=rc_ctrl.rc.ch[2] *ROMOTE_MAGNIFY;
	GIMBAL_[Y]=rc_ctrl.rc.ch[3] *ROMOTE_MAGNIFY;

//	//获取云台控制信息
//	GIMBAL_[X]=Chassis_Ctrl.v[X]*ROMOTE_MAGNIFY;
//	GIMBAL_[Y]=Chassis_Ctrl.v[Y]*ROMOTE_MAGNIFY;
	
	//设置底盘运动模式
	if(Chassis_Ctrl.mode_Flag==FOLO)
	{
		CHASSIS_[W]=W_Temp_PID.gyro_Ctrl(&W_Temp_PID.Gyro,(Middle_angle-W_Temp_PID.Yaw_angle));//地盘跟随云台解算
	}
	else if(Chassis_Ctrl.mode_Flag==SPIN)
	{
		CHASSIS_[W]=Spin_speed;//小陀螺模式
	}
	else if(Chassis_Ctrl.mode_Flag==INVA||Chassis_Ctrl.mode_Flag==_RESET)
	{
		CHASSIS_[W]=0;//卸力模式
	}
//	Chassis_Speed_Conversion((Middle_angle-W_Temp_PID.Yaw_angle),CHASSIS_,GIMBAL_);//平移解算
		
	acspeed_v[W] = Ramp_Calculate(&CHASSIS_RAMP[W],rc_ctrl.rc.ch[0]);
	acspeed_v[X] = Ramp_Calculate(&CHASSIS_RAMP[X],GIMBAL_[X]); 
	acspeed_v[Y] = Ramp_Calculate(&CHASSIS_RAMP[Y],GIMBAL_[Y]);//斜坡函数处理

	McNhamWheel_Solve(acspeed_v);//底盘运动解算
}


