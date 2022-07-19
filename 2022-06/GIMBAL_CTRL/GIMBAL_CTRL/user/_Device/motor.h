/*云台电机2.0版本
                                                __----~~~~~~~~~~~------___
                                    .  .   ~~//====......          __--~ ~~     使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~                     2022-1-11   武文斌
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-                     此为COD云台电机封装，云台共5颗电机，全部为大疆电机
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                          包括：
    _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /                                   2颗GM6020用于P，Y轴
    .~       .~       |   \\ -_    /  /-   /   ||      \   /                                  2颗M3508用于两个摩擦轮
/  ____  /         |     \\ ~-_/  /|- _/   .||       \ /                                      1颗M2006用于拨弹
|~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\                                 
            '         ~-|      /| C  |-~\~~       __--~~                            
                        |-~~-_/ | O  |   ~\_   _-~            /\
                            /  \  D  \__   \/~                \__
                        _--~ _/ | .-~~____--~-/                  ~~==.
                        ((->/~   '.|||' -_|    ~~-/ ,              . _||
                                -_     ~\      ~~---l__i__i__i--~~_/
                                _-~-__   ~)  \--______________--~~
                                //.-~~~-~_--~- |-------~~~~~~~~
                                        //.-~~~--\
    代码无bug！！！
*/ 
#ifndef MOTOR_H
#define MOTOR_H
#include "main.h"
#include "can.h"
#include "pid.h"


typedef enum{
    YAW,
    PITCH,
    SHOOTL,
    SHOOTR,
    TRIGGER,
    MOTOR_NUM,
}MOTOR_USAGE;

typedef enum{
    _6020,
    _3508,
    _2006,
    MOTOR_TYPE_NUM,
}MOTOR_TYPE;

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

    PID pid;
    CAN_TypeDef* CANx;
    CanTxMsg* txMsg;
    MOTOR_TYPE type;
    MOTOR_USAGE usage;

}DJI_MOTOR;
extern DJI_MOTOR Gimbal[MOTOR_NUM];


void get_Motor_Data(CAN_TypeDef* CANx,CanRxMsg* RxMsg,DJI_MOTOR* DJI_Motor);
static float encoder2Angle(MOTOR_USAGE motor,int16_t act_Encoder,float torque_Ratio);

#endif



