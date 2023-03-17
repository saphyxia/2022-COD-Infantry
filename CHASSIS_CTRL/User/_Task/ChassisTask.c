/* includes ------------------------------------------------------------------*/
#include "ChassisTask.h"
#include "referee_info.h"
#include "assist.h"
#include "usart.h"
#include "imu.h"
#include "UI.h"

/* Private variables ---------------------------------------------------------*/
//交互时的心跳计数
static TickType_t CHAS_InteracTick;
//底盘跟随角一阶卡尔曼
static extKalman_t KF_FOLO_Angle;

/* Exported variables --------------------------------------------------------*/
//底盘电机PID参数
float CHAS_PID_Param[STATE_NUM][PID_Param_NUM]={
	[BLOCKED]={0,},
	[READY]={0,},
	[RUNNING] = {
		13.f,//P
		0.5f,//I
		0.f,//D
		800,//Err_Max 
		4000,//Integral_MAX
		16000,//P_OUT_MAX
		1000,//I_OUT_MAX
		100,//D_OUT_MAX
		16000 //T_OUT_MAX	
	},
};
//底盘跟随PID参数
float CHAS_FOLO_Param[PID_TYPE][PID_Param_NUM]={
	[SPEED]={
		6.2f,//P
		0.f,//I
		0.f,//D
		200.f,//Err_Max 
		0.f,//Integral_MAX
		800.f,//P_OUT_MAX
		0.f,//I_OUT_MAX
		0.f,//D_OUT_MAX
		800.f,//T_OUT_MAX
	},
	[ANGLE]={
		6.4f,//P
		0.f,//I
		0.f,//D
		60.f,//Err_Max 
		0.f,//Integral_MAX
		300.f,//P_OUT_MAX
		0.f,//I_OUT_MAX
		0.f,//D_OUT_MAX
		300.f,//T_OUT_MAX
	},
};
//底盘信息
CHASSIS_INFO_T chassis={
	.state = BLOCKED,
	.mode = INVA,
	.midangle = 182.1f,
	.CANx = CAN2,
	.stdid = 0x300,
	.LF   = &Chassis_Motor[Left_Font],
	.LR   = &Chassis_Motor[Left_Rear],
	.RF   = &Chassis_Motor[Righ_Font],
	.RR   = &Chassis_Motor[Righ_Rear],
	.FOLO = &Chassis_Motor[YAW],
};

TickType_t CapTick;
bool IF_SUPERCAP_ONLINE = false;
/* Exported functions --------------------------------------------------------*/
/**
 *	@brief	底盘任务
*/
void ChassisTask(void *pvParameters) 
{
	TickType_t systick;
	//PID装载
	CHAS_Set_PID(&chassis.state);
	while(1)
	{
		//当前心跳计数
        systick = xTaskGetTickCount();
		//底盘状态切换
		CHAS_STATE_Switch(&CHAS_InteracTick);
		
		//数据装载
		switch(chassis.state)
		{
			case BLOCKED:
			//底盘卸力
				CHAS_Motor_Stop();
			break;
			
			case READY:
				chassis.state = RUNNING;
			//PID装载
				CHAS_Set_PID(&chassis.state);
			break;
			
			case RUNNING:
			//全向移动拟合
				Omnidirection_FITTING(&chassis);
			//底盘姿态控制
				CHAS_Posture_Ctrl();
			break;
			
			default:
			break;
		}
		vTaskDelayUntil(&systick,CHASSISTASK_TIM);
	}
}
/**
 *	@brief	获取底盘控制信息
*/
void get_Chas_INFO(CAN_TypeDef *CANx,CanRxMsg *RxMsg,CHASSIS_INFO_T *std)
{
	if(CANx != CAN2) return ;
	
	if(RxMsg->StdId == std->stdid)
	{
		//底盘模式刷新
		UI.User.SENTRY = (bool)((RxMsg->Data[0] & 8) >> 3);
		UI.User.COVER  = (bool)((RxMsg->Data[0] & 4) >> 2);
		UI.User.FRI    = (bool)((RxMsg->Data[0] & 2) >> 1);
		UI.User.AUTO   = (bool)( RxMsg->Data[0] & 1);
		std->mode      = (CHAS_MODE)RxMsg->Data[1];
		//控制参数刷新
		std->ch2 	= (int16_t) RxMsg->Data[2] << 8 | (int16_t) RxMsg->Data[3];
		std->ch3 	= (int16_t) RxMsg->Data[4] << 8 | (int16_t) RxMsg->Data[5];
		std->key.v 	= (uint16_t)RxMsg->Data[6] << 8 | (uint16_t)RxMsg->Data[7];
		CHAS_InteracTick = xTaskGetTickCount();
	}
}

/* Private functions ---------------------------------------------------------*/
/**
 *	@brief	底盘卸力
*/
static void CHAS_Motor_Stop(void)
{
	chassis.LF->pid_Speed.PID_Reset(&chassis.LF->pid_Speed.middle);
	chassis.LR->pid_Speed.PID_Reset(&chassis.LR->pid_Speed.middle);
	chassis.RF->pid_Speed.PID_Reset(&chassis.RF->pid_Speed.middle);
	chassis.RR->pid_Speed.PID_Reset(&chassis.RR->pid_Speed.middle);

	chassis.FOLO->pid_Speed.PID_Reset(&chassis.FOLO->pid_Speed.middle);
	chassis.FOLO->pid_Speed.PID_Reset(&chassis.FOLO->pid_Angle.middle);
	
	//CAN发送装载清空
	for(int i = 0; i < 8; i++)
	{
		CAN_TxMsg[_CAN1][_0x200].Data[i]=0;
	}
}
/**
 *	@brief	重置PID
*/
static void CHAS_Set_PID(DEVICE_STATE *state)
{
	PID_Init_ByParamArray(&chassis.LF->pid_Speed,CHAS_PID_Param[*state]);
	PID_Init_ByParamArray(&chassis.LR->pid_Speed,CHAS_PID_Param[*state]);
	PID_Init_ByParamArray(&chassis.RF->pid_Speed,CHAS_PID_Param[*state]);
	PID_Init_ByParamArray(&chassis.RR->pid_Speed,CHAS_PID_Param[*state]);
	
	PID_Init_ByParamArray(&chassis.FOLO->pid_Speed,CHAS_FOLO_Param[SPEED]);
	PID_Init_ByParamArray(&chassis.FOLO->pid_Angle,CHAS_FOLO_Param[ANGLE]);
	
	KalmanCreate(&KF_FOLO_Angle,1,20);
}
/**
 *	@brief	底盘状态切换
*/
static void CHAS_STATE_Switch(TickType_t *tick)
{
	//判段和云台的交互是否丢失
	if(*tick > (xTaskGetTickCount()-INTERAC_LOST_TIME))
	{
		if(chassis.state == BLOCKED)
		{
			chassis.state = READY;
		}
	}else
	{
		chassis.state = BLOCKED;
	}
	//电容交互离线判断
	if(CapTick > (xTaskGetTickCount()-INTERAC_LOST_TIME))
	{
		IF_SUPERCAP_ONLINE = true;
	}else
	{
		IF_SUPERCAP_ONLINE = false;
	}
}
/**
 *	@brief	键盘数据拟合
*/
static void Keyboard_Motion_FITTING(float *data)
{
	//按下最大值
	static float key_max = 660.f;
	//FR方向
	if(Key_W()== true)
		data[0] = RAMP_float( key_max,data[0],KEYSLOPE);
	else if(Key_S()== true)
		data[0] = RAMP_float(-key_max,data[0],KEYSLOPE);
	else 
		data[0] = 0;
	//LR方向
	if(Key_D()== true)
		data[1] = RAMP_float( key_max,data[1],KEYSLOPE);
	else if(Key_A()== true)
		data[1] = RAMP_float(-key_max,data[1],KEYSLOPE);
	else 
		data[1] = 0;
}
/**
 *	@brief	自旋速度装载
*/
static float Spin_velocity_Load(float *folo_angle)
{
	uint16_t power_limit = game_robot_state.chassis_power_limit;
	
	float res = 0;
	
	if(chassis.mode == FOLO)
	{
		res = chassis.FOLO->pid_Speed.PID_Calc(&chassis.FOLO->pid_Speed.middle,
			  chassis.FOLO->pid_Angle.PID_Calc(&chassis.FOLO->pid_Angle.middle,-(*folo_angle)) - MPU6050_Real_Data.Gyro_Z);
		
	}else if(chassis.mode == SPIN)
	{
		if(power_limit>=120) 	  res = SPIN_SPEED_120W;
		else if(power_limit>=100) res = SPIN_SPEED_100W;
		else if(power_limit>=90)  res = SPIN_SPEED_90W;
		else if(power_limit>=80)  res = SPIN_SPEED_80W;
		else if(power_limit>=70)  res = SPIN_SPEED_70W;
		else if(power_limit>=60)  res = SPIN_SPEED_60W;
		else if(power_limit>=50)  res = SPIN_SPEED_50W;
		else if(power_limit>=40)  res = SPIN_SPEED_40W;
		else res = SPIN_SPEED_40W; 

		if(IF_SUPERCAP_ONLINE) res = SPIN_SPEED_SUPER;
	}
	//死区控制
	if(ABS(res)<150) res = 0;
	
	return res;
}
/**
 *	@brief	全向移动拟合
*/
float keymotion[2]={0,};
static void Omnidirection_FITTING(CHASSIS_INFO_T *std)
{
	//跟随角度差
	float folo_angle = chassis.FOLO->Data.angle - chassis.midangle;
		  folo_angle = ABS(folo_angle) > 180 ? folo_angle-folo_angle/ABS(folo_angle)*360 : folo_angle;
	//横移解算三角函数
	float sin_folo = arm_sin_f32(folo_angle*2.f*3.1415926f/360.f);
	float cos_folo = arm_cos_f32(folo_angle*2.f*3.1415926f/360.f);
	//键盘数据拟合
	Keyboard_Motion_FITTING(keymotion);
	//控制数据装载
	float motion_FR = CHAS_CONTROL_GAIN * (std->ch3 + keymotion[0]);
	float motion_LR = CHAS_CONTROL_GAIN * (std->ch2 + keymotion[1]);

	//全向移动拟合
	std->target_FR   = sin_folo * motion_LR + cos_folo * motion_FR;
	std->target_LR   = cos_folo * motion_LR - sin_folo * motion_FR;
	std->target_GYRO = Spin_velocity_Load(&folo_angle);
	
	if(Key_CTRL()==true)
	{
		std->target_FR *= 6.4f;
		std->target_LR *= 1.8f;
		std->target_GYRO *= 1.8f;
	}
}
/**
 *	@brief	底盘功率控制
*/
static void CHAS_Power_Control(float *expect)
{
	float limit_gain = 0;
	float Chas_Input = ABS(expect[0])+ABS(expect[1])+ABS(expect[2])+ABS(expect[3]);
		
	if(game_robot_state.robot_id != 0)
	{
		limit_gain = power_heat_data.chassis_power_buffer /60.f;
		VAL_LIMIT(limit_gain,0.f,1.f);
		if(power_heat_data.chassis_power_buffer < 60.f)
		{
			if(Key_CTRL() == true)
			{
				limit_gain = sqrtf(limit_gain);
			}

			if(power_heat_data.chassis_power_buffer < 18.f)
			{
				limit_gain = limit_gain * limit_gain;
			}

			for(int i = 0; i < 4; i++)
			{
				expect[i] *= limit_gain; 
			}
		}
	}else{
		for(int i = 0; i < 4; i++)
		{
			expect[i] *= 0.6f; 
		}
	}
}
/**
 *	@brief	底盘姿态控制
*/
static void CHAS_Posture_Ctrl(void)
{
	float Omni_Motion[4]={0.f,};
	int16_t output[4]={0.f,};
	//变速曲线
	Chassis_VariableCurve(&chassis.target_LR,&chassis.target_FR);
	//全向移动解算
	Omni_Motion[Left_Font] = 1.0f / M_R * ( chassis.target_LR + chassis.target_FR + (M_L + M_LW)/ 2.f * chassis.target_GYRO);
	Omni_Motion[Righ_Font] = 1.0f / M_R * ( chassis.target_LR - chassis.target_FR + (M_L + M_LW)/ 2.f * chassis.target_GYRO);
	Omni_Motion[Left_Rear] = 1.0f / M_R * (-chassis.target_LR + chassis.target_FR + (M_L + M_LW)/ 2.f * chassis.target_GYRO);
	Omni_Motion[Righ_Rear] = 1.0f / M_R * (-chassis.target_LR - chassis.target_FR + (M_L + M_LW)/ 2.f * chassis.target_GYRO);
	//功率控制
	CHAS_Power_Control(Omni_Motion);
	//PID计算
	output[Left_Font] = chassis.LF->pid_Speed.PID_Calc(&chassis.LF->pid_Speed.middle,Omni_Motion[Left_Font] - chassis.LF->Data.velocity);
	output[Righ_Font] = chassis.RF->pid_Speed.PID_Calc(&chassis.RF->pid_Speed.middle,Omni_Motion[Righ_Font] - chassis.RF->Data.velocity);
	output[Left_Rear] = chassis.LR->pid_Speed.PID_Calc(&chassis.LR->pid_Speed.middle,Omni_Motion[Left_Rear] - chassis.LR->Data.velocity);
	output[Righ_Rear] = chassis.RR->pid_Speed.PID_Calc(&chassis.RR->pid_Speed.middle,Omni_Motion[Righ_Rear] - chassis.RR->Data.velocity);
	//CAN发送装载
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Left_Font].Data.StdId-0x201]] = (uint8_t)(output[Left_Font]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Left_Font].Data.StdId-0x201]] = (uint8_t)(output[Left_Font]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Righ_Font].Data.StdId-0x201]] = (uint8_t)(output[Righ_Font]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Righ_Font].Data.StdId-0x201]] = (uint8_t)(output[Righ_Font]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Left_Rear].Data.StdId-0x201]] = (uint8_t)(output[Left_Rear]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Left_Rear].Data.StdId-0x201]] = (uint8_t)(output[Left_Rear]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Righ_Rear].Data.StdId-0x201]] = (uint8_t)(output[Righ_Rear]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Righ_Rear].Data.StdId-0x201]] = (uint8_t)(output[Righ_Rear]);
}
/**
 *	@brief	底盘变速曲线
 */
static void Chassis_VariableCurve(float *LR,float *FR)
{
	static float speed_line = 0.f;
	static float speed_linepre=0.f;//上一次的线速度
	static bool  accelerating = false,decelerating = false;//速度增减情况
	static float accCnt=0.f;//自增计时
	static float curve_gain = 0;//曲线增益
	static float acck = 2.5f,deck = 20;//加速、减速曲线系数
	static float deceleRecode_Y=0.f,deceleRecode_X=0.f;//减速曲线缓存速度
	
	/*线速度计算*/
	speed_line = sqrt(powf(*LR,2.f)+powf(*FR,2.f));

	/*加减速判断*/
	if((ABS(speed_line) - ABS(speed_linepre)) > 1000)
	{
		accelerating = 1;
		decelerating = 0;
		accCnt = 0;
	}
	else if((ABS(speed_line) - ABS(speed_linepre)) < -1000)
	{
		accelerating = 0;
		decelerating = 1;
		accCnt = 0;
	}

	/*加速曲线*/
	if(accelerating == 1)
	{
		accCnt += 0.004f;
		curve_gain = AcclerateCurve(accCnt,acck);
		if (curve_gain > 0.999f)
        {
            accelerating = 0;
        }
	}
	else if(decelerating != 1)
    {
        curve_gain = 1;
        accCnt = 0;
    }

	/*减速曲线*/
	if(decelerating == 1)
	{
		accCnt += 0.004f;
		curve_gain = DecclerateCurve(accCnt,deck);
		if (curve_gain < 0.01f)
        {
            decelerating = 0;
        }
	}
	else if(accelerating != 1)
    {
        curve_gain = 1;
        accCnt = 0;
    }

	/*增益计算*/
	if(accelerating == 1)
	{
		*FR *= curve_gain;
		*LR *= curve_gain;
	}
	else if(decelerating == 1)
	{
		*FR = deceleRecode_Y * curve_gain;
		*LR = deceleRecode_X * curve_gain;
	}
	if(decelerating != 1)
	{
		deceleRecode_Y = *FR;
		deceleRecode_X = *LR;
	}

	/*更新上一次的线速度*/
	speed_linepre = speed_line;
}

/**
 * @description: 公式原型 y = 1/(1+e^(-k(x-2/k)))  当k = 4.2 , x = 1时 y = 0.9
 */
static float AcclerateCurve(float x , float k)
{
	float y;
	k = 4.2f / k;
	if(k == 0)
	{
		return 1;
	}
	y = 1/(1+powf(NATURAL_NUMBER,(-k*(x-2/k))));
	return y;
}

/**
 * @description: 公式原型 y =1/(1+e^(-k(-x+3/k)))
  当k = 10 , x = 0.8时 y = 0.01
 */
static float DecclerateCurve(float x , float k)
{
	float y;
	if(k == 0)
	{
		return 1;
	}
	y = 1/(1+powf(NATURAL_NUMBER,(-k*(-x+3/k))));
	return y;
}

