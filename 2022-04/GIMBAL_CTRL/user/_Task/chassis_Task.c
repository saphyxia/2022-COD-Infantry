/*状态更新任务
        从地盘获取裁判系统信息，以及底盘反馈
        将更新的底盘信息发送给底盘

*/
#include "chassis_Task.h"



/*--------------------------   变量声明   ------------------------------*/

CHASSIS chassis=
{
	.ctrl =1,
    .state=OFF,
    .mode=FOLO,
    .txMsg=&CAN_TxMsg[_CAN2][_0x300],
    .dr16=&rc_ctrl,
    .state_Setup=chassis_State_Handoff,
	.mode_Setup=chassis_Mode_Handoff,
};

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void CHASSIS_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
//模式控制没加呢！！！
		
//		if(rc_ctrl.rc.ch[4]==660)
//		{
//			chassis.ctrl=1;
//		}else if(rc_ctrl.rc.ch[4]==-660)
//		{
//			chassis.ctrl=2;
//		}
		
		if(Key_SHIFT() == true || Key_C()==true)
		{
			chassis.mode=SPIN;//小陀螺模式
		}
    chassis_Ctrl();

    vTaskDelayUntil(&currentTime, CHASSIS_TASK_TIM);//绝对延时
	}
}

//底盘状态切换(在dbus任务中调用)
static void chassis_State_Handoff(DEVICE_STATE state)
{
    if(state!=chassis.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		chassis.state=state;
    }
}

//底盘模式切换(在dbus任务中调用)
static void chassis_Mode_Handoff(CHASSIS_MODE mode)
{
    if(mode!=chassis.mode){//防止循环调用出错
    //参数重置（受当前模式影响）
		chassis.mode=mode;
    }
}

//底盘控制发送
static void chassis_Ctrl(void)
{
    chassis.vx=rc_ctrl.rc.ch[2];
    chassis.vy=rc_ctrl.rc.ch[3];
    chassis.key=rc_ctrl.key.v;
//发送装载
    chassis.txMsg->Data[0] = (uint8_t)(chassis.ctrl);
    chassis.txMsg->Data[1] = (uint8_t)(chassis.mode);
    chassis.txMsg->Data[2] = (uint8_t)(chassis.vx>>8);
    chassis.txMsg->Data[3] = (uint8_t)(chassis.vx);
    chassis.txMsg->Data[4] = (uint8_t)(chassis.vy>>8);
    chassis.txMsg->Data[5] = (uint8_t)(chassis.vy);
    chassis.txMsg->Data[6] = (uint8_t)(chassis.key>>8);
    chassis.txMsg->Data[7] = (uint8_t)(chassis.key);
}







