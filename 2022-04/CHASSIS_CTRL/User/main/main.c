#include "main.h"
#include "FreeRTOS_Init.h"
#include "task.h"
#include "can.h"
#include "rc.h"
#include "delay.h"
#include "gpio.h"
#include "myiic.h"
#include "referee.h"
#include "imu.h"
#include "spi.h"
#include "usart.h"

//初始化驱动
void Bsp_Init(void)
{
	gpio_Init();
	IIC_Init();
	remote_control_init();

	Referee_Init();
	CAN1_Init();
	CAN2_Init();
	uart7_Init(115200);
	uart3_Init(115200);
	SPI5_Init();
	SPI5_SetSpeed(SPI_BaudRatePrescaler_128);
	MPU6500_Init();
}

//底盘系统总初始化调用
void Chassis_System_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(configTICK_RATE_HZ);
	Bsp_Init();
	FreeRTOS_Init();
}

int main(void)
{	

	Chassis_System_Init();
	vTaskStartScheduler();
	while(1){;}
}




