/*接收机机任务

*/

#include "FreeRTOS_Init.h"
#include "dbus_Task.h"
#include "can1.h"
#include "rc.h"
#include "imu_Task.h"
#include "assist.h"

/*--------------------------   变量声明   ------------------------------*/

float RC_Angle[GIMBAL_MOTOR_NUM]={0,1.8f};
int yaw_bap=1,pitch_bap=1;
/*--------------------------   变量声明   ------------------------------*/
float yaw_cps,Last_RC_Angle;
float temp_yaw=8;

float mouse_x_k=0.f,mouse_y_k=0.f;
//调度任务
void DBUS_TASK(void *args)
{
    uint32_t currentTime;

	while(1)
	{
		currentTime = xTaskGetTickCount();	

		if(rc_ctrl.rc.s[0]==3){
				RC_Angle[YAW  ] += rc_ctrl.rc.ch[0] * 0.0006f;
				RC_Angle[PITCH] += rc_ctrl.rc.ch[1] * 0.0005f;
		}
		else if(rc_ctrl.rc.s[0]==2)
		{
				RC_Angle[YAW  ] -= Mouse_X_Speed() * 0.0035f;
				RC_Angle[PITCH] -= (Key_CTRL()==false) * Mouse_Y_Speed() * 0.006f;
				Key_Q();
				Key_E();
		}
		else{
				RC_Angle[YAW  ] = yaw_Angel;
				RC_Angle[PITCH] = pit_Angel;
		}

//      yaw_cps=( RC_Angle[YAW  ]-Last_RC_Angle)*1000.0f;
		yaw_cps=ramp_Float(yaw_cps,( RC_Angle[YAW  ]-Last_RC_Angle)*1000.0f,temp_yaw);
		Last_RC_Angle=RC_Angle[YAW  ];
		VAL_LIMIT(RC_Angle[PITCH],-26.f,35.f);
				
		currentTime = xTaskGetTickCount();//当前系统时间
    
		vTaskDelayUntil(&currentTime, DBUS_TASK_TIM);
	}
}

