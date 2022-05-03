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

typedef struct 
{
    ROBOT_MODE mode;
    ROBOT_LEVEL level;
}ROBOT;
extern ROBOT robot;




#endif



