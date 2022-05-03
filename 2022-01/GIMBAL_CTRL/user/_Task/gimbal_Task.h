#ifndef GIMBAL_TASK_H
#define GIMBAL_TASK_H


#include "gimbal.h"
#include "vision.h"
#include "filter.h"
#include "uart2.h"
#include "can1.h"
#include "can2.h"
#include "rc.h"

extern extKalman_t AI_YAW_KB,AI_PITCH_KB;
extern bool IF_VISION_ENABLE ;


extern float Gimbal_PID_Err[GIMBAL_MOTOR_NUM][GIMBAL_STATE_NUM];
extern extKalman_t  Gimbal_PIT_RC;
extern extKalman_t  Gimbal_YAW_RC;


#endif



