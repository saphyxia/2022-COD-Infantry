#ifndef VISION_TASK_H
#define VISION_TASK_H
#include "FreeRTOS_Init.h"
#include "bmi088.h"
#include "pid.h"
#include "assist.h"
#include "filter.h"
#include "uart.h"
#include "vision.h"



typedef struct{
//帧差时间
    TickType_t delta_T;
//发送结构体
    struct{
        uint8_t sign_Flag;//1++2+-3-+4--
        uint16_t inform[4];
    }send2uart;
//卡尔曼
    struct{
        float *armor_Speed;//装甲板移动速度[0]yaw;[1]pitch
        float *measurement;//测量值[0]yaw;[1]pitch
        float *predict[2]; //预测值[0][0]yaw轴角度，[0][1]yaw轴速度;[1][0]pitch轴角度，[1][1]pitch轴速度
}KF;
//云台位置偏移量（误差量）
    float pit,yaw;
//tx2挂载
    TX2 *tx2;
// 陀螺仪挂载
    IMU *imu;
}VISION;
extern VISION vision;

static TickType_t get_delta_time(void);
static void send2uart1(void);
static float* get_measurement(void);
static float* get_armor_speed(float* nowValue);

#endif



