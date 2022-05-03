#include "main.h"
#include "stm32f4xx.h"
#include "delay.h"
#include "ina219.h"
#include "Remote_Control.h"
#include "can.h"
#include "usart.h"
#include "myiic.h"
#include "usart.h"
#include "iwdg.h"
#include "CAN_Receive.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "io_init.h"
#include "start_task.h"
#include "spi.h"
#include "imu.h"


void BSP_init(void);

int main(void)
{
		BSP_init();
    delay_ms(100);
    startTast();
    vTaskStartScheduler();
}

void BSP_init(void)
{
	//中断组 4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//初始化滴答时钟
	delay_init(configTICK_RATE_HZ);

	power_ctrl_configuration();
	for (uint8_t i = POWER1_CTRL_SWITCH; i < POWER4_CTRL_SWITCH + 1; i++)
	{
		power_ctrl_on(i);
		delay_us(709);
	}
	
	remote_control_init();//遥控器初始化
//	CAN1_Init();//CAN1初始化
//	CAN2_Init();//CAN2初始化
	Motor_PID_Param_Init();//PID初始化
//	USART6_Configuration(115200);//串口6初始化
	UART7_Configuration(115200);
	SPI5_Init();
	SPI5_SetSpeed(SPI_BaudRatePrescaler_128);
	MPU6500_Init();
}


