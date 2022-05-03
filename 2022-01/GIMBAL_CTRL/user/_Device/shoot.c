#include "shoot.h"




/*  2颗电机(M3508x2) 
    两种模式(关闭，开启)
    没有串级PID，
*/Shoot_Motor Shoot[SHOOT_MOTOR_NUM]={ [L].Data.ID=0x201, [L].state=STURN_OFF, [L].ctrl_Calc=Shoot_Ctrl_L ,
                                       [R].Data.ID=0x202, [R].state=STURN_OFF, [R].ctrl_Calc=Shoot_Ctrl_R };

float SHOOT_PID_Param[SHOOT_MOTOR_NUM][SHOOT_STATE_NUM][PID_Param_NUM]={
    [L]={
        [STURN_OFF]={
            0,//P
            0,//I
            0,//D
            0,//Err_Max 
            0,//Integral_MAX
            0,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            0 //T_OUT_MAX
        },
        [STURN_ON]={
            11,//P
            0.15f,//I
            0,//D
            3000,//Err_Max 
            8000,//Integral_MAX
            16000,//P_OUT_MAX
            1000,//I_OUT_MAX
						0,
            16000 //T_OUT_MAX
        }
    },
    [R]={
        [STURN_OFF]={
            0,//P
            0,//I
            0,//D
            0,//Err_Max 
            0,//Integral_MAX
            0,//P_OUT_MAX
            0,//I_OUT_MAX
						0,
            0 //T_OUT_MAX
        },
        [STURN_ON]={
            11,//P
            0.15f,//I
            0,//D
            3000,//Err_Max 
            8000,//Integral_MAX
            16000,//P_OUT_MAX
            1000,//I_OUT_MAX
						0,
            16000 //T_OUT_MAX
        }
    }
};


void Shoot_Motor_PID_Param_Init(void)
{
    PID_Init_ByParamArray(&Shoot[L].Ctrl[STURN_OFF] , SHOOT_PID_Param[L][STURN_OFF] );
    PID_Init_ByParamArray(&Shoot[L].Ctrl[STURN_ON ] , SHOOT_PID_Param[L][STURN_ON ] );
    PID_Init_ByParamArray(&Shoot[R].Ctrl[STURN_OFF] , SHOOT_PID_Param[R][STURN_OFF] );
    PID_Init_ByParamArray(&Shoot[R].Ctrl[STURN_ON ] , SHOOT_PID_Param[R][STURN_ON ] );
}

/*
*/
static int16_t Shoot_Ctrl_L(float target)
{
    return (int16_t)Shoot[L].Ctrl[Shoot[L].state].PID_Calc(&Shoot[L].Ctrl[Shoot[L].state].middle,target-Shoot[L].Data.velocity);
}

/*
*/
static int16_t Shoot_Ctrl_R(float target)
{
    return (int16_t)Shoot[R].Ctrl[Shoot[R].state].PID_Calc(&Shoot[R].Ctrl[Shoot[R].state].middle,target-Shoot[R].Data.velocity);
}



