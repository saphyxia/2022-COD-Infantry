#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H
#include "FreeRTOS_Init.h"
#include "systemState_Task.h"
#include "uart.h"
#include "motor.h"
#include "rc.h"
#include "pid.h"

typedef enum{
    AI,
    AUTO,
    SINGLE,
    SHOOT_MODE_NUM,
}SHOOT_MODE;

typedef struct 
{
//发射机构参数
    DEVICE_STATE state;
    SHOOT_MODE mode;
    uint8_t trigger_Buf;
    int bulletNum;//发射计数
    float wheel_Speed[STATE_NUM][ROBOT_MODE_NUM][ROBOT_LEVEL_NUM];
    float trigger_Angle;
//设备
    RC_ctrl_t *dr16;
    DJI_MOTOR *wheel_L,*wheel_R,*trigger;
    void (*state_Setup)(DEVICE_STATE);
}SHOOT;
extern SHOOT shoot;

static void wheel_Ctrl(void);
static void trigger_Ctrl(void);
static void trigger_Stall_Handle(void);
static void shoot_State_Handoff(DEVICE_STATE mode);

#endif



