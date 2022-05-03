/*云台电机1.1版本
                                                __----~~~~~~~~~~~------___
                                    .  .   ~~//====......          __--~ ~~     使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~                     1.此为云台电机(YAW,PIT)文件第一版，首次完成2021-11-25武文斌
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-                     2.经过调试暂时没有发现bug，用于参考PID1.0版本的使用
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                          3.对电机PID控制进行了二次封装，建议其他电机参考使用。
    _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /                             
    .~       .~       |   \\ -_    /  /-   /   ||      \   /                            2021-12-3更新，作者：武文斌
/  ____  /         |     \\ ~-_/  /|- _/   .||       \ /                                对Gimbal_Ctrl_YAW和Gimbal_Ctrl_PIT做了修改，配合C文件中宏定义
|~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\                                 SEND_ENABLE,用于是否开启自动发送CAN信号。
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


#ifndef GIMBAL_H
#define GIMBAL_H
#include "main.h"
#include "pid.h"

#define SEND_ENABLE    1   //CAN发送许可，0为不发送
#define GIMBAL_USE_IMU 1   //云台使用C板陀螺仪控制


//云台电机类型枚举
enum{
    YAW=0,
    PITCH,
    GIMBAL_MOTOR_NUM,
};

//云台模式枚举，对于云台后续在不同模式下会用不同的PID控制参数
typedef enum{
    GTURN_OFF=0,
    RC_MECH,
    RC_GYRO,
    KB_MECH,
    KB_GYRO,
    AI_STAN,//底面
    AI_SENTRY,//哨兵
    AI_BUFF_M,//大风车
    AI_BUFF_S,//小风车
    GIMBAL_STATE_NUM,
}GIMBAL_STATE;

/*  GM6020封装，继承CAN信息结构体，PID模式，PID串级控制，以及经过二次处理的控制函数指针
    注意：一定要给函数指针初始化！！！否则会导致程序死在溢出报错。
*/
typedef struct
{
    struct{
        int ID;
        volatile float angle;
        volatile int16_t current;
        volatile int16_t velocity;
        volatile int16_t encoder;
        volatile uint8_t temperature;
    }Data;
#if GIMBAL_USE_IMU
    struct{
        volatile float angle;
        volatile float gyro;
    }IMU;
#endif
    struct{
        PID speed_Loop;
        PID angle_Loop;
    }Ctrl[GIMBAL_STATE_NUM];

    CAN_TypeDef* CAN_Port;
    GIMBAL_STATE state;
    void (*get_Data)(CanRxMsg*);
    int16_t (*ctrl_Calc)(float);//
}Gimbal_Motor;
extern Gimbal_Motor Gimbal[GIMBAL_MOTOR_NUM];

void Gimbal_Motor_PID_Param_Init(void);

static void Gimbal_YAW_Data_Get(CanRxMsg* RxMsg);
static void Gimbal_PIT_Data_Get(CanRxMsg* RxMsg);
static int16_t Gimbal_Ctrl_YAW(float err);
static int16_t Gimbal_Ctrl_PIT(float err);

#endif



