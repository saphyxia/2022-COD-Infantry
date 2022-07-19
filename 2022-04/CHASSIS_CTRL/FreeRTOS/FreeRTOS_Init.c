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

/*��������*/
#define INTERAC_TASK_PRIO              4       // �������ȼ�
#define INTERAC_STK_SIZE               256     // �����ջ��С
TaskHandle_t INTERAC_Task_Handler;              // ������
extern void InteracTask(void *args);

/*��������*/
#define CHASSIS_TASK_PRIO              4       // �������ȼ�
#define CHASSIS_STK_SIZE               1024     // �����ջ��С
TaskHandle_t CHASSIS_Task_Handler;              // ������
extern void ChassisTask(void *args);

/*����������*/
#define IMU_TASK_PRIO              4       // �������ȼ�
#define IMU_STK_SIZE               256     // �����ջ��С
TaskHandle_t IMU_Task_Handler;              // ������
extern void ImuTask(void *args);

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

	/* ����״̬������ */
	#if IS_IMUTASK_ENABLE
	xTaskCreate((TaskFunction_t		)ImuTask,						 // ������
							(const char*		)"IMU_TASK",		 // ��������
							(uint16_t			)IMU_STK_SIZE,		 // �����ջ��С
							(void*				)NULL,						 // ���ݸ��������Ĳ���
							(UBaseType_t		)IMU_TASK_PRIO,	     // �������ȼ�
							(TaskHandle_t*		)&IMU_Task_Handler); // ������
	#endif

    /* ������������ */
	#if IS_CHASSISTASK_ENABLE
    xTaskCreate((TaskFunction_t		)ChassisTask,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 
	#endif


    /* ������������ */
	#if IS_INTERACTASK_ENABLE
    xTaskCreate((TaskFunction_t		)InteracTask,						 
							(const char*		)"INTERAC_TASK",		 
							(uint16_t			)INTERAC_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)INTERAC_TASK_PRIO,	     
							(TaskHandle_t*		)&INTERAC_Task_Handler); 
	#endif

	vTaskDelete(SystemStart_Task_Handler);	//ɾ����ʼ����
	taskEXIT_CRITICAL();	// �˳��ٽ���
}




