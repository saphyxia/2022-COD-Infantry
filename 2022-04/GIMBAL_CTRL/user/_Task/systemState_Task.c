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
    .level=LV_0,
};
/*--------------------------   变量声明   ------------------------------*/

void SYSTEMSTATE_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		myprintf(gimbal.target.yaw_Angle*100,imu.yaw_Angle*100);
		currentTime = xTaskGetTickCount();//当前系统时间

		if(robot.mode == INITIAL)
		{
			robot.shooter_id1_17mm.cooling_limit = 50;
			robot.shooter_id1_17mm.cooling_rate = 10;
			robot.shooter_id1_17mm.speed_limit = 15;
		}
		else if(robot.mode == BURST)
		{
			if(robot.level == LV_2)
			{
				robot.shooter_id1_17mm.cooling_limit = 280;
				robot.shooter_id1_17mm.cooling_rate = 25;
				robot.shooter_id1_17mm.speed_limit = 15;
			}else if(robot.level == LV_3)
			{
				robot.shooter_id1_17mm.cooling_limit = 400;
				robot.shooter_id1_17mm.cooling_rate = 35;
				robot.shooter_id1_17mm.speed_limit = 15;
			}else
			{
				robot.shooter_id1_17mm.cooling_limit = 150;
				robot.shooter_id1_17mm.cooling_rate = 15;
				robot.shooter_id1_17mm.speed_limit = 15;
			}
		}else if(robot.mode == COOLING)
		{
			if(robot.level == LV_2)
			{
				robot.shooter_id1_17mm.cooling_limit = 100;
				robot.shooter_id1_17mm.cooling_rate = 60;
				robot.shooter_id1_17mm.speed_limit = 18;
			}else if(robot.level == LV_3)
			{
				robot.shooter_id1_17mm.cooling_limit = 150;
				robot.shooter_id1_17mm.cooling_rate = 80;
				robot.shooter_id1_17mm.speed_limit = 18;
			}else{
				robot.shooter_id1_17mm.cooling_limit = 50;
				robot.shooter_id1_17mm.cooling_rate = 40;
				robot.shooter_id1_17mm.speed_limit = 15;
			}
		}else if(robot.mode == RATE)
		{
			if(robot.level == LV_2)
			{
				robot.shooter_id1_17mm.cooling_limit = 150;
				robot.shooter_id1_17mm.cooling_rate = 25;
				robot.shooter_id1_17mm.speed_limit = 30;
			}else if(robot.level == LV_3)
			{
				robot.shooter_id1_17mm.cooling_limit = 200;
				robot.shooter_id1_17mm.cooling_rate = 35;
				robot.shooter_id1_17mm.speed_limit = 30;
			}else{
				robot.shooter_id1_17mm.cooling_limit = 75;
				robot.shooter_id1_17mm.cooling_rate = 15;
				robot.shooter_id1_17mm.speed_limit = 30;
			}
		}

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
		}else if(mode_L == 0&& mode_H == 1)
		{
			robot.mode = BURST;
		}else if(mode_L == 1&& mode_H == 0)
		{
			robot.mode = COOLING;
		}else if(mode_L == 1&& mode_H == 1)
		{
			robot.mode = RATE;
		}

		robot.cooling_heat = (int16_t)RxMsg->Data[1] << 8 | (int16_t)RxMsg->Data[2];//17mm 枪口热量
		robot.bullet_speed = ((int16_t)RxMsg->Data[3] << 8 | (int16_t)RxMsg->Data[4])/100.f; //17mm枪口射速
		robot.Chas_Gyro = ((int16_t)RxMsg->Data[5] << 8 | (int16_t)RxMsg->Data[6])/100.f;//底盘角速度
}

