#include "main.h"
#include "system.h"
#include "FreeRTOS_Init.h"
#include "gimbal_Task.h"
#include "vision_Task.h"
#include "uart1.h"
#include "can1.h"
#include "can2.h"
#include "rc.h"

//初始化驱动
void Bsp_Init(void)
{
//	gpio_Init();
//	TIM1_PWM_Init();
//	uart1_Init(460800);
//	uart2_Init(115200);
	remote_control_init();

	CAN1_Init();
//	CAN2_Init();
	SPI1_Init();
	IIC_Init();
}

//初始化设备
void Device_Init(void)
{
	 
}

//初始化算法参数
void Algorithms_Init(void)
{
		KalmanCreate(&Gimbal_PIT_RC,1,30);
		KalmanCreate(&Gimbal_YAW_RC,1,30);

		KalmanCreate(&KF_Mouse_X_Speed,1,60);
    KalmanCreate(&KF_Mouse_Y_Speed,1,60);

		KalmanCreate(&KF_Vision[YAW  ],1,30);
    KalmanCreate(&KF_Vision[PITCH],1,30);
	
    KalmanCreate(&KF_Vision_Forecast[YAW  ],1,30);
    KalmanCreate(&KF_Vision_Forecast[PITCH],1,30);
		
		KalmanCreate(&AI_YAW_KB,1,30);
		KalmanCreate(&AI_PITCH_KB,1,30);
	
    /*自瞄卡尔曼滤波,二阶*/
//	mat_init(&KF_Filter_Vision_Gimbal[YAW  ].Q,2,2, KF_Filter_Param_Vision_Gimbal[YAW  ].Q_data);
//	mat_init(&KF_Filter_Vision_Gimbal[YAW  ].R,2,2, KF_Filter_Param_Vision_Gimbal[YAW  ].R_data);
//	kalman_filter_init(&KF_Filter_Vision_Gimbal[YAW  ], &KF_Filter_Param_Vision_Gimbal[YAW  ]);
//	
//	mat_init(&KF_Filter_Vision_Gimbal[PITCH].Q,2,2, KF_Filter_Param_Vision_Gimbal[PITCH].Q_data);
//	mat_init(&KF_Filter_Vision_Gimbal[PITCH].R,2,2, KF_Filter_Param_Vision_Gimbal[PITCH].R_data);
//	kalman_filter_init(&KF_Filter_Vision_Gimbal[PITCH], &KF_Filter_Param_Vision_Gimbal[PITCH]);
}

//云台系统总初始化调用
void Gimbal_System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	Delay_init(168);//1ms Systick
	Bsp_Init();
//	Device_Init();
//	Algorithms_Init();

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




