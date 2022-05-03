#ifndef POWERTASK_H
#define POWERTASK_H
#include "main.h"
#include "referee_info.h"

#define IS_POWERTASK_ENABLE 1   //使能功率任务
#define IS_REFEREETASK_ENABLE 1 //使能裁判系统任务


typedef struct
{
	uint8_t robot_id : 1;//0红，1蓝
	uint8_t robot_LV_L :1;//机器人等级，00(1级) 01(2级) 10(3级)
	uint8_t robot_LV_H :1;
	uint8_t shoot_type_L :1;//发射机构类型：00(初始)，01(爆发)，10(冷却)，11(弹速)
	uint8_t shoot_type_H :1;
	uint8_t shooter_power :1;//发射机构电源输出情况：1为24v，0为无输出
	
	uint8_t shooter_cooling_heat_L;//17mm 枪口热量
	uint8_t shooter_cooling_heat_H;
	
	uint8_t bullet_speed_L;//17mm 实时射速
	uint8_t bullet_speed_H;

}Robot_Info_T;



void POWER_TASK(void *pvParameters);
void REFEREE_TASK(void *pvParameters);
void Referee_Data_Report(void);
void Chassis_AcclerateCurve(float *speed_X,float *speed_Y);
static float AcclerateCurve(float x , float k);
static float DecclerateCurve(float x , float k);
#endif
