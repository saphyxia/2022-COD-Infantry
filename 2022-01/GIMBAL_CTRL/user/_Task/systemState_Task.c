/*状态机任务

*/


#include "FreeRTOS_Init.h"
#include "systemState_Task.h"


/*--------------------------   变量声明   ------------------------------*/

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void SYSTEMSTATE_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间


        vTaskDelayUntil(&currentTime, SYSTEMSTATE_TASK_TIM);//绝对延时
	}
}

