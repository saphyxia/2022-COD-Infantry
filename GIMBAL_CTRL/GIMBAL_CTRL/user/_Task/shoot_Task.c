/*摩擦轮任务

*/

#include "shoot_Task.h"



/*--------------------------   变量声明   ------------------------------*/

SHOOT shoot=
{
    .state=OFF,
    .mode=SINGLE,
    .trigger_Buf=7,//拨盘叶数
    .dr16=&rc_ctrl,
    .wheel_L=&Gimbal[SHOOTL],
    .wheel_R=&Gimbal[SHOOTR],
    .trigger=&Gimbal[TRIGGER],

    .wheel_Speed={
        [ON]={
            [INITIAL]={//初始
                [LV_0]=1000,
                [LV_1]=0,
                [LV_2]=0,
                [LV_3]=0,
            },
            [BURST]={//爆发优先
                [LV_0]=2000,
                [LV_1]=0,
                [LV_2]=0,
                [LV_3]=0,
            },
            [RATE]={//射速优先
                [LV_0]=2000,
                [LV_1]=3500,
                [LV_2]=4500,
                [LV_3]=5500,
            },
            [COOLING]={//冷却优先
                [LV_0]=2000,
                [LV_1]=0,
                [LV_2]=0,
                [LV_3]=0,
            },
        },
    },

    .state_Setup=shoot_State_Handoff,
};

float wheel_PID_Param[2][PID_Param_NUM]={
    [ON]={
        9,//P
        0.06,//I
        0,//D
        1500,//Err_Max
        0,//Integral_MAX
        15000,//P_OUT_MAX
        3000,//I_OUT_MAX
        0,//D_OUT_MAX
        15000 //T_OUT_MAX
    },
};
float trigger_PID_Param[2][PID_Param_NUM]={
    [ON]={
        16,//P
        0.52,//I
        0,//D
        1000,//Err_Max
        1000,//Integral_MAX
        15000,//P_OUT_MAX
        1500,//I_OUT_MAX
        0,//D_OUT_MAX
        16000 //T_OUT_MAX
    },
};

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void SHOOT_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
        
        switch (shoot.mode)
        {
        case AUTO://全自动，突突突

            break;

        case SINGLE://半自动，点射

            break;
            
        case AI://智能模式
            
            break;
        default:
            break;
        }

        wheel_Ctrl();
        trigger_Ctrl();
		trigger_Stall_Handle();
        vTaskDelayUntil(&currentTime, SHOOT_TASK_TIM);//绝对延时
	}
}


//发射机构状态切换(在dbus任务中调用)
static void shoot_State_Handoff(DEVICE_STATE state)
{
    if(state!=shoot.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		shoot.state=state;
        PID_Init_ByParamArray(&shoot.wheel_L->pid,wheel_PID_Param[state]);
        PID_Init_ByParamArray(&shoot.wheel_R->pid,wheel_PID_Param[state]);
        PID_Init_ByParamArray(&shoot.trigger->pid,trigger_PID_Param[state]);
    }
}

//摩擦轮速度控制（全模式调用）（未接入裁判系统，无射速限制）
static void wheel_Ctrl(void)
{
    int16_t send_Value[2];
    float speed_Err[2];
//速度误差计算(符号相反)
    speed_Err[0] = (-shoot.wheel_Speed[shoot.state][robot.mode][robot.level] - shoot.wheel_L->Data.velocity);
    speed_Err[1] = ( shoot.wheel_Speed[shoot.state][robot.mode][robot.level] - shoot.wheel_R->Data.velocity);
//速度控制(PID控制方案)
    send_Value[0] = shoot.wheel_L->pid.PID_Calc(&shoot.wheel_L->pid.middle,speed_Err[0]);
    send_Value[1] = shoot.wheel_R->pid.PID_Calc(&shoot.wheel_R->pid.middle,speed_Err[1]);
//发送装载
    shoot.wheel_L->txMsg->Data[L[shoot.wheel_L->Data.StdId-0x201]] = (uint8_t)(send_Value[0]>>8);
    shoot.wheel_L->txMsg->Data[H[shoot.wheel_L->Data.StdId-0x201]] = (uint8_t)(send_Value[0]);
    shoot.wheel_R->txMsg->Data[L[shoot.wheel_R->Data.StdId-0x201]] = (uint8_t)(send_Value[1]>>8);
    shoot.wheel_R->txMsg->Data[H[shoot.wheel_R->Data.StdId-0x201]] = (uint8_t)(send_Value[1]);
}

//拨弹盘控制（全模式调用）（未接入裁判系统，无射频限制）
static void trigger_Ctrl(void)
{
    static TickType_t time;
    static uint8_t timeOut=1;
    int16_t send_Value;
    float angle_Err,speed_Err;
    
    if(shoot.mode==SINGLE&&(IF_MOUSE_PRESSED_LEFT||(shoot.dr16->rc.ch[3]==660))){
//点射模式
        if(timeOut){
            time=xTaskGetTickCount();
            timeOut=0;
//期望角度计算
    shoot.trigger_Angle = shoot.trigger->Data.angle
                        - 360.0f/shoot.trigger_Buf;
            if(shoot.trigger_Angle<=0)shoot.trigger_Angle+=360.0f;
        }
    }
    if(xTaskGetTickCount()-time>300){
        timeOut=1;
    }
//角度误差计算
    angle_Err = shoot.trigger_Angle - shoot.trigger->Data.angle;
//角度误差处理   
    if(angle_Err<-180.0f)angle_Err+=360.0f;
    if(angle_Err>=180.0f)angle_Err-=360.0f;
//速度误差计算
    speed_Err = angle_Err*57.29578f - shoot.trigger->Data.velocity;

    if(shoot.mode==AUTO){
//连发模式
    shoot.trigger_Angle = shoot.trigger->Data.angle;
    //速度误差计算
    speed_Err = -2000*IF_MOUSE_PRESSED_LEFT - 8*shoot.dr16->rc.ch[3] - shoot.trigger->Data.velocity;
    }
//速度控制(PID控制方案)
    send_Value = shoot.trigger->pid.PID_Calc(&shoot.trigger->pid.middle,speed_Err);
//发送装载
    shoot.trigger->txMsg->Data[L[shoot.trigger->Data.StdId-0x201]] = (uint8_t)(send_Value>>8);
    shoot.trigger->txMsg->Data[H[shoot.trigger->Data.StdId-0x201]] = (uint8_t)(send_Value);
}

//堵转判断和处理（全模式调用）
static void trigger_Stall_Handle(void)
{

}


