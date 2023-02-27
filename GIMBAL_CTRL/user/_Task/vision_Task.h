#ifndef VISION_TASK_H
#define VISION_TASK_H
#include "FreeRTOS_Init.h"
#include "bmi088.h"
#include "pid.h"
#include "assist.h"
#include "filter.h"
#include "uart.h"
#include "vision.h"

/*数据结构*/
typedef struct
{
    uint16_t nowLength;
    uint16_t queueLength;
    float queueTotal;
    //长度
    float queue[100];
    //指针
    float aver_num;//平均值

    float Diff;//差分值

    uint8_t full_flag;
} QueueObj;


typedef struct{
//帧差时间
    TickType_t delta_T;
//发送结构体
    struct{
        uint8_t sign_Flag;//1++2+-3-+4--
        uint8_t inform[4];
    }send2uart;
//云台位置偏移量（误差量）
    float pit,yaw;
//tx2挂载
    TX2 *tx2;
// 陀螺仪挂载
    IMU *imu;
}VISION;
extern VISION vision;

extern float predict_yaw_temp;
extern float yaw_accel;
extern float armor_position2vision[2];

extern float imu_yaw_angle;
extern bool IF_Fire_Ready;

extern bool key_KF;
extern bool IS_SENTRY_MODE;
extern extKalman_t KF_YAW_FEED,KF_YAW_ACCEL,KF_YAW_DIS,KF_YAW_ANGLE,KF_YAW_SEED;


static TickType_t get_delta_time(void);
static void send2uart1(void);
static float* get_measurement(void);
static float* get_armor_speed(float* nowValue);
float Get_Diff(uint8_t queue_len, QueueObj *Data,float add_data);
#endif



