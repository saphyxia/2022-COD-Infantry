#include "main.h"
#include "system.h"
#include "FreeRTOS_Init.h"
#include "gimbal_Task.h"
#include "shoot_Task.h"
#include "cover_Task.h"
#include "chassis_Task.h"
#include "vision_Task.h"
#include "uart.h"
#include "can.h"
#include "rc.h"

RCC_ClocksTypeDef sys_clock;

//初始化驱动
void Bsp_Init(void)
{
	gpio_Init();
	SPI1_Init();
//	IIC_Init();
	uart1_Init(460800);
	uart2_Init(115200);
	remote_control_init();
	Beep_Init();

//  TIM1_PWM_Init();//弹舱盖舵机

	CAN1_Init();
	CAN2_Init();
}

//初始化设备
void Device_Init(void)
{
	gimbal.state_Setup (CALIBRATING);
	shoot.state_Setup  (CALIBRATING);
	cover.state_Setup  (CALIBRATING);
	chassis.state_Setup(CALIBRATING);
}

//初始化算法参数
void Algorithms_Init(void)
{
	KalmanCreate(&KF_Mouse_X_Speed,1,60);
    KalmanCreate(&KF_Mouse_Y_Speed,1,60);
}

//云台系统总初始化调用
void Gimbal_System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
    Delay_init(168);//1ms Systick
    Bsp_Init();
    Device_Init();
    Algorithms_Init();
	RCC_GetClocksFreq(&sys_clock);
    FreeRTOS_Init();
}

int main(void)
{
 	Gimbal_System_Init();
	while(1)
	{
		;
	}
}




