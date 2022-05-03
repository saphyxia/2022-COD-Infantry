#ifndef __FREERTOS_INIT_H
#define __FREERTOS_INIT_H
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"
#include "State_Task.h"
#include "Power_Task.h"
#include "Chassis_Task.h"


#define     TASK_TIM_Unit   1  //FreeRTOS一毫秒单位,软实时1ms单位是1000
#define     FreeRTOS_1S_Unit     1000
/* 任务时数 */
#define     CHASSIS_TASK_TIM         ( 2    *TASK_TIM_Unit)
#define     STATE_TASK_TIM           ( 1    *TASK_TIM_Unit)
#define     POWER_TASK_TIM           ( 10    *TASK_TIM_Unit)
#define     REFEREE_TASK_TIM         ( 50   *TASK_TIM_Unit)

void FreeRTOS_Init(void);
#endif


