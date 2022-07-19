#include "FreeRTOS_Init.h"


static u8  fac_us=0;						//us��ʱ������			   
static u16 fac_ms=0;						//ms��ʱ������,��os��,����ÿ�����ĵ�ms��
 


//��ʼ���ӳٺ���
//SYSTICK ��ʱ�ӹ̶�Ϊ AHB ʱ�ӣ������������� SYSTICK ʱ��Ƶ��Ϊ AHB/8
//����Ϊ�˼��� FreeRTOS�����Խ� SYSTICK ��ʱ��Ƶ�ʸ�Ϊ AHB ��Ƶ�ʣ�
//SYSCLK:ϵͳʱ��Ƶ��
void Delay_init(u8 SYSCLK)
{
	u32 reload;
	//SysTickƵ��ΪHCLK
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload=SYSCLK;							//ÿ���ӵļ������� ��λΪM	   
	reload*=1000000/configTICK_RATE_HZ;		//��configTICK_RATE_HZ�����ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.0998s����	
	fac_ms=1000/configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//����SYSTICK�ж�
	SysTick->LOAD=reload; 					//ÿ1/delay_ostickspersec���ж�һ��	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //����SYSTICK     
}				    

#if SYSTEM_SUPPORT_OS 
//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~204522252(���ֵ��2^32/fac_us@fac_us=168)	    	ffa							   
void delay_us(u32 nus)
{ 
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;  //LOAD ��ֵ
	ticks=nus*fac_us; //��Ҫ�Ľ�����
	told=SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL; 
		if(tnow!=told)
		{ 
			//����ע��һ�� SYSTICK ��һ���ݼ��ļ������Ϳ�����.
			if(tnow<told)tcnt+=told-tnow; 
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;  //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	}; 
}//��ʱnms
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(u32 nms)
{ 
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{ 
		if(nms>=fac_ms) //��ʱ��ʱ����� OS ������ʱ������
		{
			vTaskDelay(nms/fac_ms);  //FreeRTOS ��ʱ
		}
		nms%=fac_ms;  //OS �Ѿ��޷��ṩ��ôС����ʱ��,
		//������ͨ��ʽ��ʱ
	}
	delay_us((u32)(nms*1000)); //��ͨ��ʽ��ʱ
}
//��ʱ nms,���������������
//nms:Ҫ��ʱ�� ms ��
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/*
    FreeRTOS       _Task
*/
//start_Task
#define System_START_TASK_PRIO		1			// �������ȼ�
#define System_START_STK_SIZE		128			// �����ջ��С
TaskHandle_t SystemStart_Task_Handler;			// ������
extern void System_Start_Task(void *args);

/*###-----������������������������----   Task Configuration Table   ----��������������������������-----###*/

/*״̬������*/
#define SYSTEMSTATE_TASK_PRIO              1       // �������ȼ�
#define SYSTEMSTATE_STK_SIZE               256     // �����ջ��С
TaskHandle_t SYSTEMSTATE_Task_Handler;              // ������
extern void SYSTEMSTATE_TASK(void *args);

/*��̨������*/
#define GIMBAL_TASK_PRIO              5       // �������ȼ�
#define GIMBAL_STK_SIZE               512     // �����ջ��С
TaskHandle_t GIMBAL_Task_Handler;              // ������
extern void GIMBAL_TASK(void *args);

/*��������*/
#define CHASSIS_TASK_PRIO              5       // �������ȼ�
#define CHASSIS_STK_SIZE               1024     // �����ջ��С
TaskHandle_t CHASSIS_Task_Handler;              // ������
extern void CHASSIS_TASK(void *args);

/*ң�����źŴ�������*/
#define DBUS_TASK_PRIO              6       // �������ȼ�
#define DBUS_STK_SIZE               256     // �����ջ��С
TaskHandle_t DBUS_Task_Handler;              // ������
extern void DBUS_TASK(void *args);

/*���������ݴ�������*/
#define IMU_TASK_PRIO              6       // �������ȼ�
#define IMU_STK_SIZE               1024    // �����ջ��С
TaskHandle_t IMU_Task_Handler;              // ������
extern void IMU_TASK(void *args);

/*�Ӿ����ݴ�������*/
#define VISION_TASK_PRIO              6       // �������ȼ�
#define VISION_STK_SIZE               512     // �����ջ��С
TaskHandle_t VISION_Task_Handler;              // ������
extern void VISION_TASK(void *args);

/*��������*/
#define SHOOT_TASK_PRIO              4       // �������ȼ�
#define SHOOT_STK_SIZE               256     // �����ջ��С
TaskHandle_t SHOOT_Task_Handler;              // ������
extern void SHOOT_TASK(void *args);

/*���ո�����*/
#define COVER_TASK_PRIO              4       // �������ȼ�
#define COVER_STK_SIZE               256     // �����ջ��С
TaskHandle_t COVER_Task_Handler;              // ������
extern void COVER_TASK(void *args);

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

    /* ����ң������������ */
    xTaskCreate((TaskFunction_t		)DBUS_TASK,						 
							(const char*		)"DBUS_TASK",		 
							(uint16_t			)DBUS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)DBUS_TASK_PRIO,	     
							(TaskHandle_t*		)&DBUS_Task_Handler); 

	
    /* ����ϵͳ״̬������ */
	xTaskCreate((TaskFunction_t		)SYSTEMSTATE_TASK,						 // ������
							(const char*		)"SYSTEMSTATE_TASK",		 // ��������
							(uint16_t			)SYSTEMSTATE_STK_SIZE,		 // �����ջ��С
							(void*				)NULL,						 // ���ݸ��������Ĳ���
							(UBaseType_t		)SYSTEMSTATE_TASK_PRIO,	     // �������ȼ�
							(TaskHandle_t*		)&SYSTEMSTATE_Task_Handler); // ������

    /* ������̨���� */
    xTaskCreate((TaskFunction_t		)GIMBAL_TASK,						 
							(const char*		)"GIMBAL_TASK",		 
							(uint16_t			)GIMBAL_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)GIMBAL_TASK_PRIO,	     
							(TaskHandle_t*		)&GIMBAL_Task_Handler); 

    /* ������̨״̬�������� */
    xTaskCreate((TaskFunction_t		)CHASSIS_TASK,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 

    /* �������������ݴ������� */
    xTaskCreate((TaskFunction_t		)IMU_TASK,						 
							(const char*		)"IMU_TASK",		 
							(uint16_t			)IMU_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)IMU_TASK_PRIO,	     
							(TaskHandle_t*		)&IMU_Task_Handler); 

    /* �����Ӿ����ݴ������� */
    xTaskCreate((TaskFunction_t		)VISION_TASK,						 
							(const char*		)"VISION_TASK",		 
							(uint16_t			)VISION_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)VISION_TASK_PRIO,	     
							(TaskHandle_t*		)&VISION_Task_Handler); 


    /* ������������ */
    xTaskCreate((TaskFunction_t		)SHOOT_TASK,						 
							(const char*		)"SHOOT_TASK",		 
							(uint16_t			)SHOOT_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)SHOOT_TASK_PRIO,	     
							(TaskHandle_t*		)&SHOOT_Task_Handler); 

    /* �������ո����� */
    xTaskCreate((TaskFunction_t		)COVER_TASK,						 
							(const char*		)"COVER_TASK",		 
							(uint16_t			)COVER_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)COVER_TASK_PRIO,	     
							(TaskHandle_t*		)&COVER_Task_Handler);  
							
	vTaskDelete(SystemStart_Task_Handler);	//ɾ����ʼ����
	taskEXIT_CRITICAL();	// �˳��ٽ���
}

			 
#endif



