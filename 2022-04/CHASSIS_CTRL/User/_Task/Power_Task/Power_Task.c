#include "Power_Task.h"
#include "FreeRTOS_Init.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "UI.h"


void POWER_TASK(void *pvParameters)
{
  uint32_t currentTime;

	while(1)
	{
    currentTime = xTaskGetTickCount();//当前系统时间


    vTaskDelayUntil(&currentTime,POWER_TASK_TIM);
	}
}

void REFEREE_TASK(void *pvParameters)
{
  uint32_t currentTime;


	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

		Referee_Data_Report();
		
		Startjudge_task();
		
		vTaskDelayUntil(&currentTime,REFEREE_TASK_TIM);
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
		if(game_robot_state.shooter_id2_17mm_cooling_limit == 150)
		{
			shoot_type_L = 0;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id2_17mm_cooling_limit == 75)
		{
			shoot_type_L = 1;
			shoot_type_H = 1;
		}else if(game_robot_state.shooter_id2_17mm_cooling_limit == 50)
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
	}
	else if(game_robot_state.robot_level==3)//3级
	{
			robot_LV_L = 1;
			robot_LV_H = 0;
	}

	CAN_TxMsg[_CAN2][_0x302].Data[0] = robot_id << 7 | robot_LV_H << 6 | robot_LV_L << 5
									 | shoot_type_H << 4 | shoot_type_L << 3 | shooter_power << 2 ;//机器人类型
	CAN_TxMsg[_CAN2][_0x302].Data[1] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat >> 8);//17mm 枪口热量
	CAN_TxMsg[_CAN2][_0x302].Data[2] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat	 );
	CAN_TxMsg[_CAN2][_0x302].Data[3] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100) >> 8);
	CAN_TxMsg[_CAN2][_0x302].Data[4] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100)     );
}
/**
 *	@brief	底盘加速曲线
 */
void Chassis_AcclerateCurve(float *speed_X,float *speed_Y)
{
	static float speed_line = 0.f;
  static float speed_linelast=0.f;//上一次的线速度
  static bool  accelerating = false,decelerating = false;//速度增减情况
  static float accCnt=0.f;//自增计时
  static float speed_k = 0;//曲线增益
  static float acck = 2.5f,deck = 20;//加速、减速曲线系数
  static float deceleRecode_Y=0.f,deceleRecode_X=0.f;//减速曲线缓存速度
	
	/*线速度计算*/
	speed_line = sqrt(powf(*speed_X,2.f)+powf(*speed_Y,2.f));

	/*加减速判断*/
	if((ABS(speed_line) - ABS(speed_linelast)) > 1000)
	{
		accelerating = 1;
		decelerating = 0;
		accCnt = 0;
	}
	else if((ABS(speed_line) - ABS(speed_linelast)) < -1000)
	{
		accelerating = 0;
		decelerating = 1;
		accCnt = 0;
	}

	/*加速曲线*/
	if(accelerating == 1)
	{
		accCnt += 0.005f;
		speed_k = AcclerateCurve(accCnt,acck);
		if (speed_k > 0.999f)
        {
            accelerating = 0;
        }
	}
	else if(decelerating != 1)
    {
        speed_k = 1;
        accCnt = 0;
    }

	/*减速曲线*/
	if(decelerating == 1)
	{
		accCnt += 0.005f;
		speed_k = DecclerateCurve(accCnt,deck);
		if (speed_k < 0.01f)
        {
            decelerating = 0;
        }
	}
	else if(accelerating != 1)
    {
        speed_k = 1;
        accCnt = 0;
    }

	/*增益计算*/
	if(accelerating == 1)
	{
		*speed_Y *= speed_k;
		*speed_X *= speed_k;
	}
	else if(decelerating == 1)
	{
		*speed_Y = deceleRecode_Y * speed_k;
		*speed_X = deceleRecode_X * speed_k;
	}
	if(decelerating != 1)
	{
		deceleRecode_Y = *speed_Y;
		deceleRecode_X = *speed_X;
	}

	/*更新上一次的线速度*/
	speed_linelast = speed_line;
}

/**
 * @description: 公式原型 y = 1/(1+e^(-k(x-2/k)))  当k = 4.2 , x = 1时 y = 0.9
 */
static float AcclerateCurve(float x , float k)
{
	float y;
	k = 4.2f / k;
	if(k == 0)
	{
		return 1;
	}
	y = 1/(1+powf(NATURAL_NUMBER,(-k*(x-2/k))));
	return y;
}

/**
 * @description: 公式原型 y =1/(1+e^(?k(-(x-(5/k))?2/k) ) )
  当k = 10 , x = 0.8时 y = 0.01
 */
static float DecclerateCurve(float x , float k)
{
	float y;
	if(k == 0)
	{
		return 1;
	}
	y = 1/(1+powf(NATURAL_NUMBER,(-k*(-x+3/k))));
	return y;
}


