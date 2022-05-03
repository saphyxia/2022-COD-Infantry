#include "motor.h"
#include "imu.h"


/*  4颗电机(M3508x4) 
    三种模式(关闭，遥控器，键盘)
    没有串级PID，
*/
DJI_M3508_t Chassis[Chassis_Motor_NUM]={[Left_Font].Data.ID=0x201, [Left_Font].state=TURN_OFF, [Left_Font].speed_Ctrl=DJI_Motor_Speed_Ctrl ,
                                        [Righ_Font].Data.ID=0x202, [Righ_Font].state=TURN_OFF, [Righ_Font].speed_Ctrl=DJI_Motor_Speed_Ctrl ,
                                        [Left_Rear].Data.ID=0x203, [Left_Rear].state=TURN_OFF, [Left_Rear].speed_Ctrl=DJI_Motor_Speed_Ctrl ,
                                        [Righ_Rear].Data.ID=0x204, [Righ_Rear].state=TURN_OFF, [Righ_Rear].speed_Ctrl=DJI_Motor_Speed_Ctrl};

float Chassis_PID_Param[CTRL_STATE_NUM][CHASSIS_PID_STATE_NUM][PID_Param_NUM]={
    [SPEED]={
				[TURN_OFF]={
					0,//P
					0,//I
					0,//D
					0,//Err_Max 
					0,//Integral_MAX
					0,//P_OUT_MAX
					0,//I_OUT_MAX
					0,//D_OUT_MAX
					0 //T_OUT_MAX
				},
				[REMOTE]={
						8.7f,//P
						0.3f,//I
						0.5f,//D
						3000,//Err_Max 
						2000,//Integral_MAX
						10000,//P_OUT_MAX
						2000,//I_OUT_MAX
						300,//D_OUT_MAX
						16300 //T_OUT_MAX
				},
				[KEYBORD]={
						0,//P
						0,//I
						0,//D
						0,//Err_Max 
						0,//Integral_MAX
						0,//P_OUT_MAX
						0,//I_OUT_MAX
						0,//D_OUT_MAX
						0 //T_OUT_MAX
				}
		}
};

//地盘跟随云台解算
W_TEMP_T W_Temp_PID={.gyro_Ctrl=GYRO_Gyro_Ctrl};

float W_Temp_PID_Param[CTRL_STATE_NUM][PID_Param_NUM]={
		[SPEED]={
			1.4f,//P
			0.f,//I
			-0.8f,//D
			1000.f,//Err_Max 
			600.f,//Integral_MAX
			1000.f,//P_OUT_MAX
			800.f,//I_OUT_MAX
			160,//D_OUT_MAX
			1200.f //T_OUT_MAX
		},
		[ANGLE]={
			3.4f,//P
			0.f,//I
			-1.4f,//D
			400.f,//Err_Max 
			0.f,//Integral_MAX
			800.f,//P_OUT_MAX
			0.f,//I_OUT_MAX
			180,//D_OUT_MAX
			1200.f //T_OUT_MAX
		}
};

//功率控制
POWER_CONTROL_T POWER_Control_PID[Chassis_Motor_NUM]={[Left_Font].Current_updata_Ctrl=Current_Update_Ctrl,
                                                      [Righ_Font].Current_updata_Ctrl=Current_Update_Ctrl,
                                                      [Left_Rear].Current_updata_Ctrl=Current_Update_Ctrl,
                                                      [Righ_Rear].Current_updata_Ctrl=Current_Update_Ctrl};

float Power_PID_Param[PID_Param_NUM]={
    0.5f,//P
    0.01f,//I
    0.f,//D
    6000,//Err_Max 
    1000,//Integral_MAX
    6000,//P_OUT_MAX
    1000,//I_OUT_MAX
		0,//D_OUT_MAX
    3000 //T_OUT_MAX
};

/*  电机PID参数初始化函数，只调用一遍
    使用PID库中的外接函数PID_Init_ByParamArray进行。
*/
void Motor_PID_Param_Init(void)
{
    PID_Init_ByParamArray(&Chassis[Left_Font].pid[TURN_OFF],Chassis_PID_Param[SPEED][TURN_OFF]);
    PID_Init_ByParamArray(&Chassis[Left_Font].pid[REMOTE]  ,Chassis_PID_Param[SPEED][REMOTE]  );
    PID_Init_ByParamArray(&Chassis[Left_Font].pid[KEYBORD] ,Chassis_PID_Param[SPEED][KEYBORD] );

    PID_Init_ByParamArray(&Chassis[Righ_Font].pid[TURN_OFF],Chassis_PID_Param[SPEED][TURN_OFF]);
    PID_Init_ByParamArray(&Chassis[Righ_Font].pid[REMOTE]  ,Chassis_PID_Param[SPEED][REMOTE]  );
    PID_Init_ByParamArray(&Chassis[Righ_Font].pid[KEYBORD] ,Chassis_PID_Param[SPEED][KEYBORD] );

    PID_Init_ByParamArray(&Chassis[Left_Rear].pid[TURN_OFF],Chassis_PID_Param[SPEED][TURN_OFF]);
    PID_Init_ByParamArray(&Chassis[Left_Rear].pid[REMOTE]  ,Chassis_PID_Param[SPEED][REMOTE]  );
    PID_Init_ByParamArray(&Chassis[Left_Rear].pid[KEYBORD] ,Chassis_PID_Param[SPEED][KEYBORD] );

    PID_Init_ByParamArray(&Chassis[Righ_Rear].pid[TURN_OFF],Chassis_PID_Param[SPEED][TURN_OFF]);
    PID_Init_ByParamArray(&Chassis[Righ_Rear].pid[REMOTE]  ,Chassis_PID_Param[SPEED][REMOTE]  );
    PID_Init_ByParamArray(&Chassis[Righ_Rear].pid[KEYBORD] ,Chassis_PID_Param[SPEED][KEYBORD] );

	  PID_Init_ByParamArray(&W_Temp_PID.pid.speed_Loop,W_Temp_PID_Param[SPEED]);
    PID_Init_ByParamArray(&W_Temp_PID.pid.angle_Loop,W_Temp_PID_Param[ANGLE]);

    PID_Init_ByParamArray(&POWER_Control_PID[Left_Font].pid,Power_PID_Param);
    PID_Init_ByParamArray(&POWER_Control_PID[Righ_Font].pid,Power_PID_Param);
    PID_Init_ByParamArray(&POWER_Control_PID[Left_Rear].pid,Power_PID_Param);
    PID_Init_ByParamArray(&POWER_Control_PID[Righ_Rear].pid,Power_PID_Param);
}
/*
*/
static int16_t DJI_Motor_Speed_Ctrl(Motor_Type type,uint8_t tate,float err)
{
    return (int16_t)Chassis[type].pid[tate].PID_Calc(&Chassis[type].pid[tate].middle,err);
}
/*
*/
static int16_t GYRO_Gyro_Ctrl(GYRO_DATA_T *p,int16_t yaw_err)
{		
	float Yaw_Err=ABS(yaw_err)>4096 ? (yaw_err-yaw_err/ABS(yaw_err)*8192):yaw_err;
	
   return W_Temp_PID.pid.speed_Loop.PID_Calc(&W_Temp_PID.pid.speed_Loop.middle ,
		 W_Temp_PID.pid.angle_Loop.PID_Calc(&W_Temp_PID.pid.angle_Loop.middle,Yaw_Err)-MPU6050_Real_Data.Gyro_Z);
}
/*
*/
static int16_t Current_Update_Ctrl(Motor_Type type,float err)
{
  return (int16_t)POWER_Control_PID[type].pid.PID_Calc(&POWER_Control_PID[type].pid.middle,err);
}

