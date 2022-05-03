/*拨弹电机1.0版本
                                                __----~~~~~~~~~~~------___
                                    .  .   ~~//====......          __--~ ~~   使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~                1.此为拨弹电机(Trigger)文件第一版，首次完成2021-11-25武文斌
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-                2.经过调试暂时没有发现bug，用于参考PID1.0版本的使用
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                     3.对电机PID控制进行了二次封装，建议其他电机参考使用。
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


#ifndef TRIGGER_H
#define TRIGGER_H
#include "main.h"
#include "pid.h"

#include "assist.h"


//云台模式枚举，对于云台后续在不同模式下会用不同的PID控制参数
typedef enum{
    TURN_OFF=0,
    AUTO,
    SINGLE,
    TRIGGER_STATE_NUM,
}TRIGGER_STATE;


/*  GM6020封装，继承CAN信息结构体，PID模式，PID串级控制，以及经过二次处理的控制函数指针
    注意：一定要给函数指针初始化！！！否则会导致程序死在溢出报错。
*/
typedef struct
{
    struct{
        int ID;
        volatile double   angle;
        volatile int16_t current;
        volatile int16_t velocity;
        volatile int16_t encoder;
        volatile uint8_t temperature;
    }Data;

    struct{
        PID speed_Loop;
        PID angle_Loop;
    }Ctrl[TRIGGER_STATE_NUM];

    TRIGGER_STATE state;
    int16_t (*ctrl_Speed)(float);
    int16_t (*ctrl_Angle)(float);
}Trigger_Motor;
extern Trigger_Motor Trigger;

extern Critical_t Trigger_critical;


void Trigger_Motor_PID_Param_Init(void);

static int16_t Trigger_Ctrl_Speed(float target);
static int16_t Trigger_Ctrl_Angle(float err);



#endif




