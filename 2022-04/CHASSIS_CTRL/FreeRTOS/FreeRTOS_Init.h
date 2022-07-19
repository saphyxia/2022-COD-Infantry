#ifndef __FREERTOS_INIT_H
#define __FREERTOS_INIT_H
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "sys.h"


#define     TASK_TIM_Unit   1  //FreeRTOS一毫秒单位,软实时1ms单位是1000
#define     FreeRTOS_1S_Unit     1000
/* 任务时数 */
#define     CHASSISTASK_TIM         ( 2    *TASK_TIM_Unit)
#define     IMUTASK_TIM           ( 1    *TASK_TIM_Unit)
#define     INTERACTASK_TIM         ( 10   *TASK_TIM_Unit)

/* 任务创建使能 */
#define IS_IMUTASK_ENABLE   1
#define IS_CHASSISTASK_ENABLE 1
#define IS_INTERACTASK_ENABLE 1

void FreeRTOS_Init(void);
#endif


