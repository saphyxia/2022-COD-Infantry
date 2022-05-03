#include "gimbal_Task.h"
#include "shoot_Task.h"
#include "chassis_Task.h"

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

float yaw_PID_Param[2][2][PID_Param_NUM]={
    [ON]={
        [0]={
            120,//P
            0.1,//I
            0,//D
            200,//Err_Max
            20000,//Integral_MAX
            26000,//P_OUT_MAX
            2500,//I_OUT_MAX
            0,//D_OUT_MAX
            30000 //T_OUT_MAX
        },
        [1]={
            60,//P//60
            0,//I
            0,//D
            30,//Err_Max
            2000,//Integral_MAX
            26000,//P_OUT_MAX
            2500,//I_OUT_MAX
            0,//D_OUT_MAX
            30000 //T_OUT_MAX
			},
    },
};
float pit_PID_Param[2][2][PID_Param_NUM]={
    [ON]={
        [0]={
            -100,//P
            -0.14f,//I
            0,//D
            300,//Err_Max
            20000,//Integral_MAX
            26000,//P_OUT_MAX
            2500,//I_OUT_MAX
            0,//D_OUT_MAX
            30000 //T_OUT_MAX
        },
        [1]={
            -60,//P 60
            0,//I
            0,//D
            40,//Err_Max
            2000,//Integral_MAX
            26000,//P_OUT_MAX
            2500,//I_OUT_MAX
            0,//D_OUT_MAX
            30000 //T_OUT_MAX
        },
    }
};

int vision_Flag=0;
float ky=1.f,kp=1.f;
/*--------------------------   变量声明   ------------------------------*/
float target_yaw_last = 0.f;
float yaw_cps = 0.f;
float yaw_cps_k = 1.5f;
float yaw_cha_k = 0.033f;
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
            gimbal.target.yaw_Angle += -Mouse_X_Speed()*0.002f
                                       -gimbal.dr16->rc.ch[0]*0.0003f;

            gimbal.target.pit_Angle += -Mouse_Y_Speed()*0.0035f
                                       +(shoot.state==OFF)*gimbal.dr16->rc.ch[1]*0.0001f;
        }break;

        case OFF:{//云台卸力（对应PID参数为0）
            gimbal.target.yaw_Angle = imu.yaw_Angle;
//						gimbal.target.yaw_Angle = gimbal.yaw->Data.angle;
            gimbal.target.pit_Angle = imu.pit_Angle;
        }break;

        case CALIBRATING:{//校准（上电默认为该状态，陀螺仪工作后才会切出去）
            gimbal.target.yaw_Angle = imu.yaw_Angle;
//						gimbal.target.yaw_Angle = gimbal.yaw->Data.angle;
            gimbal.target.pit_Angle = imu.pit_Angle;
        }break;

        default:
            break;
        }
				//yaw轴前馈控制量计算
				yaw_cps = ramp_Float(yaw_cps,(gimbal.target.yaw_Angle-target_yaw_last)*1000.0f,8);
				target_yaw_last = gimbal.target.yaw_Angle;
				//云台电机控制
        gimbal_Posture_Ctrl();
				Key_Q();
				Key_E();
        vTaskDelayUntil(&currentTime, GIMBAL_TASK_TIM);//绝对延时
	}
}


//云台状态切换(在dbus任务中调用)
static void gimbal_State_Handoff(DEVICE_STATE state)
{
    if(state!=gimbal.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		gimbal.state=state;
        PID_Init_ByParamArray(&gimbal.yaw->pid_Angle,yaw_PID_Param[state][1]);
        PID_Init_ByParamArray(&gimbal.pit->pid_Angle,pit_PID_Param[state][1]);
        PID_Init_ByParamArray(&gimbal.yaw->pid_Speed,yaw_PID_Param[state][0]);
        PID_Init_ByParamArray(&gimbal.pit->pid_Speed,pit_PID_Param[state][0]);
    }
}

//云台姿态控制（全模式调用）
static void gimbal_Posture_Ctrl(void)
{
    float angle_Err[2],speed_Err[2];
    int16_t send_Value[2];

//期望角度处理
    VAL_LIMIT(gimbal.target.pit_Angle,-26.f,35.f);
//角度误差计算
    //常规控制
		
    angle_Err[0] = gimbal.target.yaw_Angle - gimbal.imu->yaw_Angle;//使用陀螺仪角度，相对地球改变
    angle_Err[1] = gimbal.target.pit_Angle - gimbal.imu->pit_Angle;//使用陀螺仪角度，相对地球改变
//    angle_Err[0] = gimbal.target.yaw_Angle - gimbal.yaw->Data.angle;//使用陀螺仪角度，相对地球改变

    //自瞄
//		if(Key_MOUSE_R()==true)
//		{
//			vision_Flag=1;
//		}else{
//			vision_Flag=0;
//		}
		if(rc_ctrl.rc.ch[4]==660 || Key_C()==true)
		{
			vision_Flag=1;
		}else if(rc_ctrl.rc.ch[4]==-660)
		{
			vision_Flag=0;
		}
    if((vision_Flag==1)&&gimbal.vision->tx2->isFind==1){
        //角度误差   =         视觉计算        +          前馈补偿（now-last)*k
        if(key_KF){
            angle_Err[0] = gimbal.vision->yaw*ky;
            angle_Err[1] = gimbal.vision->tx2->pit_Err[0]*kp;
        }
        else{
            angle_Err[0] = gimbal.vision->tx2->yaw_Err[0]*ky;
            angle_Err[1] = gimbal.vision->tx2->pit_Err[0]*kp;
        }
        VAL_LIMIT(angle_Err[0],-3.0f,3.0f);
        VAL_LIMIT(angle_Err[1],-3.0f,3.0f);
        gimbal.target.yaw_Angle = imu.yaw_Angle;
        gimbal.target.pit_Angle = imu.pit_Angle;
    }
//角度误差处理
    VAL_LIMIT(angle_Err[0],-100.0f,100.0f);
    VAL_LIMIT(angle_Err[1],-100.0f,100.0f);
//速度误差计算
    speed_Err[0] = gimbal.yaw->pid_Angle.PID_Calc(&gimbal.yaw->pid_Angle.middle,angle_Err[0]) - gimbal.imu->yaw_Gyro 
								 + (!vision_Flag)*yaw_cps_k*yaw_cps + (chassis.mode==SPIN)*yaw_cha_k*robot.Chas_Gyro;
//    speed_Err[0] = gimbal.yaw->pid_Angle.PID_Calc(&gimbal.yaw->pid_Angle.middle,angle_Err[0]) - gimbal.yaw->Data.velocity;

    speed_Err[1] = gimbal.pit->pid_Angle.PID_Calc(&gimbal.pit->pid_Angle.middle,angle_Err[1]) - gimbal.imu->pit_Gyro;

		//速度控制(PID控制方案)
    send_Value[0] = (int16_t)gimbal.yaw->pid_Speed.PID_Calc(&gimbal.yaw->pid_Speed.middle,speed_Err[0]);
    send_Value[1] = (int16_t)gimbal.pit->pid_Speed.PID_Calc(&gimbal.pit->pid_Speed.middle,speed_Err[1]);
//发送装载
    gimbal.yaw->txMsg->Data[H[gimbal.yaw->Data.StdId-0x205]] = (uint8_t)(send_Value[0]>>8);
    gimbal.yaw->txMsg->Data[L[gimbal.yaw->Data.StdId-0x205]] = (uint8_t)(send_Value[0]);
    gimbal.pit->txMsg->Data[H[gimbal.pit->Data.StdId-0x205]] = (uint8_t)(send_Value[1]>>8);
    gimbal.pit->txMsg->Data[L[gimbal.pit->Data.StdId-0x205]] = (uint8_t)(send_Value[1]);
}







