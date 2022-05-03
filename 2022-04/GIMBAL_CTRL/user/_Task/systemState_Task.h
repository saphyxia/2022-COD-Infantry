#ifndef SYSTEMSTATE_TASK_H
#define SYSTEMSTATE_TASK_H
#include "FreeRTOS_Init.h"
#include "uart.h"

#define IS_STANDARD

//设备工作状态
typedef enum{
    OFF,//关闭（卸力）
    ON,//工作
    CALIBRATING,//掉线
    DEVICE_ERROR,//堵转，掉线等
    STATE_NUM,
}DEVICE_STATE;


typedef enum{
    INITIAL,
    BURST,
    COOLING,
    RATE,
    ROBOT_MODE_NUM,
}ROBOT_MODE;

typedef enum{
    LV_0,
    LV_1,
    LV_2,
    LV_3,
    ROBOT_LEVEL_NUM,
}ROBOT_LEVEL;


typedef __packed struct
{
	uint16_t	cooling_rate;		//枪口每秒冷却值
	uint16_t	cooling_limit;  //枪口热量上限
	uint16_t	speed_limit;    //枪口上限速度 单位 m/s
}ROBOT_SHOOTER_T;

typedef __packed struct 
{
		uint8_t id; //机器人Id 0红1蓝
    ROBOT_LEVEL level; //等级
		ROBOT_MODE mode; //发射机构类型
		uint8_t mains_power_shooter_output;//主控输出情况，0 无输出 1 24V
		uint16_t cooling_heat;   //枪口热量
		float bullet_speed;//实时射速
		ROBOT_SHOOTER_T shooter_id1_17mm;		//发射机构信息
		float Chas_Gyro;
}ROBOT;

extern ROBOT robot;


void Referee_get_Data(CAN_TypeDef* CANx,CanRxMsg* RxMsg);
#endif



