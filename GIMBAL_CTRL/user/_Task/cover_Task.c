/*弹舱盖任务

*/
#include "cover_Task.h"
#include "gpio.h"
#include "rc.h"

/*--------------------------   变量声明   ------------------------------*/
bool IF_COVER_OPEN = false;

uint16_t TEST_COVER_PWM;
/*--------------------------   变量声明   ------------------------------*/

//调度任务
void COVER_TASK(void *args)
{
    uint32_t currentTime;

	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

		if(Key_R() == false)
		{
			TIM_SetCompare1(TIM1,1150);
			IF_COVER_OPEN = false;
			
		}else if(Key_R() == true)
		{
			TIM_SetCompare1(TIM1,3615);
			IF_COVER_OPEN = true;
		}
		
    vTaskDelayUntil(&currentTime, COVER_TASK_TIM);//绝对延时
	}
}

