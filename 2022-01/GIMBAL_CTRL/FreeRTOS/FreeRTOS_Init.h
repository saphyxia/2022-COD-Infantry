#ifndef __FREERTOS_INIT_H
#define __FREERTOS_INIT_H

#include "system.h"
#include "FreeRTOS.h"
#include "task.h"


#define     TASK_TIM_Unit   1  //FreeRTOS一毫秒单位,软实时1ms单位是1000
#define     FreeRTOS_1S_Unit     1000
/* 任务时数 */
#define     CHASSIS_TASK_TIM         ( 2    *TASK_TIM_Unit)
#define     GIMBAL_TASK_TIM          ( 1    *TASK_TIM_Unit)
#define     SHOOT_TASK_TIM           ( 2    *TASK_TIM_Unit)
#define     COVER_TASK_TIM           ( 2    *TASK_TIM_Unit)
#define     DBUS_TASK_TIM            ( 3    *TASK_TIM_Unit)
#define     VISION_TASK_TIM          ( 1    *TASK_TIM_Unit)
#define     IMU_TASK_TIM             ( 1    *TASK_TIM_Unit)
#define     SYSTEMSTATE_TASK_TIM     ( 5    *TASK_TIM_Unit)
#define     STATE_TASK_TIM				  	( 4    *TASK_TIM_Unit)
 

void FreeRTOS_Init(void);
void Delay_init(uint8_t SYSCLK);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
void delay_xms(uint32_t nms);


#endif


