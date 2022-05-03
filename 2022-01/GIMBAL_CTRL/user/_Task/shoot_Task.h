#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H
#include "stm32f4xx.h"

#define SHOOT_SPEED_15M_S    4600
#define SHOOT_SPEED_18M_S    5100
#define SHOOT_SPEED_30M_S    7450

#define TRIGGER_FREQ_2_HZ     750
#define TRIGGER_FREQ_3_HZ     1000
#define TRIGGER_FREQ_4_HZ     1250
#define TRIGGER_FREQ_5_HZ     1500
#define TRIGGER_FREQ_6_HZ     1750
#define TRIGGER_FREQ_7_HZ     2000
#define TRIGGER_FREQ_8_HZ     2400
#define TRIGGER_FREQ_9_HZ     2700
#define TRIGGER_FREQ_10_HZ    3000


enum{
	INITIAL,//初始状态
	BURST,//爆发状态
	COOLING,//冷却优先
	RATE,//弹速优先
};

typedef __packed struct
{
    uint8_t robot_id;		//机器人ID
    uint8_t robot_level;	//机器人等级
	uint8_t shoot_type; 	//发射机构类型
    uint8_t mains_power_shooter_output; //主控电源输出情况：1为24v，0为无输出

		__packed struct
		{
			uint16_t	cooling_rate;	//枪口每秒冷却值
			uint16_t	cooling_heat;   //枪口热量
			uint16_t	cooling_limit;  //枪口热量上限
			uint16_t	speed_limit;    //枪口上限速度 单位 m/s
		} shooter_id1_17mm;//机器人 1 号 17mm
		
		__packed struct
		{
			uint16_t	cooling_rate;	//枪口每秒冷却值
			uint16_t	cooling_heat;   //枪口热量
			uint16_t	cooling_limit;  //枪口热量上限
			uint16_t	speed_limit;    //枪口上限速度 单位 m/s
		} shooter_id1_42mm;//机器人 1 号 42mm
		
}REFEREE_DATA_T;

extern REFEREE_DATA_T Robort_State;

void Trigger_AUTO_Ctrl(void);
void Trigger_SINGE_Ctrl(void);
bool Judge_IF_SingeStuck(void);
bool Judge_IF_AutoBlock(void);
void Referee_get_Data(CanRxMsg *std);
#endif



