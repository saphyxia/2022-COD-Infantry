#include "State_Task.h"
#include "FreeRTOS_Init.h"
#include "iwdg.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Chassis_Task.h"
#include "imu.h"
#include "usart.h"
#include "chassis.h"
#include "gpio.h"

void STATE_TASK(void *pvParameters)
{
  uint32_t currentTime;
	while(1)
	{
    currentTime = xTaskGetTickCount();//当前系统时间
		//底盘加速度
		IMU_getYawPitchRoll(angle);
		CAN_TxMsg[_CAN2][_0x302].Data[5] = (uint8_t)((int16_t)(MPU6050_Real_Data.Gyro_Z*100) >> 8);
		CAN_TxMsg[_CAN2][_0x302].Data[6] = (uint8_t)((int16_t)(MPU6050_Real_Data.Gyro_Z*100));
				
    vTaskDelayUntil(&currentTime,STATE_TASK_TIM);//绝对延时
	}
}


/**
 *	@brief	数据NAN判断
 */
bool Judge_IF_NAN(float x)
{
  bool res = true;

  res = (bool)__ARM_isnan(x);

  return res;
}

void Remote_Protect(void)
{
	#if IF_DR16_CONNECT
		memset(&rc_ctrl.rc.ch,0,sizeof(rc_ctrl.rc.ch));
		memset(&rc_ctrl.rc.s,0,sizeof(rc_ctrl.rc.s));
		rc_ctrl.mouse.x = 0;
		rc_ctrl.mouse.y = 0;
		rc_ctrl.mouse.z = 0;
		rc_ctrl.mouse.press_l = 0;
		rc_ctrl.mouse.press_r = 0;
		rc_ctrl.key.v = 0;
  #else
		Chassis.ctrl = Ctrl_Err;
		Chassis.act = Act_Err;
		Chassis.CH2 = 0;
		Chassis.CH3 = 0;
		Chassis.key.V = 0;
	#endif
}
