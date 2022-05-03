/*弹舱盖任务

*/
#include "cover_Task.h"
#include "gpio.h"
COVER cover=
{
    .state=OFF,
    .state_Setup=cover_State_Handoff,
};

/*--------------------------   变量声明   ------------------------------*/


/*--------------------------   变量声明   ------------------------------*/

//调度任务
void COVER_TASK(void *args)
{
    uint32_t currentTime;

	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

    cover_Ctrl();		

    vTaskDelayUntil(&currentTime, COVER_TASK_TIM);//绝对延时
	}
}

//弹舱盖状态设置(在dbus任务中调用)
static void cover_State_Handoff(DEVICE_STATE state)
{
    if(state!=cover.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		cover.state=state;
    }
}

static void cover_Ctrl(void)
{
    switch (cover.state)
    {
    case ON:
        TIM_SetCompare1(TIM1,980);
        break;
    case OFF:
        TIM_SetCompare1(TIM1,3615);
        break;

    default:
        break;
    }
}


