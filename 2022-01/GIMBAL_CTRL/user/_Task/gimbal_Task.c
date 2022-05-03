#include "FreeRTOS_Init.h"
#include "gimbal_Task.h"
#include "imu_Task.h"
#include "can1.h"
#include "vision_Task.h"

/*--------------------------   变量声明   ------------------------------*/

float Gimbal_PID_Err[GIMBAL_MOTOR_NUM][GIMBAL_STATE_NUM];
extern float RC_Angle[GIMBAL_MOTOR_NUM];
/*--------------------------   变量声明   ------------------------------*/
extKalman_t  Gimbal_PIT_RC;
extKalman_t  Gimbal_YAW_RC;

extKalman_t AI_YAW_KB,AI_PITCH_KB;

float yaw_err=0.f,pit_err=0.f;

bool IF_VISION_ENABLE = false ;

//调度任务
void GIMBAL_TASK(void *args)
{
    Gimbal_Motor_PID_Param_Init();
    uint32_t currentTime;
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
		
		
		if(IF_VISION_ENABLE == true)
		{
			Gimbal[YAW  ].state=AI_STAN;
			Gimbal[PITCH].state=AI_STAN;
			yaw_err = KalmanFilter(&AI_YAW_KB,yaw_signed);
			pit_err = KalmanFilter(&AI_PITCH_KB,pit_signed);
			
			CAN_Send_Msg(CAN1,0x1ff,0,Gimbal[PITCH].ctrl_Calc(pit_err),0);
			CAN_Send_Msg(CAN2,0x1ff,Gimbal[YAW  ].ctrl_Calc(yaw_err),0,0);
		}
		else{
				if(rc_ctrl.rc.s[0]==1){//卸力
						Gimbal[YAW  ].state=GTURN_OFF;
						Gimbal[PITCH].state=GTURN_OFF;
				}
				else if((rc_ctrl.rc.s[0]==3)&&(IMU_Working==1)){
						Gimbal[YAW  ].state=RC_GYRO;
						Gimbal[PITCH].state=RC_GYRO;
				}
				
				//误差更新，注意只会涉及到RC开头的类别。包括：RC_GYRO,RC_MECH两种。千万别写别的出来
				Gimbal_PID_Err[YAW  ][RC_GYRO]=RC_Angle[YAW  ] - yaw_Angel;
				Gimbal_PID_Err[PITCH][RC_GYRO]=RC_Angle[PITCH] - pit_Angel;

				Gimbal_PID_Err[PITCH][RC_GYRO]=KalmanFilter(&Gimbal_PIT_RC,Gimbal_PID_Err[PITCH][RC_GYRO]);
				Gimbal_PID_Err[YAW  ][RC_GYRO]=KalmanFilter(&Gimbal_YAW_RC,Gimbal_PID_Err[YAW  ][RC_GYRO]);
				// Gimbal[YAW  ].ctrl_Calc(Gimbal_PID_Err[YAW  ][Gimbal[YAW  ].state]);
				// Gimbal[PITCH].ctrl_Calc(Gimbal_PID_Err[PITCH][Gimbal[PITCH].state]);
				CAN_Send_Msg(CAN1,0x1ff,0,Gimbal[PITCH].ctrl_Calc(Gimbal_PID_Err[PITCH][Gimbal[PITCH].state]),0);
				CAN_Send_Msg(CAN2,0x1ff,Gimbal[YAW  ].ctrl_Calc(Gimbal_PID_Err[YAW  ][Gimbal[YAW  ].state]),0,0);
	}
    vTaskDelayUntil(&currentTime, GIMBAL_TASK_TIM);//绝对延时
	}
}




