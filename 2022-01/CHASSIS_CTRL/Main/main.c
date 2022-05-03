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
	//�ж��� 4
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	//��ʼ���δ�ʱ��
	delay_init(configTICK_RATE_HZ);

	power_ctrl_configuration();
	for (uint8_t i = POWER1_CTRL_SWITCH; i < POWER4_CTRL_SWITCH + 1; i++)
	{
		power_ctrl_on(i);
		delay_us(709);
	}
	
	remote_control_init();//ң������ʼ��
//	CAN1_Init();//CAN1��ʼ��
//	CAN2_Init();//CAN2��ʼ��
	Motor_PID_Param_Init();//PID��ʼ��
//	USART6_Configuration(115200);//����6��ʼ��
	UART7_Configuration(115200);
	SPI5_Init();
	SPI5_SetSpeed(SPI_BaudRatePrescaler_128);
	MPU6500_Init();
}


