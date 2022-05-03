#ifndef STATETASK_H
#define STATETASK_H
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f4xx.h"
#include "stdbool.h"
#include "motor.h"
#define IS_STATETASK_ENABLE 1   //使能状态机任务

//设备工作状态
typedef enum{
    DEV_OFF,//关闭
    DEV_ON,//启动
    DEV_ERROR,//异常
    DEV_WORK_Cnt,
}DEVICE_WORK_STATE;

/* 控制状态枚举 */
typedef enum
{
	Ctrl_Err,
  RC_CTRL,     //遥控器模式
  KEY_CTRL,    //键盘模式
  CTRL_MODE_Cnt ,
}CHASSIS_CTRL_MODE;

typedef enum
{
  Act_Err=0,
  INVA,//卸力         
  FOLO,//跟随云台     
  SPIN,//小陀螺       
  ACT_MODE_Cnt,
}CHASSIS_ACT_MODE;


void STATE_TASK(void *pvParameters);
#endif

