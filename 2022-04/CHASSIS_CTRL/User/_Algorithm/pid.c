#include "pid.h"
/*
    PID_Init外调初始化，分参数
    PID_Init_ByParamArray外调初始化函数，需要先初始化参数数组，将数组指针传入参数2
    PID_Calculating 算法执行函数，由实例化出的对象用指针调用，

------2021.11.15---武文斌-首次完成

*/

//                    比例系数 积分系数 微分系数 误差限幅  积分限幅  比例输出限幅 积分输出限幅 总输出限幅
void PID_Init(PID* pid,float p,float i ,float d,float e_M,float I_M,float P_OM,float I_OM,float D_OM,float T_OM)
{
    pid->middle.P_OUT=0;
    pid->middle.I_OUT=0;
    pid->middle.D_OUT=0;
    pid->middle.T_OUT=0;

    pid->middle.Err=0;
    pid->middle.Err_Last=0;
    pid->middle.Integral=0;

    pid->PID_Calc   = PID_Calculating;
    pid->param_Init = PID_Parameter_Init;
    pid->PID_Reset  = PID_Reset;
    pid->param_Init(&pid->middle.Param,p,i,d,e_M,I_M,P_OM,I_OM,D_OM,T_OM);
}

void PID_Init_ByParamArray(PID* pid,float* param)
{
    PID_Init(pid,param[0],param[1],param[2],param[3],param[4],param[5],param[6],param[7],param[8]);
}
//重置PID控制器（停止输出，历史数据清空）
void PID_Reset(PID_MIDDLE*p)
{
    p->P_OUT=0;
    p->I_OUT=0;
    p->D_OUT=0;
    p->T_OUT=0;

    p->Err=0;
    p->Err_Last=0;
    p->Integral=0;
}

static void PID_Parameter_Init(PID_PARAMETER*param,float p,float i ,float d,float e_M,float I_M,float P_OM,float I_OM,float D_OM,float T_OM)
{
    param->P=p;
    param->I=i;
    param->D=d;
    param->Err_Max=e_M;
    param->Integral_MAX=I_M;

    param->P_OUT_MAX=P_OM;
    param->I_OUT_MAX=I_OM;
    param->D_OUT_MAX=D_OM;
    param->T_OUT_MAX=T_OM;             
}

static float PID_Calculating(PID_MIDDLE*p,float err)
{
    p->Err_Last=p->Err;		
    p->Err = err;			
    VAL_LIMIT(p->Err,-p->Param.Err_Max,p->Param.Err_Max);	

    p->Integral += (p->Err * 0.002f);	
    VAL_LIMIT(p->Integral,-p->Param.Integral_MAX,p->Param.Integral_MAX); 

    p->P_OUT = p->Param.P * p->Err;			
    VAL_LIMIT(p->P_OUT,-p->Param.P_OUT_MAX,p->Param.P_OUT_MAX);	

    p->I_OUT = p->Param.I * p->Integral;		
    VAL_LIMIT(p->I_OUT,-p->Param.I_OUT_MAX,p->Param.I_OUT_MAX);

    p->D_OUT = p->Param.D * (p->Err-p->Err_Last);	
    VAL_LIMIT(p->D_OUT,-p->Param.D_OUT_MAX,p->Param.D_OUT_MAX);

    p->T_OUT = p->P_OUT
             + p->I_OUT
             + p->D_OUT;	

    VAL_LIMIT(p->T_OUT,-p->Param.T_OUT_MAX,p->Param.T_OUT_MAX);

    return p->T_OUT;
}




