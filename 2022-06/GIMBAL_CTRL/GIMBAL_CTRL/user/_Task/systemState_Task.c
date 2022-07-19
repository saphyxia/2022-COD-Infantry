/*状态机任务

*/


#include "systemState_Task.h"
#include "gimbal_Task.h"
#include "vision_Task.h"
#include "shoot_Task.h"

/*--------------------------   变量声明   ------------------------------*/

ROBOT robot=
{
    .mode=INITIAL,
    .level=LV_0,
};
/*--------------------------   变量声明   ------------------------------*/
//调度任务
extern float k2;
float aaa=1;
extern float yaw_Predict;
//装甲板位置
extern float armor_position2vision[2];
//装甲板速度
extern float armor_velocity2vision[2];
//预测量0y1p
extern float predictValue[2];
void SYSTEMSTATE_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

        myprintf(   (int)(-shoot.wheel_L->Data.velocity),
                    (int)(shoot.wheel_R->Data.velocity),
                    (int)(100)
                );
        
        vTaskDelayUntil(&currentTime, SYSTEMSTATE_TASK_TIM);//绝对延时
	}
}

