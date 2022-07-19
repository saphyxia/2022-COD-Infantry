#include "ImuTask.h"
#include "imu.h"
#include "can.h"
/**
 *	@brief	状态机任务
*/
void ImuTask(void *pvParameters) 
{
	TickType_t systick;
	
	while(1)
	{
        systick = xTaskGetTickCount();//心跳计数
		//底盘加速度
		IMU_getYawPitchRoll(angle);
		CAN_TxMsg[_CAN2][_0x302].Data[5] = (uint8_t)((int16_t)(MPU6050_Real_Data.Gyro_Z*100) >> 8);
		CAN_TxMsg[_CAN2][_0x302].Data[6] = (uint8_t)((int16_t)(MPU6050_Real_Data.Gyro_Z*100));
		
        vTaskDelayUntil(&systick,IMUTASK_TIM);
	}
}

