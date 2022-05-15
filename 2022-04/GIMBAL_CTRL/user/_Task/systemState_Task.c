/*状态机任务

*/


#include "systemState_Task.h"
#include "gimbal_Task.h"
#include "vision_Task.h"
#include "shoot_Task.h"
#include "vision.h"

/*--------------------------   变量声明   ------------------------------*/

ROBOT robot=
{
    .mode=INITIAL,
    .level=LV_1,
	.shooter_id1_17mm.cooling_limit = 50,
	.shooter_id1_17mm.cooling_rate = 10,
	.shooter_id1_17mm.speed_limit = 15,
};
ROBOT_SHOOTER_T Shooter_BURST[ROBOT_LEVEL_NUM]=
{
	[LV_1]={
		.cooling_limit = 150,
		.cooling_rate = 15,
		.speed_limit = 15,
	},
	[LV_2]={
		.cooling_limit = 280,
		.cooling_rate = 25,
		.speed_limit = 15,
	},
	[LV_3]={
		.cooling_limit = 400,
		.cooling_rate = 35,
		.speed_limit = 15,
	},
};
ROBOT_SHOOTER_T Shooter_COOLING[ROBOT_LEVEL_NUM]=
{
	[LV_1]={
		.cooling_limit = 50,
		.cooling_rate = 40,
		.speed_limit = 15,
	},
	[LV_2]={
		.cooling_limit = 100,
		.cooling_rate = 60,
		.speed_limit = 18,
	},
	[LV_3]={
		.cooling_limit = 150,
		.cooling_rate = 80,
		.speed_limit = 18,
	},
};
ROBOT_SHOOTER_T Shooter_RATE[ROBOT_LEVEL_NUM]=
{
	[LV_1]={
		.cooling_limit = 75,
		.cooling_rate = 15,
		.speed_limit = 30,
	},
	[LV_2]={
		.cooling_limit = 150,
		.cooling_rate = 25,
		.speed_limit = 30,
	},
	[LV_3]={
		.cooling_limit = 200,
		.cooling_rate = 35,
		.speed_limit = 30,
	},
};
/*--------------------------   变量声明   ------------------------------*/

void SYSTEMSTATE_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		myprintf(vision.tx2->pit_Err[0]* 100,vision.tx2->yaw_Err[0]*100);
		currentTime = xTaskGetTickCount();//当前系统时间

		vTaskDelayUntil(&currentTime, SYSTEMSTATE_TASK_TIM);//绝对延时
	}
}
uint16_t shoot_delay[20]={0,};
int delay_cnt=0;;
void Referee_get_Data(CAN_TypeDef* CANx,CanRxMsg* RxMsg)
{
	if(RxMsg->StdId!=0x302)
	{
		return;
	}

		uint8_t mode_L = (RxMsg->Data[0] & (uint8_t)(1 << 3)) >> 3,
				mode_H = (RxMsg->Data[0] & (uint8_t)(1 << 4)) >> 4; //发射机构类型：00(初始)，01(爆发)，10(冷却)，11(弹速)
		uint8_t robot_LV_L = (RxMsg->Data[0] & (uint8_t)(1 << 5)) >> 5,
				robot_LV_H = (RxMsg->Data[0] & (uint8_t)(1 << 6)) >> 6;//等级情况，00(1级) 01(2级) 10(3级)
	
		robot.id = (RxMsg->Data[0] & (uint8_t)(1 << 7)) >> 7;//0红，1蓝
		robot.mains_power_shooter_output = (RxMsg->Data[0] & (uint8_t)(1 << 2)) >> 2; //主控电源输出情况：1为24v，0为无输出

		if(robot_LV_H == 0&&robot_LV_L == 0)
		{
			robot.level = LV_1;
		}else if(robot_LV_H == 0&&robot_LV_L == 1)
		{
			robot.level = LV_2;
		}else if(robot_LV_H == 1&&robot_LV_L == 0)
		{
			robot.level = LV_3;
		}

		if(mode_L == 0&& mode_H == 0)
		{
			robot.mode = INITIAL;
			robot.shooter_id1_17mm.cooling_limit = 50;
			robot.shooter_id1_17mm.cooling_rate = 10;
			robot.shooter_id1_17mm.speed_limit = 15;
		}else if(mode_L == 0&& mode_H == 1)
		{
			robot.mode = BURST;
			robot.shooter_id1_17mm = Shooter_BURST[robot.level];
		}else if(mode_L == 1&& mode_H == 0)
		{
			robot.mode = COOLING;
			robot.shooter_id1_17mm = Shooter_COOLING[robot.level];
		}else if(mode_L == 1&& mode_H == 1)
		{
			robot.mode = RATE;
			robot.shooter_id1_17mm = Shooter_RATE[robot.level];
		}
		
		robot.cooling_heat = (int16_t)RxMsg->Data[1] << 8 | (int16_t)RxMsg->Data[2];//17mm 枪口热量
		robot.bullet_speed = ((int16_t)RxMsg->Data[3] << 8 | (int16_t)RxMsg->Data[4])/100.f; //17mm枪口射速
		robot.Chas_Gyro = ((int16_t)RxMsg->Data[5] << 8 | (int16_t)RxMsg->Data[6])/100.f;//底盘角速度
}

