#ifndef SHOOT_TASK_H
#define SHOOT_TASK_H
#include "FreeRTOS_Init.h"
#include "systemState_Task.h"
#include "uart.h"
#include "motor.h"
#include "rc.h"
#include "pid.h"

#define SHOOT_SPEED_15M_S    4600
#define SHOOT_SPEED_18M_S    5100
#define SHOOT_SPEED_30M_S    7450

#define TRIGGER_FREQ_2_HZ     750
#define TRIGGER_FREQ_3_HZ     1000
#define TRIGGER_FREQ_4_HZ     1250
#define TRIGGER_FREQ_5_HZ     1500
#define TRIGGER_FREQ_6_HZ     1750
#define TRIGGER_FREQ_7_HZ     2000
#define TRIGGER_FREQ_8_HZ     2400
#define TRIGGER_FREQ_9_HZ     2700
#define TRIGGER_FREQ_10_HZ    3000

typedef enum{
    AI,
    AUTO,
    SINGLE,
	COILED,
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

extern bool IF_SHOOT_ENABLE;

static void wheel_Ctrl(void);
static void trigger_Ctrl(void);
static void trigger_Stall_Handle(void);
void switch_shoot_mode(void);
static void shoot_State_Handoff(DEVICE_STATE mode);
static float Temp_Fix_speed(uint16_t real_speed);
static float SpeedAdapt(float real_S , float min_S, float max_S,float up_num , float down_num);
static uint16_t Trigger_Speed_deliver(uint16_t cooling_limit);
#endif



