#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H
#include "FreeRTOS_Init.h"
#include "systemState_Task.h"
#include "vision_Task.h"
#include "bmi088.h"
#include "vision.h"
#include "motor.h"
#include "rc.h"
#include "pid.h"
#include "filter.h"

#define GIMBAL_USE_IMU      1   //云台使用C板陀螺仪控制
#define IS_BUFF_STANDARD    0   //打符步兵

typedef struct 
{
//云台状态
    DEVICE_STATE state;
    struct{
        float pit_Angle;
        float yaw_Angle;
    }target;
//云台设备
    IMU *imu;
    RC_ctrl_t *dr16;
	VISION *vision;
    DJI_MOTOR *yaw,*pit;

    void (*state_Setup)(DEVICE_STATE);
}GIMBAL;
extern GIMBAL gimbal;

extern int vision_Flag;

static void gimbal_Posture_Ctrl(void);
static void gimbal_State_Handoff(DEVICE_STATE mode);


#endif



