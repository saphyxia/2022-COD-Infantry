/*状态更新任务
        从地盘获取裁判系统信息，以及底盘反馈
        将更新的底盘信息发送给底盘

*/
#include "FreeRTOS_Init.h"
#include "state_Task.h"
#include "rc.h"
#include "can2.h"
#include "imu_Task.h"

/*--------------------------   变量声明   ------------------------------*/
CHASSIS_Info chas_Ctrl;
uint8_t RC_Info_Buf[8];

/*--------------------------   变量声明   ------------------------------*/

void STATE_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

		chas_Ctrl.vx=rc_ctrl.rc.ch[2];
		chas_Ctrl.vy=rc_ctrl.rc.ch[3];
		chas_Ctrl.key=rc_ctrl.key.v;
		
		if(rc_ctrl.rc.s[0]==2)
		{
			chas_Ctrl.ctrl_Flag=2;	//1;	
		}else if(rc_ctrl.rc.s[0]==3)
		{
			chas_Ctrl.ctrl_Flag=1;
		}
		
		if(rc_ctrl.rc.s[1]==3)
		{
			chas_Ctrl.mode_Flag=INVA;//卸力模式
		}
		else if(rc_ctrl.rc.s[1]==1)
		{
			chas_Ctrl.mode_Flag=FOLO;//底盘跟随云台
		}
		else if(rc_ctrl.rc.s[1]==2)
		{
			chas_Ctrl.mode_Flag=SPIN;//小陀螺模式
		}
		if(Key_SHIFT() == true)
		{
			chas_Ctrl.mode_Flag=SPIN;//小陀螺模式
		}

		RC_Info_SendtoChassis();//将更新的底盘信息发送给底盘
		vTaskDelayUntil(&currentTime, STATE_TASK_TIM);//绝对延时
	}
}

void RC_Info_SendtoChassis(void)
{
    RC_Info_Buf[0] = (uint8_t)(chas_Ctrl.ctrl_Flag);
    RC_Info_Buf[1] = (uint8_t)(chas_Ctrl.mode_Flag);
    RC_Info_Buf[2] = (uint8_t)(chas_Ctrl.vx>>8);
    RC_Info_Buf[3] = (uint8_t)(chas_Ctrl.vx);
    RC_Info_Buf[4] = (uint8_t)(chas_Ctrl.vy>>8);
    RC_Info_Buf[5] = (uint8_t)(chas_Ctrl.vy);
    RC_Info_Buf[6] = (uint8_t)(chas_Ctrl.key>>8);
    RC_Info_Buf[7] = (uint8_t)(chas_Ctrl.key);
}



