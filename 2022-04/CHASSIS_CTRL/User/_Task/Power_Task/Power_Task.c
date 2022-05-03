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
    currentTime = xTaskGetTickCount();//��ǰϵͳʱ��


    vTaskDelayUntil(&currentTime,POWER_TASK_TIM);
	}
}

void REFEREE_TASK(void *pvParameters)
{
  uint32_t currentTime;


	while(1)
	{
		currentTime = xTaskGetTickCount();//��ǰϵͳʱ��

		Referee_Data_Report();
		
		Startjudge_task();
		
		vTaskDelayUntil(&currentTime,REFEREE_TASK_TIM);
	}
}

void Referee_Data_Report(void)
{
	static uint8_t robot_id = 0;//0�죬1��
	static uint8_t robot_LV_L = 0,robot_LV_H = 0;//�ȼ������00(1��) 01(2��) 10(3��)
	static uint8_t shoot_type_L = 0,shoot_type_H = 0; //����������ͣ�00(��ʼ)��01(����)��10(��ȴ)��11(����)
	uint8_t shooter_power = game_robot_state.mains_power_shooter_output; //���ص�Դ��������1Ϊ24v��0Ϊ�����

	//id�ж�
	if(game_robot_state.robot_id > 99)
	{
		robot_id = 1;//��
	}else{
		robot_id = 0;//��
	}
	//�ȼ��ж�
	if(game_robot_state.robot_level==1)//1��
	{
		robot_LV_L = 0;
		robot_LV_H = 0;
		//������������ж�
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
	else if(game_robot_state.robot_level==2)//2��
	{
			robot_LV_L = 0;
			robot_LV_H = 1;
	}
	else if(game_robot_state.robot_level==3)//3��
	{
			robot_LV_L = 1;
			robot_LV_H = 0;
	}

	CAN_TxMsg[_CAN2][_0x302].Data[0] = robot_id << 7 | robot_LV_H << 6 | robot_LV_L << 5
									 | shoot_type_H << 4 | shoot_type_L << 3 | shooter_power << 2 ;//����������
	CAN_TxMsg[_CAN2][_0x302].Data[1] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat >> 8);//17mm ǹ������
	CAN_TxMsg[_CAN2][_0x302].Data[2] = (uint8_t)(power_heat_data.shooter_id1_17mm_cooling_heat	 );
	CAN_TxMsg[_CAN2][_0x302].Data[3] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100) >> 8);
	CAN_TxMsg[_CAN2][_0x302].Data[4] = (uint8_t)((int16_t)(real_shoot_data.bullet_speed*100)     );
}
/**
 *	@brief	���̼�������
 */
void Chassis_AcclerateCurve(float *speed_X,float *speed_Y)
{
	static float speed_line = 0.f;
  static float speed_linelast=0.f;//��һ�ε����ٶ�
  static bool  accelerating = false,decelerating = false;//�ٶ��������
  static float accCnt=0.f;//������ʱ
  static float speed_k = 0;//��������
  static float acck = 2.5f,deck = 20;//���١���������ϵ��
  static float deceleRecode_Y=0.f,deceleRecode_X=0.f;//�������߻����ٶ�
	
	/*���ٶȼ���*/
	speed_line = sqrt(powf(*speed_X,2.f)+powf(*speed_Y,2.f));

	/*�Ӽ����ж�*/
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

	/*��������*/
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

	/*��������*/
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

	/*�������*/
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

	/*������һ�ε����ٶ�*/
	speed_linelast = speed_line;
}

/**
 * @description: ��ʽԭ�� y = 1/(1+e^(-k(x-2/k)))  ��k = 4.2 , x = 1ʱ y = 0.9
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
 * @description: ��ʽԭ�� y =1/(1+e^(?k(-(x-(5/k))?2/k) ) )
  ��k = 10 , x = 0.8ʱ y = 0.01
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


