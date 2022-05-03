/* PID1.0版本
                                                __----~~~~~~~~~~~------___      
                                    .  .   ~~//====......          __--~ ~~      使用说明：
                    -.            \_|//     |||\\  ~~~~~~::::... /~                     1.包含pid.h (#include"pid.h")
                ___-==_       _-~o~  \/    |||  \\   C O D    _/~~-                     2.建议在需要PID控制的硬件设备的描述结构体中直接加入成员
        __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\   COD  _/~                            例如3508开一个结构体里面有从CAN接收到信息的成员，也有
    _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /                               PID成员然后直接就可以把PID里的内容调出来，
    .~       .~       |   \\ -_    /  /-   /   ||      \   /                              如：M3508[LF].pid。。。在此建议二次封装电机控制函数
/  ____  /         |     \\ ~-_/  /|- _/   .||       \ /                                3.初始化方式有两种，推荐先创建一个float数组，然后用其指针
|~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\                                   传入PID_Init_ByParamArray函数的参数2.另一种就是直接形参赋值
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


#ifndef PID_H
#define PID_H


#define VAL_LIMIT(val,min,max)	{	if(val <= min)	val=min;		\
									else if(val >=max)	val=max;    }	
//PID算法参数个数
#define PID_Param_NUM   9

//PID控制类型枚举，用于PID二次封装以及后续控制模式分类
enum{
    SPEED,
    ANGLE,
    CONCATENATION_NUM
};

//PID参数，包括PID以及最大误差值，最大积分值，最大比例输出，最大积分输出，最大总输出，用于限幅
typedef __packed struct
{
	float P,I,D;
    float Err_Max,Integral_MAX;
    float P_OUT_MAX,I_OUT_MAX,D_OUT_MAX,T_OUT_MAX;
}PID_PARAMETER;

/*  PID运算过程中间量，包括误差，前一次误差，积分，比例输出值，积分输出值，微分输出值，总输出值。
    继承了PID参数
*/
typedef __packed struct
{
    PID_PARAMETER Param;
    float Err,Err_Last,Integral;
    float P_OUT,I_OUT,D_OUT;
    float T_OUT;//P_OUT+I_OUT+D_OUT;
}PID_MIDDLE;

/*  继承了PID中间量，同时也得到参数(层级继承)
    三个函数指针，param_Init用于初始化PID参数，不会涉及外部调用，可以不考虑
                 PID_Reset用来重置运算，会将中间量清空，切忌循环调用(会导致PID废掉的)
                 PID_Calc此为外部调用最重要的运算函数，参数1为PID参数容器，即实例化出的PID结构体
                                                     参数2为实时误差(控制器目的即将误差收敛至零)
*/
typedef __packed struct
{
    PID_MIDDLE middle;
    void  (*param_Init)(PID_PARAMETER*,float,float,float,float,float,float,float,float,float);
    void  (*PID_Reset)(PID_MIDDLE*);
    float (*PID_Calc)(PID_MIDDLE*,float);
}PID;

void PID_Init(PID* pid,float p,float i ,float d,float e_M,float I_M,float P_OM,float I_OM,float D_OM,float T_OM);
void PID_Init_ByParamArray(PID* pid,float* param);
static void PID_Parameter_Init(PID_PARAMETER*,float p,float i ,float d,float e_M,float I_M,float P_OM,float I_OM,float D_OM,float T_OM);
static void PID_Reset(PID_MIDDLE*p);
static float PID_Calculating(PID_MIDDLE*p,float err);


#endif




