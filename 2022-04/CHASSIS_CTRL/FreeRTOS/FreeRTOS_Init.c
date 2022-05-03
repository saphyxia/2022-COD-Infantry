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

/*状态机任务*/
#define STATE_TASK_PRIO              4       // 任务优先级
#define STATE_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t STATE_Task_Handler;              // 任务句柄
extern void STATE_TASK(void *args);

/*底盘任务*/
#define CHASSIS_TASK_PRIO              5       // 任务优先级
#define CHASSIS_STK_SIZE               1024     // 任务堆栈大小
TaskHandle_t CHASSIS_Task_Handler;              // 任务句柄
extern void CHASSIS_TASK(void *args);

/*功率控制任务*/
#define POWER_TASK_PRIO              5       // 任务优先级
#define POWER_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t POWER_Task_Handler;              // 任务句柄
extern void POWER_TASK(void *args);

/*裁判系统任务*/
#define REFEREE_TASK_PRIO              6       // 任务优先级
#define REFEREE_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t REFEREE_Task_Handler;              // 任务句柄
extern void REFEREE_TASK(void *args);

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

	/* 创建系统状态机任务 */
	#if IS_STATETASK_ENABLE
	xTaskCreate((TaskFunction_t		)STATE_TASK,						 // 任务函数
							(const char*		)"STATE_TASK",		 // 任务名称
							(uint16_t			)STATE_STK_SIZE,		 // 任务堆栈大小
							(void*				)NULL,						 // 传递给任务函数的参数
							(UBaseType_t		)STATE_TASK_PRIO,	     // 任务优先级
							(TaskHandle_t*		)&STATE_Task_Handler); // 任务句柄
	#endif

    /* 创建底盘任务 */
	#if IS_CHASSISTASK_ENABLE
    xTaskCreate((TaskFunction_t		)CHASSIS_TASK,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 
	#endif


    /* 创建功率任务 */
	#if IS_POWERTASK_ENABLE
    xTaskCreate((TaskFunction_t		)POWER_TASK,						 
							(const char*		)"POWER_TASK",		 
							(uint16_t			)POWER_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)POWER_TASK_PRIO,	     
							(TaskHandle_t*		)&POWER_Task_Handler); 
	#endif

    /* 创建功率任务 */
	#if IS_REFEREETASK_ENABLE
    xTaskCreate((TaskFunction_t		)REFEREE_TASK,						 
							(const char*		)"REFEREE_TASK",		 
							(uint16_t			)REFEREE_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)REFEREE_TASK_PRIO,	     
							(TaskHandle_t*		)&REFEREE_Task_Handler); 
	#endif

	vTaskDelete(SystemStart_Task_Handler);	//删除开始任务
	taskEXIT_CRITICAL();	// 退出临界区
}




