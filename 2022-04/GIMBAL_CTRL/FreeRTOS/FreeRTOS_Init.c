#include "FreeRTOS_Init.h"


static u8  fac_us=0;						//us延时倍乘数			   
static u16 fac_ms=0;						//ms延时倍乘数,在os下,代表每个节拍的ms数
 


//初始化延迟函数
//SYSTICK 的时钟固定为 AHB 时钟，基础例程里面 SYSTICK 时钟频率为 AHB/8
//这里为了兼容 FreeRTOS，所以将 SYSTICK 的时钟频率改为 AHB 的频率！
//SYSCLK:系统时钟频率
void Delay_init(u8 SYSCLK)
{
	u32 reload;
	//SysTick频率为HCLK
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	fac_us=SYSCLK;							//不论是否使用OS,fac_us都需要使用
	reload=SYSCLK;							//每秒钟的计数次数 单位为M	   
	reload*=1000000/configTICK_RATE_HZ;		//根configTICK_RATE_HZ定溢出时间
											//reload为24位寄存器,最大值:16777216,在168M下,约合0.0998s左右	
	fac_ms=1000/configTICK_RATE_HZ;			//代表OS可以延时的最少单位	   
	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;//开启SYSTICK中断
	SysTick->LOAD=reload; 					//每1/delay_ostickspersec秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk; //开启SYSTICK     
}				    

#if SYSTEM_SUPPORT_OS 
//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	    	ffa							   
void delay_us(u32 nus)
{ 
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;  //LOAD 的值
	ticks=nus*fac_us; //需要的节拍数
	told=SysTick->VAL; //刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL; 
		if(tnow!=told)
		{ 
			//这里注意一下 SYSTICK 是一个递减的计数器就可以了.
			if(tnow<told)tcnt+=told-tnow; 
			else tcnt+=reload-tnow+told;
			told=tnow;
			if(tcnt>=ticks)break;  //时间超过/等于要延迟的时间,则退出.
		}
	}; 
}//延时nms
//nms:要延时的ms数
//nms:0~65535
void delay_ms(u32 nms)
{ 
	if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
	{ 
		if(nms>=fac_ms) //延时的时间大于 OS 的最少时间周期
		{
			vTaskDelay(nms/fac_ms);  //FreeRTOS 延时
		}
		nms%=fac_ms;  //OS 已经无法提供这么小的延时了,
		//采用普通方式延时
	}
	delay_us((u32)(nms*1000)); //普通方式延时
}
//延时 nms,不会引起任务调度
//nms:要延时的 ms 数
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}

/*
    FreeRTOS       _Task
*/
//start_Task
#define System_START_TASK_PRIO		1			// 任务优先级
#define System_START_STK_SIZE		128			// 任务堆栈大小
TaskHandle_t SystemStart_Task_Handler;			// 任务句柄
extern void System_Start_Task(void *args);

/*###-----↓↓↓↓↓↓↓↓↓↓↓↓----   Task Configuration Table   ----↓↓↓↓↓↓↓↓↓↓↓↓↓-----###*/

/*状态机任务*/
#define SYSTEMSTATE_TASK_PRIO              1       // 任务优先级
#define SYSTEMSTATE_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t SYSTEMSTATE_Task_Handler;              // 任务句柄
extern void SYSTEMSTATE_TASK(void *args);

/*云台总任务*/
#define GIMBAL_TASK_PRIO              5       // 任务优先级
#define GIMBAL_STK_SIZE               512     // 任务堆栈大小
TaskHandle_t GIMBAL_Task_Handler;              // 任务句柄
extern void GIMBAL_TASK(void *args);

/*底盘任务*/
#define CHASSIS_TASK_PRIO              5       // 任务优先级
#define CHASSIS_STK_SIZE               1024     // 任务堆栈大小
TaskHandle_t CHASSIS_Task_Handler;              // 任务句柄
extern void CHASSIS_TASK(void *args);

/*遥控器信号处理任务*/
#define DBUS_TASK_PRIO              6       // 任务优先级
#define DBUS_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t DBUS_Task_Handler;              // 任务句柄
extern void DBUS_TASK(void *args);

/*陀螺仪数据处理任务*/
#define IMU_TASK_PRIO              6       // 任务优先级
#define IMU_STK_SIZE               1024    // 任务堆栈大小
TaskHandle_t IMU_Task_Handler;              // 任务句柄
extern void IMU_TASK(void *args);

/*视觉数据处理任务*/
#define VISION_TASK_PRIO              6       // 任务优先级
#define VISION_STK_SIZE               512     // 任务堆栈大小
TaskHandle_t VISION_Task_Handler;              // 任务句柄
extern void VISION_TASK(void *args);

/*发射任务*/
#define SHOOT_TASK_PRIO              4       // 任务优先级
#define SHOOT_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t SHOOT_Task_Handler;              // 任务句柄
extern void SHOOT_TASK(void *args);

/*弹舱盖任务*/
#define COVER_TASK_PRIO              4       // 任务优先级
#define COVER_STK_SIZE               256     // 任务堆栈大小
TaskHandle_t COVER_Task_Handler;              // 任务句柄
extern void COVER_TASK(void *args);

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

    /* 创建遥控器接收任务 */
    xTaskCreate((TaskFunction_t		)DBUS_TASK,						 
							(const char*		)"DBUS_TASK",		 
							(uint16_t			)DBUS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)DBUS_TASK_PRIO,	     
							(TaskHandle_t*		)&DBUS_Task_Handler); 

	
    /* 创建系统状态机任务 */
	xTaskCreate((TaskFunction_t		)SYSTEMSTATE_TASK,						 // 任务函数
							(const char*		)"SYSTEMSTATE_TASK",		 // 任务名称
							(uint16_t			)SYSTEMSTATE_STK_SIZE,		 // 任务堆栈大小
							(void*				)NULL,						 // 传递给任务函数的参数
							(UBaseType_t		)SYSTEMSTATE_TASK_PRIO,	     // 任务优先级
							(TaskHandle_t*		)&SYSTEMSTATE_Task_Handler); // 任务句柄

    /* 创建云台任务 */
    xTaskCreate((TaskFunction_t		)GIMBAL_TASK,						 
							(const char*		)"GIMBAL_TASK",		 
							(uint16_t			)GIMBAL_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)GIMBAL_TASK_PRIO,	     
							(TaskHandle_t*		)&GIMBAL_Task_Handler); 

    /* 创建云台状态更新任务 */
    xTaskCreate((TaskFunction_t		)CHASSIS_TASK,						 
							(const char*		)"CHASSIS_TASK",		 
							(uint16_t			)CHASSIS_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)CHASSIS_TASK_PRIO,	     
							(TaskHandle_t*		)&CHASSIS_Task_Handler); 

    /* 创建陀螺仪数据处理任务 */
    xTaskCreate((TaskFunction_t		)IMU_TASK,						 
							(const char*		)"IMU_TASK",		 
							(uint16_t			)IMU_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)IMU_TASK_PRIO,	     
							(TaskHandle_t*		)&IMU_Task_Handler); 

    /* 创建视觉数据处理任务 */
    xTaskCreate((TaskFunction_t		)VISION_TASK,						 
							(const char*		)"VISION_TASK",		 
							(uint16_t			)VISION_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)VISION_TASK_PRIO,	     
							(TaskHandle_t*		)&VISION_Task_Handler); 


    /* 创建发射任务 */
    xTaskCreate((TaskFunction_t		)SHOOT_TASK,						 
							(const char*		)"SHOOT_TASK",		 
							(uint16_t			)SHOOT_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)SHOOT_TASK_PRIO,	     
							(TaskHandle_t*		)&SHOOT_Task_Handler); 

    /* 创建弹舱盖任务 */
    xTaskCreate((TaskFunction_t		)COVER_TASK,						 
							(const char*		)"COVER_TASK",		 
							(uint16_t			)COVER_STK_SIZE,		 
							(void*				)NULL,						 
							(UBaseType_t		)COVER_TASK_PRIO,	     
							(TaskHandle_t*		)&COVER_Task_Handler);  
							
	vTaskDelete(SystemStart_Task_Handler);	//删除开始任务
	taskEXIT_CRITICAL();	// 退出临界区
}

			 
#endif



