#include "gimbal_Task.h"


/*--------------------------   变量声明   ------------------------------*/

GIMBAL gimbal=
{
    .state=OFF,
	.vision=&vision,
    .imu=&imu,
    .dr16=&rc_ctrl,
    .yaw=&Gimbal[YAW],
    .pit=&Gimbal[PITCH],
    .state_Setup=gimbal_State_Handoff,
};

float yaw_PID_Param[2][PID_Param_NUM]={
    [ON]={
        65,//P
        1.5,//I
        0,//D
        400,//Err_Max
        2000,//Integral_MAX
        26000,//P_OUT_MAX
        2500,//I_OUT_MAX
        0,//D_OUT_MAX
        30000 //T_OUT_MAX
    },
};
float pit_PID_Param[2][PID_Param_NUM]={
    [ON]={
        82,//P
        0.36,//I
        0,//D
        500,//Err_Max
        30500 ,//Integral_MAX
        20000,//P_OUT_MAX
        11000,//I_OUT_MAX
        0,//D_OUT_MAX
        30000 //T_OUT_MAX
    },
};

int vision_Flag=0;
float ky=1.f,kp=1.f;

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void GIMBAL_TASK(void *args)
{
    uint32_t currentTime;

	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

        switch (gimbal.state)
        {
        case ON:{
            gimbal.target.yaw_Angle +=-gimbal.dr16->mouse.x*0.003f
                                      -gimbal.dr16->rc.ch[0]*0.0003f
                                      //+gimbal.tx2->yaw_Err*0.012f*IF_MOUSE_PRESSED_RIGH//右键开启自瞄
                                      ;

            gimbal.target.pit_Angle +=-gimbal.dr16->mouse.y*0.003f
                                      +gimbal.dr16->rc.ch[1]*0.0001f
                                      //-gimbal.tx2->pit_Err*0.015f*IF_MOUSE_PRESSED_RIGH//右键开启自瞄
                                      ;
        }break;

        case OFF:{//云台卸力（对应PID参数为0）
            gimbal.target.yaw_Angle = imu.yaw_Angle;
            gimbal.target.pit_Angle = 25.0f; 
        }break;

        case CALIBRATING:{//校准（上电默认为该状态，陀螺仪工作后才会切出去）
            gimbal.target.yaw_Angle = 180.0f;//看陀螺仪校准启动后是多少(避免上来就甩脑袋)
            gimbal.target.pit_Angle = 25.0f;
        }break;

        default:
            break;
        }
        
        gimbal_Posture_Ctrl();

        vTaskDelayUntil(&currentTime, GIMBAL_TASK_TIM);//绝对延时
	}
}


//云台状态切换(在dbus任务中调用)
static void gimbal_State_Handoff(DEVICE_STATE state)
{
    if(state!=gimbal.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		gimbal.state=state;
        PID_Init_ByParamArray(&gimbal.yaw->pid,yaw_PID_Param[state]);
        PID_Init_ByParamArray(&gimbal.pit->pid,pit_PID_Param[state]);
    }
}

//云台姿态控制（全模式调用）
int key_KF=0;
static void gimbal_Posture_Ctrl(void)
{
    float angle_Err[2],speed_Err[2];
    int16_t send_Value[2];

//期望角度处理
    if(gimbal.target.yaw_Angle>180.0f){
        gimbal.target.yaw_Angle-=360.0f;
    }
    if(gimbal.target.yaw_Angle<-180.0f){
        gimbal.target.yaw_Angle+=360.0f;
    }
    VAL_LIMIT(gimbal.target.pit_Angle,2.0f,52.0f);
//角度误差计算
    //常规控制
    angle_Err[0] = gimbal.target.yaw_Angle - gimbal.imu->yaw_Angle;//使用陀螺仪角度，相对地球改变
    angle_Err[1] = gimbal.target.pit_Angle - gimbal.pit->Data.angle;//使用编码器角度，相对车体改变
    //自瞄
    if((IF_MOUSE_PRESSED_RIGH||vision_Flag==1)&&gimbal.vision->tx2->isFind==1){
        //角度误差   =         视觉计算        +          前馈补偿（now-last)*k
        if(key_KF){
            angle_Err[0] = gimbal.vision->yaw*ky;
            angle_Err[1] = gimbal.vision->tx2->pit_Err[0]*kp;
        }
        else{
            angle_Err[0] = gimbal.vision->tx2->yaw_Err[0]*ky;
            angle_Err[1] = gimbal.vision->tx2->pit_Err[0]*kp;
        }
        VAL_LIMIT(angle_Err[0],-6.0f,6.0f);
        VAL_LIMIT(angle_Err[1],-6.0f,6.0f);
        gimbal.target.yaw_Angle = imu.yaw_Angle;
        gimbal.target.pit_Angle = gimbal.pit->Data.angle;
    }
//角度误差处理
    if(angle_Err[0]>=180.0f)angle_Err[0]-=360.0f;
    if(angle_Err[0]<-180.0f)angle_Err[0]+=360.0f;
    VAL_LIMIT(angle_Err[0],-100.0f,100.0f);
    VAL_LIMIT(angle_Err[1],-100.0f,100.0f);
//速度误差计算
    speed_Err[0] = angle_Err[0]*57.29578f - gimbal.imu->yaw_Gyro;
    speed_Err[1] = angle_Err[1]*57.29578f - gimbal.imu->pit_Gyro;
	//speed_Err[1] = gimbal.dr16->rc.ch[1] - gimbal.imu->pit_Gyro;
//速度控制(PID控制方案)
    send_Value[0] = (int16_t)gimbal.yaw->pid.PID_Calc(&gimbal.yaw->pid.middle,speed_Err[0]);
    send_Value[1] = (int16_t)gimbal.pit->pid.PID_Calc(&gimbal.pit->pid.middle,speed_Err[1]);
//发送装载
    gimbal.yaw->txMsg->Data[L[gimbal.yaw->Data.StdId-0x205]] = (uint8_t)(send_Value[0]>>8);
    gimbal.yaw->txMsg->Data[H[gimbal.yaw->Data.StdId-0x205]] = (uint8_t)(send_Value[0]);
    gimbal.pit->txMsg->Data[L[gimbal.pit->Data.StdId-0x205]] = (uint8_t)(send_Value[1]>>8);
    gimbal.pit->txMsg->Data[H[gimbal.pit->Data.StdId-0x205]] = (uint8_t)(send_Value[1]);
}








