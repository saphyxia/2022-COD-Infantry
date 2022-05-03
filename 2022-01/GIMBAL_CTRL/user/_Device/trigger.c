#include "trigger.h"


/*  1颗电机(M2006)  
    3种模式(关闭，连发半自动，单发打符)，
    2层串级(速度环，角度环)
*/Trigger_Motor Trigger={ .Data.ID=0x205, .state = TURN_OFF, .ctrl_Speed=Trigger_Ctrl_Speed, .ctrl_Angle=Trigger_Ctrl_Angle };

Critical_t Trigger_critical;

float Trigger_PID_Param[TRIGGER_STATE_NUM][CONCATENATION_NUM][PID_Param_NUM]={
    [TURN_OFF]={//          
        [SPEED]={
            0,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        },
        [ANGLE]={//     
            0,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        }
    },
    [AUTO]={//          连发（半自动）
        [SPEED]={
            11,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        },
        [ANGLE]={//     该模式失效(不用调)
            0,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        }
    },
    [SINGLE]={//        单发
        [SPEED]={
            11,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        },
        [ANGLE]={//     该模式启用，保证一颗一颗的打，重在打符。（得调好）
            7,//P
            0,//I
            0,//D
            3000,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            10000 //T_OUT_MAX
        }
    }
};

//开局点射，按U切换半自动


void Trigger_Motor_PID_Param_Init(void)
{
//1级，速度环部分
    PID_Init_ByParamArray(&Trigger.Ctrl[TURN_OFF].speed_Loop , Trigger_PID_Param[TURN_OFF][SPEED]);
    PID_Init_ByParamArray(&Trigger.Ctrl[SINGLE   ].speed_Loop , Trigger_PID_Param[SINGLE   ][SPEED]);
    PID_Init_ByParamArray(&Trigger.Ctrl[AUTO     ].speed_Loop , Trigger_PID_Param[AUTO     ][SPEED]);

//2级，角度环部分   
    PID_Init_ByParamArray(&Trigger.Ctrl[TURN_OFF].angle_Loop , Trigger_PID_Param[TURN_OFF][ANGLE]);
    PID_Init_ByParamArray(&Trigger.Ctrl[SINGLE   ].angle_Loop , Trigger_PID_Param[SINGLE   ][ANGLE]);
    PID_Init_ByParamArray(&Trigger.Ctrl[AUTO     ].angle_Loop , Trigger_PID_Param[AUTO     ][ANGLE]);

}


/*
*/
static int16_t Trigger_Ctrl_Speed(float target)
{
    return (int16_t)Trigger.Ctrl[Trigger.state].speed_Loop.PID_Calc(&Trigger.Ctrl[Trigger.state].speed_Loop.middle,target-Trigger.Data.velocity);
}

/*
*/
static int16_t Trigger_Ctrl_Angle(float err)
{
    return (int16_t)Trigger.Ctrl[Trigger.state].speed_Loop.PID_Calc(&Trigger.Ctrl[Trigger.state].speed_Loop.middle,
                    Trigger.Ctrl[Trigger.state].angle_Loop.PID_Calc(&Trigger.Ctrl[Trigger.state].angle_Loop.middle,err)-Trigger.Data.velocity);
}



