/*发射电机1.0版本
                                                __----~~~~~~~~~~~------___
                                    .  .   ~~//====......          __--~ ~~  使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~               1.此为发射电机(SHOOT_L,SHOOT_R)文件第一版，首次完成2021-11-25武文斌
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-               2.经过调试暂时没有发现bug，用于参考PID1.0版本的使用
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                    3.对电机PID控制进行了二次封装，建议其他电机参考使用。
    _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /                             
    .~       .~       |   \\ -_    /  /-   /   ||      \   /                            
/  ____  /         |     \\ ~-_/  /|- _/   .||       \ /                                
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

#ifndef SHOOT_H
#define SHOOT_H
#include "main.h"
#include "pid.h"



//发射电机类型枚举
enum{
    L=0,
    R,
    SHOOT_MOTOR_NUM,
};

//发射模式枚举，对于云台后续在不同模式下会用不同的PID控制参数
typedef enum{
    STURN_OFF=0,
    STURN_ON,
    SHOOT_STATE_NUM,
}SHOOT_STATE;

/*  GM6020封装，继承CAN信息结构体，PID模式，PID串级控制，以及经过二次处理的控制函数指针
    注意：一定要给函数指针初始化！！！否则会导致程序死在溢出报错。
*/
typedef struct
{
    struct{
        int ID;
        volatile float   angle;
        volatile int16_t current;
        volatile int16_t velocity;
        volatile int16_t encoder;
        volatile uint8_t temperature;
    }Data;

    SHOOT_STATE state;
    PID Ctrl[SHOOT_STATE_NUM];
    int16_t (*ctrl_Calc)(float);
}Shoot_Motor;

extern Shoot_Motor Shoot[SHOOT_MOTOR_NUM];
void Shoot_Motor_PID_Param_Init(void);

static int16_t Shoot_Ctrl_L(float target);
static int16_t Shoot_Ctrl_R(float target);


#endif



