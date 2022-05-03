#include "Start_Task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Chassis_Task.h"
#include "supercap_task.h"
#include "state_task.h"
#include "imu_task.h"


#define IMU_TASK_PRIO 4
#define IMU_STK_SIZE 512
static TaskHandle_t IMUTask_Handler;

#define Chassis_TASK_PRIO 4
#define Chassis_STK_SIZE 512
TaskHandle_t ChassisTask_Handler;

#define State_TASK_PRIO 4
#define State_STK_SIZE 512
TaskHandle_t StateTASK_Handler;

#define SuperCap_TASK_PRIO 3
#define SuperCap_STK_SIZE 512
TaskHandle_t SuperCapTASK_Handler;

#define START_TASK_PRIO 1
#define START_STK_SIZE 512
static TaskHandle_t StartTask_Handler;


void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();

	    xTaskCreate((TaskFunction_t)state_task,
                (const char *)"StateTASK",
                (uint16_t)State_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)State_TASK_PRIO,
                (TaskHandle_t *)&StateTASK_Handler);
								
		xTaskCreate((TaskFunction_t)supercap_task,
                (const char *)"SuperCapTASK",
                (uint16_t)SuperCap_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)SuperCap_TASK_PRIO,
                (TaskHandle_t *)&SuperCapTASK_Handler);
								
    xTaskCreate((TaskFunction_t)chassis_task,
                (const char *)"ChassisTask",
                (uint16_t)Chassis_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)Chassis_TASK_PRIO,
                (TaskHandle_t *)&ChassisTask_Handler);

		xTaskCreate((TaskFunction_t)imu_task,
                (const char *)"IMUTASK",
                (uint16_t)IMU_STK_SIZE,
                (void *)NULL,
                (UBaseType_t)IMU_TASK_PRIO,
                (TaskHandle_t *)&IMUTask_Handler);
								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}
