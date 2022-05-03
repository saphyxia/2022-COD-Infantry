/*地盘电机1.0版本
                                                __----~~~~~~~~~~~------___
                                    .  .   ~~//====......          __--~ ~~     使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~                     1.此为地盘电机文件第一版，改版来自2021-11-16武文斌云台电机
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-                     
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                          
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

#ifndef _motor_H
#define _motor_H
#include "main.h"
#include "stm32f4xx.h"
#include "pid.h"
#include "gyro.h"
#include "CAN_Receive.h"

//地盘电机类型枚举
typedef enum
{
	Left_Font=0,
	Righ_Font,
	Left_Rear,
	Righ_Rear,
	Chassis_Motor_NUM
}Motor_Type;
//PID模式枚举，对于地盘在不同控制模式下会用不同的PID参数
typedef enum
{
    TURN_OFF=0,
    REMOTE,
    KEYBORD,
    CHASSIS_PID_STATE_NUM,

    SPEED=0,
    ANGLE,
    CTRL_STATE_NUM
}PID_STATE_t;
//二次封装PID，引入串级
typedef struct{
    PID speed_Loop;
    PID angle_Loop;
}PID_t;
//CAN通信成员变量封装，包括电机ID，实时接收的电机信息，用于CAN中断接收
typedef struct{
    int ID;
    volatile float   angle;
    volatile int16_t current;
    volatile int16_t velocity;
    volatile int16_t encoder;
    volatile uint8_t temperature;
}CAN_RX_Data_t;
/*  M3508封装，继承CAN信息结构体，PID模式，PID串级控制，以及经过二次处理的控制函数指针
    注意：一定要给函数指针初始化！！！否则会导致程序死在溢出报错。
*/
typedef struct
{
    CAN_RX_Data_t Data;
    uint8_t state;
    PID pid[CHASSIS_PID_STATE_NUM];
    int16_t (*speed_Ctrl)(Motor_Type,uint8_t,float);
}DJI_M3508_t;

extern DJI_M3508_t Chassis[Chassis_Motor_NUM];

//地盘跟随云台解算结构体
typedef struct 
{
    int16_t Yaw_angle;
    GYRO_DATA_T Gyro;
    PID_t pid;
    int16_t (*gyro_Ctrl)(GYRO_DATA_T*,int16_t);
}W_TEMP_T;

extern W_TEMP_T W_Temp_PID;

typedef struct
{
    PID pid;
    int16_t (*Current_updata_Ctrl)(Motor_Type,float);
}POWER_CONTROL_T;

extern POWER_CONTROL_T POWER_Control_PID[Chassis_Motor_NUM];

extern void Motor_PID_Param_Init(void);
static int16_t DJI_Motor_Speed_Ctrl(Motor_Type type,uint8_t tate,float err);
static int16_t GYRO_Gyro_Ctrl(GYRO_DATA_T *p,int16_t midangle);
static int16_t Current_Update_Ctrl(Motor_Type type,float err);
#endif


