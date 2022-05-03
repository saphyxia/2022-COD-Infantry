#ifndef _motor_H
#define _motor_H
#include "main.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "can.h"

typedef enum{
    Left_Font=0,
    Righ_Font,
    Left_Rear,
    Righ_Rear,
    YAW,
    MOTOR_USAGE_NUM,
}MOTOR_USAGE;

typedef enum{
    _6020=0,
    _3508,
    _2006,
    MOTOR_TYPE_NUM,
}MOTOR_TYPE;//电机类型枚举

typedef enum{
    SPEED,
    ANGLE,
    PID_TYPE,
}MOTOR_PID_TYPE;

/*  大疆电机封装，继承CAN信息结构体，
    注意：一定要给函数指针初始化！！！否则会导致程序死在溢出报错。
*/
typedef struct
{
    struct{
        uint32_t StdId;
        volatile float angle;
        volatile int16_t current;
        volatile int16_t velocity;
        volatile int16_t encoder;
        volatile uint8_t temperature;
    }Data;

    PID pid_Speed,pid_Angle;
    CAN_TypeDef *CANx;
    CanTxMsg *txMsg;
    MOTOR_TYPE type;
    MOTOR_USAGE usage;

}DJI_MOTOR;

extern DJI_MOTOR Chassis_Motor[];
void get_Motor_Data(CAN_TypeDef* CANx,CanRxMsg* RxMsg,DJI_MOTOR* DJI_Motor);
#endif


