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
#define System_START_TASK_PRIO		1			// �������ȼ�
#define System_START_STK_SIZE		128			// �����ջ��С
TaskHandle_t SystemStart_Task_Handler;			// ������
extern void System_Start_Task(void *args);

/*###-----������������������������----   Task Configuration Table   ----��������������������������-----###*/

/*״̬������*/
#define STATE_TASK_PRIO              4       // �������ȼ�
#define STATE_STK_SIZE               256     // �����ջ��С
TaskHandle_t STATE_Task_Handler;              // ������
extern void STATE_TASK(void *args);

/*��������*/
#define CHASSIS_TASK_PRIO              5       // �������ȼ�
#define CHASSIS_STK_SIZE               1024     // �����ջ��С
TaskHandle_t CHASSIS_Task_Handler;              // ������
extern void CHASSIS_TASK(void *args);

/*���ʿ�������*/
#define POWER_TASK_PRIO              5       // �������ȼ�
#define POWER_STK_SIZE               256     // �����ջ��С
TaskHandle_t POWER_Task_Handler;              // ������
extern void POWER_TASK(void *args);

/*����ϵͳ����*/
#define REFEREE_TASK_PRIO              6       // �������ȼ�
#define REFEREE_STK_SIZE               256     // �����ջ��С
TaskHandle_t REFEREE_Task_Handler;              // ������
extern void REFEREE_TASK(void *args);

/*###-----������������������������----   Task Configuration Table   ----��������������������������-----###*/



/*  RTOS��ʼ���������������    */
void FreeRTOS_Init(void)
{
	/* ������ʼ������ */
	xTaskCreate((TaskFunction_t		)System_Start_Task,						// ������
							(const char*		)"System_Start_Task",		// ��������
							(uint16_t			)System_START_STK_SIZE,		// �����ջ��С
							(void*				)NULL,					    // ���ݸ��������Ĳ���
							(UBaseType_t		)System_START_TASK_PRIO,	// �������ȼ�
							(TaskHandle_t*		)&SystemStart_Task_Handler);	// ������
	vTaskStartScheduler();	// �����������
}

/**/
void System_Start_Task(void *args)
{
    taskENTER_CRITICAL();//�����ٽ���

	/* ����ϵͳ״̬������ */
	#if IS_STATETASK_ENABLE
	xTaskCreate((TaskFunction_t		)STATE_TASK,						 // ������
							(const char*		)"STATE_TASK",		 // ��������
							(uint16_t			)STATE_STK_SIZE,		 // �����ջ��С
							(void*				)NULL,						 // ���ݸ��������Ĳ���
							(UBaseType_t		)STATE_TASK_PRIO,	     // �������ȼ�
							(TaskHandle_t*		)&STATE_Task_Handler); // ������
	#endif

    /* ������������ */
	#if IS_CHASSISTASK_ENABLE
    xTaskCreate((TaskFunction_t		)CHASSIS_TASK,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 
	#endif


    /* ������������ */
	#if IS_POWERTASK_ENABLE
    xTaskCreate((TaskFunction_t		)POWER_TASK,						 
							(const char*		)"POWER_TASK",		 
							(uint16_t			)POWER_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)POWER_TASK_PRIO,	     
							(TaskHandle_t*		)&POWER_Task_Handler); 
	#endif

    /* ������������ */
	#if IS_REFEREETASK_ENABLE
    xTaskCreate((TaskFunction_t		)REFEREE_TASK,						 
							(const char*		)"REFEREE_TASK",		 
							(uint16_t			)REFEREE_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)REFEREE_TASK_PRIO,	     
							(TaskHandle_t*		)&REFEREE_Task_Handler); 
	#endif

	vTaskDelete(SystemStart_Task_Handler);	//ɾ����ʼ����
	taskEXIT_CRITICAL();	// �˳��ٽ���
}




