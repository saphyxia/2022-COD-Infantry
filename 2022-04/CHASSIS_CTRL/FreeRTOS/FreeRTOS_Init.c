/*
    FreeRTOS _Task
*/

#include "FreeRTOS_Init.h"

extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
        xPortSysTickHandler();
    }
}

//start_Task
#define System_START_TASK_PRIO		1			// 任务优先级
#define System_START_STK_SIZE		128			// 任务堆栈大小
TaskHandle_t SystemStart_Task_Handler;			// 任务句柄
extern void System_Start_Task(void *args);

/*###-----↓↓↓↓↓↓↓↓↓↓↓↓----   Task Configuration Table   ----↓↓↓↓↓↓↓↓↓↓↓↓↓-----###*/

/*交互任务*/
#define INTERAC_TASK_PRIO              4       // 任务优先级
#define INTERAC_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t INTERAC_Task_Handler;              // 任务句柄
extern void InteracTask(void *args);

/*底盘任务*/
#define CHASSIS_TASK_PRIO              4       // 任务优先级
#define CHASSIS_STK_SIZE               1024     // 任务堆栈大小
TaskHandle_t CHASSIS_Task_Handler;              // 任务句柄
extern void ChassisTask(void *args);

/*陀螺仪任务*/
#define IMU_TASK_PRIO              4       // 任务优先级
#define IMU_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t IMU_Task_Handler;              // 任务句柄
extern void ImuTask(void *args);

/*###-----↑↑↑↑↑↑↑↑↑↑↑↑----   Task Configuration Table   ----↑↑↑↑↑↑↑↑↑↑↑↑↑-----###*/



/*  RTOS初始化，开启任务调度    */
void FreeRTOS_Init(void)
{
	/* 创建初始化任务 */
	xTaskCreate((TaskFunction_t		)System_Start_Task,						// 任务函数
							(const char*		)"System_Start_Task",		// 任务名称
							(uint16_t			)System_START_STK_SIZE,		// 任务堆栈大小
							(void*				)NULL,					    // 传递给任务函数的参数
							(UBaseType_t		)System_START_TASK_PRIO,	// 任务优先级
							(TaskHandle_t*		)&SystemStart_Task_Handler);	// 任务句柄
	vTaskStartScheduler();	// 开启任务调度
}

/**/
void System_Start_Task(void *args)
{
    taskENTER_CRITICAL();//进入临界区

	/* 创建状态机任务 */
	#if IS_IMUTASK_ENABLE
	xTaskCreate((TaskFunction_t		)ImuTask,						 // 任务函数
							(const char*		)"IMU_TASK",		 // 任务名称
							(uint16_t			)IMU_STK_SIZE,		 // 任务堆栈大小
							(void*				)NULL,						 // 传递给任务函数的参数
							(UBaseType_t		)IMU_TASK_PRIO,	     // 任务优先级
							(TaskHandle_t*		)&IMU_Task_Handler); // 任务句柄
	#endif

    /* 创建底盘任务 */
	#if IS_CHASSISTASK_ENABLE
    xTaskCreate((TaskFunction_t		)ChassisTask,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 
	#endif


    /* 创建交互任务 */
	#if IS_INTERACTASK_ENABLE
    xTaskCreate((TaskFunction_t		)InteracTask,						 
							(const char*		)"INTERAC_TASK",		 
							(uint16_t			)INTERAC_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)INTERAC_TASK_PRIO,	     
							(TaskHandle_t*		)&INTERAC_Task_Handler); 
	#endif

	vTaskDelete(SystemStart_Task_Handler);	//删除开始任务
	taskEXIT_CRITICAL();	// 退出临界区
}




