#ifndef VISION_TASK_H
#define VISION_TASK_H

#include "gimbal_Task.h"
#include "gimbal.h"
#include "filter.h"

#define VISION_INFO_BUF_LEN 32

enum{
    LAST,
    NOW,
    TIME_FLAG_NUM
};

extern extKalman_t KF_Vision[GIMBAL_MOTOR_NUM];
extern extKalman_t KF_Vision_Forecast[GIMBAL_MOTOR_NUM];

extern kalman_filter_t KF_Filter_Vision_Gimbal[GIMBAL_MOTOR_NUM];
extern kalman_filter_init_t KF_Filter_Param_Vision_Gimbal[GIMBAL_MOTOR_NUM];





#endif



