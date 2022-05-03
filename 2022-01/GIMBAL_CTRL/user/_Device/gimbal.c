#include "gimbal.h"
#include "assist.h"
#include "imu_Task.h"
#include "dbus_Task.h"
#include "gimbal_Task.h"


//YAW,PITCH

/*  2颗电机(YAW,PIT) 
    9种模式(关闭，遥控器机械，遥控器陀螺仪，键盘机械，键盘陀螺仪，自瞄态下打底面，打哨兵，打大小符)
    2层串级(速度环，角度环) PID有8个参数(P I D 误差限幅，积分限幅，比例输出限幅，积分输出限幅，总输出限幅)
*/Gimbal_Motor Gimbal[GIMBAL_MOTOR_NUM]=
{ 
    [YAW  ]={.CAN_Port=CAN2, .Data.ID=1, .state = GTURN_OFF, .get_Data=Gimbal_YAW_Data_Get, .ctrl_Calc=Gimbal_Ctrl_YAW },
    [PITCH]={.CAN_Port=CAN1, .Data.ID=2, .state = GTURN_OFF, .get_Data=Gimbal_PIT_Data_Get, .ctrl_Calc=Gimbal_Ctrl_PIT },
};

float Gimbal_PID_Param[GIMBAL_MOTOR_NUM][GIMBAL_STATE_NUM][CONCATENATION_NUM][PID_Param_NUM]={
    [YAW]={
        [GTURN_OFF ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [RC_MECH   ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [RC_GYRO   ]={          
            [SPEED]={          //速度环
//                70,//P
//                0.16f,//I
//                26,//D
								120,//P
                0.05f,//I
                0,//D
                1000,//Err_Max 
                10000,//Integral_MAX
                20000,//P_OUT_MAX
                4000,//I_OUT_MAX
                3000, //T_OUT_MAX
							  25000
            },
            [ANGLE]={          //角度环
//                70,//P
//                0,//I
//                26,//D
                80,//P
                0,//I
                0,//D
                20,//Err_Max 
                0,//Integral_MAX
                1000,//P_OUT_MAX
                0,//I_OUT_MAX
                150, //T_OUT_MAX
								1000
            }
        },
        [KB_MECH   ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [KB_GYRO   ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_STAN   ]={          
            [SPEED]={          //速度环
                120,//P
                0,//I
                0,//D
                120,//Err_Max 
                1500,//Integral_MAX
                26000,//P_OUT_MAX
                2500,//I_OUT_MAX
								0,
                26000 //T_OUT_MAX
            },
            [ANGLE]={          //角度环
                -60,//P
                0,//I
                0,//D
                30,//Err_Max 
                500,//Integral_MAX
                110,//P_OUT_MAX
                60,//I_OUT_MAX
								0,
                110 //T_OUT_MAX
            }
        },
        [AI_SENTRY  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_BUFF_M  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_BUFF_S  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
    },

    [PITCH]={
        [GTURN_OFF ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [RC_MECH   ]={          
            [SPEED]={          //速度环
                0,//P
                0,//I
                0,//D
                350,//Err_Max 
                50000,//Integral_MAX
                21000,//P_OUT_MAX
                10000,//I_OUT_MAX
								0,
                31000 //T_OUT_MAX
            },
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                40,//Err_Max 
                0,//Integral_MAX
                400,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                350 //T_OUT_MAX
            }
        },
        [RC_GYRO   ]={          
            [SPEED]={          //速度环
//                -40,//P
//                -0.32f,//I
//                0,//D
                100,//P
                0.14,//I
                0,//D
                200,//Err_Max 
                10000,//Integral_MAX
                16000,//P_OUT_MAX
                3000,//I_OUT_MAX
                2000, //T_OUT_MAX
								20000
            },
            [ANGLE]={          //角度环
//                60,//P
//                0,//I
//                100,//D
                80,//P
                0,//I
                4,//D
                20,//Err_Max 
                0,//Integral_MAX
                600,//P_OUT_MAX
                0,//I_OUT_MAX
                200, //T_OUT_MAX
								1000
            }
        },
        [KB_MECH   ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [KB_GYRO   ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_STAN   ]={          
            [SPEED]={          //速度环
                100,//P
                0,//I
                0,//D
                350,//Err_Max 
                2000,//Integral_MAX
                25000,//P_OUT_MAX
                1500,//I_OUT_MAX
								0,
                31000 //T_OUT_MAX
            },
            [ANGLE]={          //角度环
                60,//P
                0,//I
                0,//D
                40,//Err_Max 
                1000,//Integral_MAX
                400,//P_OUT_MAX
                30,//I_OUT_MAX
								0,
                350 //T_OUT_MAX
            }
        },
        [AI_SENTRY  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_BUFF_M  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
        [AI_BUFF_S  ]={          
            [SPEED]={          //速度环
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
            [ANGLE]={          //角度环
                0,//P
                0,//I
                0,//D
                0,//Err_Max 
                0,//Integral_MAX
                0,//P_OUT_MAX
                0,//I_OUT_MAX
								0,
                0 //T_OUT_MAX
            }
        },
    },
};



void Gimbal_Motor_PID_Param_Init(void)
{
//1级，速度环部分
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[GTURN_OFF].speed_Loop , Gimbal_PID_Param[YAW  ][GTURN_OFF][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[RC_MECH  ].speed_Loop , Gimbal_PID_Param[YAW  ][RC_MECH  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[RC_GYRO  ].speed_Loop , Gimbal_PID_Param[YAW  ][RC_GYRO  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[KB_MECH  ].speed_Loop , Gimbal_PID_Param[YAW  ][KB_MECH  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[KB_GYRO  ].speed_Loop , Gimbal_PID_Param[YAW  ][KB_GYRO  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_STAN  ].speed_Loop , Gimbal_PID_Param[YAW  ][AI_STAN  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_SENTRY].speed_Loop , Gimbal_PID_Param[YAW  ][AI_SENTRY][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_BUFF_M].speed_Loop , Gimbal_PID_Param[YAW  ][AI_BUFF_M][SPEED]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_BUFF_S].speed_Loop , Gimbal_PID_Param[YAW  ][AI_BUFF_S][SPEED]); 
 
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[GTURN_OFF].speed_Loop , Gimbal_PID_Param[PITCH][GTURN_OFF][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[RC_MECH  ].speed_Loop , Gimbal_PID_Param[PITCH][RC_MECH  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[RC_GYRO  ].speed_Loop , Gimbal_PID_Param[PITCH][RC_GYRO  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[KB_MECH  ].speed_Loop , Gimbal_PID_Param[PITCH][KB_MECH  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[KB_GYRO  ].speed_Loop , Gimbal_PID_Param[PITCH][KB_GYRO  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_STAN  ].speed_Loop , Gimbal_PID_Param[PITCH][AI_STAN  ][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_SENTRY].speed_Loop , Gimbal_PID_Param[PITCH][AI_SENTRY][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_BUFF_M].speed_Loop , Gimbal_PID_Param[PITCH][AI_BUFF_M][SPEED]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_BUFF_S].speed_Loop , Gimbal_PID_Param[PITCH][AI_BUFF_S][SPEED]); 

//2级，角度环部分
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[GTURN_OFF].angle_Loop , Gimbal_PID_Param[YAW  ][GTURN_OFF][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[RC_MECH  ].angle_Loop , Gimbal_PID_Param[YAW  ][RC_MECH  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[RC_GYRO  ].angle_Loop , Gimbal_PID_Param[YAW  ][RC_GYRO  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[KB_MECH  ].angle_Loop , Gimbal_PID_Param[YAW  ][KB_MECH  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[KB_GYRO  ].angle_Loop , Gimbal_PID_Param[YAW  ][KB_GYRO  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_STAN  ].angle_Loop , Gimbal_PID_Param[YAW  ][AI_STAN  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_SENTRY].angle_Loop , Gimbal_PID_Param[YAW  ][AI_SENTRY][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_BUFF_M].angle_Loop , Gimbal_PID_Param[YAW  ][AI_BUFF_M][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[YAW  ].Ctrl[AI_BUFF_S].angle_Loop , Gimbal_PID_Param[YAW  ][AI_BUFF_S][ANGLE]);
 
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[GTURN_OFF].angle_Loop , Gimbal_PID_Param[PITCH][GTURN_OFF][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[RC_MECH  ].angle_Loop , Gimbal_PID_Param[PITCH][RC_MECH  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[RC_GYRO  ].angle_Loop , Gimbal_PID_Param[PITCH][RC_GYRO  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[KB_MECH  ].angle_Loop , Gimbal_PID_Param[PITCH][KB_MECH  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[KB_GYRO  ].angle_Loop , Gimbal_PID_Param[PITCH][KB_GYRO  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_STAN  ].angle_Loop , Gimbal_PID_Param[PITCH][AI_STAN  ][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_SENTRY].angle_Loop , Gimbal_PID_Param[PITCH][AI_SENTRY][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_BUFF_M].angle_Loop , Gimbal_PID_Param[PITCH][AI_BUFF_M][ANGLE]);
    PID_Init_ByParamArray(&Gimbal[PITCH].Ctrl[AI_BUFF_S].angle_Loop , Gimbal_PID_Param[PITCH][AI_BUFF_S][ANGLE]);

}


/*
*/
static void Gimbal_YAW_Data_Get(CanRxMsg* RxMsg)
{
    if(RxMsg->StdId==Gimbal[YAW].Data.ID+0x204){
        Gimbal[YAW].Data.encoder = ((int16_t)RxMsg->Data[0]<<8 | (int16_t)RxMsg->Data[1]);
        Gimbal[YAW].Data.velocity= ((int16_t)RxMsg->Data[2]<<8 | (int16_t)RxMsg->Data[3]);
        Gimbal[YAW].Data.current = ((int16_t)RxMsg->Data[4]<<8 | (int16_t)RxMsg->Data[5]);
        Gimbal[YAW].Data.temperature = RxMsg->Data[6];
        angle_Extend(&Gimbal[YAW].Data.angle,encoder_To_Angle(&Gimbal[YAW].Data.encoder,8192));
#if GIMBAL_USE_IMU
        Gimbal[YAW].IMU.gyro = yaw_Gyro;
        Gimbal[YAW].IMU.angle= yaw_Angel;
#endif
    }
}

/*
*/
static void Gimbal_PIT_Data_Get(CanRxMsg* RxMsg)
{
    if(RxMsg->StdId==Gimbal[PITCH].Data.ID+0x204){
        Gimbal[PITCH].Data.encoder = ((int16_t)RxMsg->Data[0]<<8 | (int16_t)RxMsg->Data[1]);
        Gimbal[PITCH].Data.velocity= ((int16_t)RxMsg->Data[2]<<8 | (int16_t)RxMsg->Data[3]);
        Gimbal[PITCH].Data.current = ((int16_t)RxMsg->Data[4]<<8 | (int16_t)RxMsg->Data[5]);
        Gimbal[PITCH].Data.temperature = RxMsg->Data[6];
        Gimbal[PITCH].Data.angle=encoder_To_Angle(&Gimbal[PITCH].Data.encoder,8192);
#if GIMBAL_USE_IMU
        Gimbal[PITCH].IMU.gyro = pit_Gyro;
        Gimbal[PITCH].IMU.angle= pit_Angel;
#endif
    }
}


/*
*/
#if SEND_ENABLE
    CanTxMsg YAW_TxMsg;
	CanTxMsg PIT_TxMsg;
#endif
extern float yaw_cps;
float cps_ratio=1.5f,Cha_ratio=-0.3f;
static int16_t Gimbal_Ctrl_YAW(float err)
{
#if GIMBAL_USE_IMU
    int16_t res=(int16_t)Gimbal[YAW].Ctrl[Gimbal[YAW].state].speed_Loop.PID_Calc(&Gimbal[YAW].Ctrl[Gimbal[YAW].state].speed_Loop.middle,
                         Gimbal[YAW].Ctrl[Gimbal[YAW].state].angle_Loop.PID_Calc(&Gimbal[YAW].Ctrl[Gimbal[YAW].state].angle_Loop.middle,err)-yaw_Gyro+(!IF_VISION_ENABLE)*(yaw_cps*cps_ratio)-(rc_ctrl.rc.s[0]==2)*Chass_Gyro*Cha_ratio);
#endif

#if SEND_ENABLE
	YAW_TxMsg.StdId=Gimbal[YAW].Data.ID<5?0x1ff:0x2ff;
	YAW_TxMsg.IDE=CAN_ID_STD;
	YAW_TxMsg.RTR=CAN_RTR_DATA;
	YAW_TxMsg.DLC=0X08;
	
//    YAW_TxMsg.Data[0] = res>>8;
//    YAW_TxMsg.Data[1] = res;

//	CAN_Transmit(Gimbal[YAW].CAN_Port, &YAW_TxMsg);
#endif

    return res;
}

/*
*/
int16_t Gravity_cps=0;
static int16_t Gimbal_Ctrl_PIT(float err)
{
#if GIMBAL_USE_IMU
    int16_t res=(int16_t)Gimbal[PITCH].Ctrl[Gimbal[PITCH].state].speed_Loop.PID_Calc(&Gimbal[PITCH].Ctrl[Gimbal[PITCH].state].speed_Loop.middle,
                         Gimbal[PITCH].Ctrl[Gimbal[PITCH].state].angle_Loop.PID_Calc(&Gimbal[PITCH].Ctrl[Gimbal[PITCH].state].angle_Loop.middle,err)-pit_Gyro);
#endif

#if SEND_ENABLE
	PIT_TxMsg.StdId=Gimbal[PITCH].Data.ID<5?0x1ff:0x2ff;
	PIT_TxMsg.IDE=CAN_ID_STD;
	PIT_TxMsg.RTR=CAN_RTR_DATA;
	PIT_TxMsg.DLC=0X08;

	if(rol_Angel>1.f&&IMU_Working==1)
;//	 Gravity_cps = (-80.f*rol_Angel) + 3000.0f;
	else
		Gravity_cps=0;
	res=res+Gravity_cps;
	
//    PIT_TxMsg.Data[2] = res>>8;
//    PIT_TxMsg.Data[3] = res;

//	CAN_Transmit(Gimbal[PITCH].CAN_Port, &PIT_TxMsg);//往上是正电流
#endif
 
    return res;
}



