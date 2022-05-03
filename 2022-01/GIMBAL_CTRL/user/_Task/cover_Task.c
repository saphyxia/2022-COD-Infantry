/*弹舱盖任务

*/

#include "FreeRTOS_Init.h"
#include "cover_Task.h"
#include "dbus_Task.h"
#include "imu_Task.h"
#include "dbus_Task.h"
#include "vision.h"

/*--------------------------   变量声明   ------------------------------*/

int GIMBAL_YAW_TEST=0,GIMBAL_PIT_TEST=0;
int test=0;
/*--------------------------   变量声明   ------------------------------*/

//调度任务
void COVER_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

//		if(Key_R()==true)
//		{
//			TIM_SetCompare1(TIM1,5500);
//		}else{
//			TIM_SetCompare1(TIM1,1200);
//		}
//		
//		GIMBAL_YAW_TEST = yaw_signed*100.0f;
//		GIMBAL_PIT_TEST = pit_signed*100.0f;

//		myprintf(GIMBAL_PIT_TEST,GIMBAL_YAW_TEST);

		//myprintf((int)(yaw_Angel*100.0f),(int)(RC_Angle[YAW]*100.0f));
		//		myprintf((int)(rol_Angel*100.0f-17871),(int)(-Gimbal[PITCH].Data.angle*100.0f));
		//		myprintf((int)(RC_Angle[PITCH]*100.0f),(int)(Gimbal[PITCH].IMU.angle*100.0f));
		

        vTaskDelayUntil(&currentTime, COVER_TASK_TIM);//绝对延时
	}
}

