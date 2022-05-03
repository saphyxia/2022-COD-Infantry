#include "imu_Task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "imu.h"


int imu_flag;
void imu_task(void *pvParameters)
{
  uint32_t currentTime;

//	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{
//        imu_flag++;
				IMU_getYawPitchRoll( angle );
    vTaskDelayUntil(&currentTime,TIME_STAMP_1MS);

	}
}


