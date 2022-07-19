#include "InteracTask.h"
#include "UI.h"
#include "can.h"
/**
 *	@brief	交互任务
*/
void InteracTask(void *pvParameters) 
{
	TickType_t systick;
	
	while(1)
	{
        systick = xTaskGetTickCount();//当前系统时间
		
		Referee_Data_Report();
		
		Startjudge_task();

        vTaskDelayUntil(&systick,INTERACTASK_TIM);
	}
}

void Referee_Data_Report(void)
{
	static uint8_t robot_id = 0;//0红，1蓝
	static uint8_t robot_LV_L = 0,robot_LV_H = 0;//等级情况，00(1级) 01(2级) 10(3级)
	static uint8_t shoot_type_L = 0,shoot_type_H = 0; //发射机构类型：00(初始)，01(爆发)，10(冷却)，11(弹速)
	uint8_t shooter_power = game_robot_state.mains_power_shooter_output; //主控电源输出情况：1为24v，0为无输出

	//id判断
	if(game_robot_state.robot_id > 99)
	{
		robot_id = 1;//蓝
	}else{
		robot_id = 0;//红
	}
	//等级判断
	if(game_robot_state.robot_level==1)//1级
	{
		robot_LV_L = 0;
		robot_LV_H = 0;
		//发射机构类型判断
		if(game_robot_state.shooter_id1_17mm_cooling_limit == 150)
		{
			shoot_type_L = 0;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 75)
		{
			shoot_type_L = 1;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 50)
		{
			if(game_robot_state.shooter_id1_17mm_cooling_rate == 10)
			{
				shoot_type_L = 0;
				shoot_type_H = 0;
			}else if(game_robot_state.shooter_id1_17mm_cooling_rate == 40)
			{
				shoot_type_L = 1;
				shoot_type_H = 0;
			}
		}
	}
	else if(game_robot_state.robot_level==2)//2级
	{
			robot_LV_L = 0;
			robot_LV_H = 1;
		//发射机构类型判断
		if(game_robot_state.shooter_id1_17mm_cooling_limit == 280)
		{
			shoot_type_L = 0;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 150)
		{
			shoot_type_L = 1;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 100)
		{
			shoot_type_L = 1;
			shoot_type_H = 0;
		}
	}
	else if(game_robot_state.robot_level==3)//3级
	{
			robot_LV_L = 1;
			robot_LV_H = 0;
		//发射机构类型判断
		if(game_robot_state.shooter_id1_17mm_cooling_limit == 400)
		{
			shoot_type_L = 0;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 200)
		{
			shoot_type_L = 1;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id1_17mm_cooling_limit == 150)
		{
			shoot_type_L = 1;
			shoot_type_H = 0;
		}
	}

	CAN_TxMsg[_CAN2][_0x302].Data[0] = robot_id << 7 | robot_LV_H << 6 | robot_LV_L << 5
									 | shoot_type_H << 4 | shoot_type_L << 3 | shooter_power << 2 ;
	CAN_TxMsg[_CAN2][_0x302].Data[1] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat >> 8);
	CAN_TxMsg[_CAN2][_0x302].Data[2] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat	 );
	CAN_TxMsg[_CAN2][_0x302].Data[3] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100) >> 8);
	CAN_TxMsg[_CAN2][_0x302].Data[4] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100)     );
}

