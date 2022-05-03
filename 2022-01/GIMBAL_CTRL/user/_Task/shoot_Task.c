/*摩擦轮任务

*/

#include "FreeRTOS_Init.h"
#include "shoot_Task.h"
#include "gimbal_Task.h"
#include "imu_Task.h"
#include "dbus_Task.h"
#include "can1.h"
#include "rc.h"
#include "shoot.h"
#include "trigger.h"

/*--------------------------   变量声明   ------------------------------*/

//根据裁判系统数据改变初值
int16_t Shoot_Speed[2]={0,};
int16_t Trigger_Speed=0;
double Trigger_Angle=0;

int16_t Trigger_SendData=0;

bool IF_TRIGGER_READY=false;

REFEREE_DATA_T Robort_State;

int16_t Shoot_Speed_test = 0;

TRIGGER_STATE Shoot_state = TURN_OFF;
/*--------------------------   变量声明   ------------------------------*/

//调度任务
void SHOOT_TASK(void *args)
{
	uint32_t currentTime;
	
	Shoot_Motor_PID_Param_Init();
	Trigger_Motor_PID_Param_Init();

	Shoot[L].state = STURN_ON ;
	Shoot[R].state = STURN_ON ;

	Trigger.state = SINGLE;
	
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

		Key_B();//后改在状态机统一刷

		if(rc_ctrl.rc.s[0]==2)
		{
			switch(Robort_State.shooter_id1_17mm.speed_limit)
			{
				case 15:
					Shoot_Speed[L] = -SHOOT_SPEED_15M_S ;
				break;
				case 18:
					Shoot_Speed[L] = -SHOOT_SPEED_18M_S ;
				break;
				case 30:
					Shoot_Speed[L] = -SHOOT_SPEED_30M_S ;
				break;
				default:
					Shoot_Speed[L] = -SHOOT_SPEED_15M_S ;
				break;
			}
			Shoot_Speed[R] = -Shoot_Speed[L] ;
			
			if(Shoot_state!=Trigger.state)
			{
				Shoot_state = Trigger.state;
				Trigger_Angle = Trigger.Data.angle;
			}
			
			static uint16_t cnt  = 0;
			static int8_t trigger_stuck_flag = 1;
			
			if(Trigger.state == AUTO)
			{
				if(Judge_IF_AutoBlock() == true)
				{
					cnt++;
					if(cnt>150)
					{
						trigger_stuck_flag = -trigger_stuck_flag;
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 30)
						{
							Trigger_Speed *= trigger_stuck_flag;
						}else{
							Trigger_Speed = 0;
						}
						cnt = 0;
					}
				}else{
					Trigger_AUTO_Ctrl();
					if(cnt!=0)cnt = 0;
				}
				Trigger_SendData = Trigger.ctrl_Speed(-Trigger_Speed);
			}
			else if(Trigger.state == SINGLE)
			{
				if(Judge_IF_SingeStuck()==true)
				{
					cnt++;
					if(cnt > 150)
					{
						trigger_stuck_flag = -trigger_stuck_flag;
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 30)
						{
							Trigger_Angle = Trigger.Data.angle + 2*trigger_stuck_flag*8192.f/7.f;
						}
						cnt = 0;
					}
				}
				else{
					Trigger_SINGE_Ctrl();
					if(cnt!=0)cnt = 0;
				}
				Trigger_SendData = Trigger.ctrl_Angle(Trigger_Angle-Trigger.Data.angle);
			}
		}
		else{
			Shoot_Speed[L] = 0 ;
			Shoot_Speed[R] = -Shoot_Speed[L] ;
			Trigger_SendData = Trigger.ctrl_Speed(0);
		}
		Trigger_SendData = Trigger.ctrl_Speed(Shoot_Speed_test);

		CAN_Send_Msg(CAN1,0x200,Shoot[L].ctrl_Calc(Shoot_Speed[L]),Shoot[R].ctrl_Calc(Shoot_Speed[R]),Trigger_SendData);
		CAN_Send_Msg(CAN1,0x1ff,Trigger_SendData,0,0);
		
    vTaskDelayUntil(&currentTime, SHOOT_TASK_TIM);//绝对延时
	}
}
void Trigger_AUTO_Ctrl(void)
{
		Trigger_Speed = rc_ctrl.rc.ch[1]*3;
		if(Key_MOUSE_L()==true)
		{
				switch(Robort_State.shooter_id1_17mm.cooling_rate)
				{
					case 15:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_2_HZ;
						else 
							Trigger_Speed = 0;
					break;
					case 25:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_5_HZ;
						else 
							Trigger_Speed = TRIGGER_FREQ_2_HZ;
					break;
					case 35:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_6_HZ;
						else 
							Trigger_Speed = TRIGGER_FREQ_3_HZ;
					break;
					case 40:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_5_HZ;
						else 
							Trigger_Speed = TRIGGER_FREQ_4_HZ;
					break;
					case 60:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_8_HZ;
						else
							Trigger_Speed = TRIGGER_FREQ_6_HZ;
					break;
					case 80:
						if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
							Trigger_Speed = TRIGGER_FREQ_10_HZ;
						else 
							Trigger_Speed = TRIGGER_FREQ_8_HZ;							
					break;
				}
		}
}
void Trigger_SINGE_Ctrl(void)
{	
	if(rc_ctrl.rc.ch[1]!=0&&IF_TRIGGER_READY==false&&Judge_IF_SingeStuck() == false)
	{
		Trigger_Angle += rc_ctrl.rc.ch[1]/ABS(rc_ctrl.rc.ch[1])*8192.f/7.f;
		IF_TRIGGER_READY = true;
	}
	if(rc_ctrl.rc.ch[1]==0)
	{
		IF_TRIGGER_READY = false;
	}

	if(Key_MOUSE_L() == true&&Judge_IF_SingeStuck() == false)
	{
		if((Robort_State.shooter_id1_17mm.cooling_limit-Robort_State.shooter_id1_17mm.cooling_heat) >= 20)
		{
			Trigger_Angle = Trigger.Data.angle - 8192.f/7.f;
		}
	}
}

bool Judge_IF_SingeStuck(void)
{
	bool res = false;
	
	float angle_err = Trigger.Ctrl[SINGLE].angle_Loop.middle.Err;
	
	if(ABS(angle_err) >= 8192.f/13.f)
	{
				res = true;
	}
	
	return res;
}

bool Judge_IF_AutoBlock(void)
{
	bool res = false;
	
	float speed_err = Trigger.Ctrl[AUTO].speed_Loop.middle.Err;
	
	if(ABS(speed_err) > 500 && ABS(Trigger.Data.velocity) < 300)
	{
		res = true;
	}
	
	return res;
}

void Referee_get_Data(CanRxMsg *std)
{
	if(std->StdId!=0x302)
	{
		return;
	}


	uint8_t shooter_power = std->Data[0] >> 2; //主控电源输出情况：1为24v，0为无输出
	uint8_t shoot_type_L = std->Data[0] >> 3,shoot_type_H = std->Data[0] >> 4; //发射机构类型：00(初始)，01(爆发)，10(冷却)，11(弹速)
	uint8_t robot_LV_L = std->Data[0] >> 5,robot_LV_H = std->Data[0] >> 6;//等级情况，00(1级) 01(2级) 10(3级)
	uint8_t robot_id = std->Data[0] >> 7;//0红，1蓝

	Robort_State.robot_id = robot_id;//0红，1蓝

	if(robot_LV_H == 0&&robot_LV_L == 0)
	{
		Robort_State.robot_level = 1;
	}else if(robot_LV_H == 0&&robot_LV_L == 1)
	{
		Robort_State.robot_level = 2;
	}else if(robot_LV_H == 1&&robot_LV_L == 0)
	{
		Robort_State.robot_level = 3;
	}

	if(shoot_type_L == 0&& shoot_type_H == 0)
	{
		Robort_State.shoot_type = INITIAL;
	}else if(shoot_type_L == 0&& shoot_type_H == 1)
	{
		Robort_State.shoot_type = BURST;
	}else if(shoot_type_L == 1&& shoot_type_H == 0)
	{
		Robort_State.shoot_type = COOLING;
	}else if(shoot_type_L == 1&& shoot_type_H == 1)
	{
		Robort_State.shoot_type = RATE;
	}

	Robort_State.mains_power_shooter_output = shooter_power;
	Robort_State.shooter_id1_17mm.cooling_heat = (int16_t)std->Data[1] << 8 | (int16_t)std->Data[2];//17mm 枪口热量


	if(Robort_State.robot_level == 1)
	{
		if(Robort_State.shoot_type == INITIAL)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 50;
			Robort_State.shooter_id1_17mm.cooling_rate = 10;
			Robort_State.shooter_id1_17mm.speed_limit = 15;
		}else if(Robort_State.shoot_type == BURST)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 150;
			Robort_State.shooter_id1_17mm.cooling_rate = 15;
			Robort_State.shooter_id1_17mm.speed_limit = 15;
		}else if(Robort_State.shoot_type == COOLING)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 50;
			Robort_State.shooter_id1_17mm.cooling_rate = 40;
			Robort_State.shooter_id1_17mm.speed_limit = 15;
		}else if(Robort_State.shoot_type == RATE)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 75;
			Robort_State.shooter_id1_17mm.cooling_rate = 15;
			Robort_State.shooter_id1_17mm.speed_limit = 30;
		}
	}else if(Robort_State.robot_level == 2)
	{
		if(Robort_State.shoot_type == BURST)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 280;
			Robort_State.shooter_id1_17mm.cooling_rate = 25;
			Robort_State.shooter_id1_17mm.speed_limit = 15;
		}else if(Robort_State.shoot_type == COOLING)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 100;
			Robort_State.shooter_id1_17mm.cooling_rate = 60;
			Robort_State.shooter_id1_17mm.speed_limit = 18;
		}else if(Robort_State.shoot_type == RATE)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 150;
			Robort_State.shooter_id1_17mm.cooling_rate = 25;
			Robort_State.shooter_id1_17mm.speed_limit = 30;
		}
	}else if(Robort_State.robot_level == 3)
	{
		if(Robort_State.shoot_type == BURST)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 400;
			Robort_State.shooter_id1_17mm.cooling_rate = 35;
			Robort_State.shooter_id1_17mm.speed_limit = 15;
		}else if(Robort_State.shoot_type == COOLING)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 150;
			Robort_State.shooter_id1_17mm.cooling_rate = 80;
			Robort_State.shooter_id1_17mm.speed_limit = 18;
		}else if(Robort_State.shoot_type == RATE)
		{
			Robort_State.shooter_id1_17mm.cooling_limit = 200;
			Robort_State.shooter_id1_17mm.cooling_rate = 35;
			Robort_State.shooter_id1_17mm.speed_limit = 30;
		}
	}
}





