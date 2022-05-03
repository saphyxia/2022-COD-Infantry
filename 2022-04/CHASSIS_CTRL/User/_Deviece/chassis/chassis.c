#include "chassis.h"
#include "arm_math.h"
#include "referee_info.h"
#include "imu.h"
#include "usart.h"


float CHAS_PID_Param[2][CTRL_MODE_Cnt][PID_Param_NUM]={
	[DEV_ON]={
		[RC_CTRL]={
			13.f,//P
			0.5f,//I
			0.f,//D
			800,//Err_Max 
			4000,//Integral_MAX
			14000,//P_OUT_MAX
			1000,//I_OUT_MAX
			100,//D_OUT_MAX
			14000 //T_OUT_MAX
		},
		[KEY_CTRL]={
			13.f,//P
			0.5f,//I
			0.f,//D
			800,//Err_Max 
			4000,//Integral_MAX
			14000,//P_OUT_MAX
			1000,//I_OUT_MAX
			100,//D_OUT_MAX
			14000 //T_OUT_MAX
		},
	},
};

float CHAS_FOLO_Param[2][PID_TYPE][PID_Param_NUM]={
	[DEV_ON]={
		[SPEED]={
			7.8f,//P
			0.f,//I
			0.f,//D
			500.f,//Err_Max 
			0.f,//Integral_MAX
			1800.f,//P_OUT_MAX
			0.f,//I_OUT_MAX
			0.f,//D_OUT_MAX
			1800.f,//T_OUT_MAX
		},
		[ANGLE]={
			6.4f,//P
			0.f,//I
			0.f,//D
			60.f,//Err_Max 
			0.f,//Integral_MAX
			500.f,//P_OUT_MAX
			0.f,//I_OUT_MAX
			0.f,//D_OUT_MAX
			500.f,//T_OUT_MAX
		},
	},
};

/**
 *	@brief	???????
 */
float Speed_X_deliver(float speed_X , float speed_Y)
{
	float angle =  Chassis_Motor[YAW].Data.angle - Chassis.midangle ;
				angle = ABS(angle) > 180 ? angle-angle/ABS(angle)*360 :angle;
	
	float sin_yaw = arm_sin_f32(angle*2*3.1415926f/360.f);
	float cos_yaw = arm_cos_f32(angle*2*3.1415926f/360.f);
	
  return (cos_yaw * speed_X - sin_yaw * speed_Y);
}
float speed_Y_deliver(float speed_X , float speed_Y)
{
	float angle =  Chassis_Motor[YAW].Data.angle - Chassis.midangle ;
				angle = ABS(angle) > 180 ? angle-angle/ABS(angle)*360 :angle;
	
	float sin_yaw = arm_sin_f32(angle*2*3.1415926f/360.f);
	float cos_yaw = arm_cos_f32(angle*2*3.1415926f/360.f);
	
  return (sin_yaw * speed_X + cos_yaw * speed_Y);
}
/**
 *	@brief	??????
 */
static float KEY_Slope_deliver(CHASSIS_SPEED_TYPE speed_type)
{
	float slope = 0.f;
	uint16_t power_limit = game_robot_state.chassis_power_limit;


	if(speed_type!=_X && speed_type!=_Y)
	{
		return 0.f;
	}
	
	else if(speed_type == _X)
	{
		if(power_limit >=120) slope = KeySlope_X_120W;
		else if(power_limit >=100) slope = KeySlope_X_100W;
		else if(power_limit >= 80) slope = KeySlope_X_80W;
		else if(power_limit >= 60) slope = KeySlope_X_60W;
		else slope = KeySlope_X_40W;
		
	}else if(speed_type == _Y){
		if(power_limit >=120) slope = KeySlope_Y_120W;
		else if(power_limit >=100) slope = KeySlope_Y_100W;
		else if(power_limit >= 80) slope = KeySlope_Y_80W;
		else if(power_limit >= 60) slope = KeySlope_Y_60W;
		else slope = KeySlope_Y_40W;
		
	}

	return slope;
}
float slope_x = 0.f,slope_y = 0.f;
float KEY_X_Ctrl(void)
{
	float slope = KEY_Slope_deliver(_X);
	static float res_X = 0.f,KEY_MAX = 660.f;

	if(Key_A==1)
	{
		res_X = RAMP_float(-KEY_MAX,res_X,slope);
	}else if(Key_D==1){
		res_X = RAMP_float(KEY_MAX,res_X,slope);
	}else{
		res_X = 0;
	}

	return res_X;
}
float KEY_Y_Ctrl(void)
{
	float slope = KEY_Slope_deliver(_Y);;
	static float res_Y = 0.f,KEY_MAX = 660.f;

	if(Key_W==1)
	{
		res_Y = RAMP_float(KEY_MAX,res_Y,slope);
	}else if(Key_S==1){
		res_Y = RAMP_float(-KEY_MAX,res_Y,slope);
	}else{
		res_Y = 0;
	}
	
	return res_Y;
}
/**
 * @brief ??????
 * @param 
 */
float Chas_Input = 0.f;
float limitk = 0.f;

static void Power_Limit(float *expect,int16_t *sendData)
{
	float output[4]={0.f,};
	
	Chas_Input = ABS(expect[Left_Font]) \
						 + ABS(expect[Righ_Font]) \
						 + ABS(expect[Left_Rear]) \
						 + ABS(expect[Righ_Rear]) ;
	
	if(game_robot_state.robot_id != 0)
	{
		
		limitk = power_heat_data.chassis_power_buffer /60.f;
		
		VAL_LIMIT(limitk,0.f,1.f);
		
		if(power_heat_data.chassis_power_buffer < 18.f)
		{
			limitk = limitk *limitk ;
		}
		if(power_heat_data.chassis_power_buffer < 60.f)
		{
//			for(int i = 0; i < 4; i++)
//			{
//				expect[i] *= limitk; 
//			}
		}
	}
	if(Cap_V <= 14.f && Chas_Input >= 16000.f)
	{
//		for(int i = 0; i < 4; i++)
//		{
//			expect[i] = expect[i] / Chas_Input * 16000.f;
//		}	
	}
	for(int i = 0; i < YAW; i++)
	{
		output[i] = Chassis_Motor[i].pid_Speed.PID_Calc(
											 &Chassis_Motor[i].pid_Speed.middle,
											 (expect[i]-Chassis_Motor[i].Data.velocity));
	}

	for(int i = 0; i < 4; i++)
	{
		sendData[i] = output[i]; 
	}
}
/**
 * @brief CHAS????
 * @param 
 */
	int16_t CAN1_SendData[4]={0.f,};
	float Expect[4]={0.f,};
void CHASSIS_Handler(float speed[SPEED_TYPE])
{

	/*???????*/
	Expect[Left_Font] = 1.0f/M_R*( speed[_X]+speed[_Y]+(M_L+M_LW)*speed[_W]/2);
	Expect[Righ_Font] = 1.0f/M_R*( speed[_X]-speed[_Y]+(M_L+M_LW)*speed[_W]/2);
	Expect[Left_Rear] = 1.0f/M_R*(-speed[_X]+speed[_Y]+(M_L+M_LW)*speed[_W]/2);
	Expect[Righ_Rear] = 1.0f/M_R*(-speed[_X]-speed[_Y]+(M_L+M_LW)*speed[_W]/2);
	
	Power_Limit(Expect,CAN1_SendData);

	//????
	//CAN????
	//L[shoot.wheel_L->Data.StdId-0x201]
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Left_Font].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Left_Font]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Left_Font].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Left_Font]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Righ_Font].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Righ_Font]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Righ_Font].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Righ_Font]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Left_Rear].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Left_Rear]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Left_Rear].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Left_Rear]);
	CAN_TxMsg[_CAN1][_0x200].Data[H[Chassis_Motor[Righ_Rear].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Righ_Rear]>>8);
	CAN_TxMsg[_CAN1][_0x200].Data[L[Chassis_Motor[Righ_Rear].Data.StdId-0x201]] = (uint8_t)(CAN1_SendData[Righ_Rear]);
}

/**
 *	@brief	??PID??
 */
void CHASSIS_Set_PID(CHASSIS_CTRL_MODE ctrl_mode)
{
	for(int i = 0; i < YAW; i++)
	{
		PID_Init_ByParamArray(&Chassis_Motor[i].pid_Speed,CHAS_PID_Param[DEV_ON][ctrl_mode]);
	}
	PID_Init_ByParamArray(&Chassis_Motor[YAW].pid_Speed,CHAS_FOLO_Param[DEV_ON][SPEED]);
	PID_Init_ByParamArray(&Chassis_Motor[YAW].pid_Angle,CHAS_FOLO_Param[DEV_ON][ANGLE]);
	KalmanCreate(&Chassis.KF_FOLO_Angle,1,20);
}

/**
 *	@brief	??????
 */
int SPIN_Speed=600;
float Speed_W_deliver(CHASSIS_ACT_MODE mode)
{
	float res=0.f;
	uint16_t power_limit = game_robot_state.chassis_power_limit;
	

	if(mode == FOLO)//????
	{
		float angle = Chassis.midangle - Chassis_Motor[YAW].Data.angle ;
					angle = ABS(angle) > 180 ? angle-angle/ABS(angle)*360 :angle;
		float KF_angle_err = KalmanFilter(&Chassis.KF_FOLO_Angle,angle);
		float speed_err = Chassis_Motor[YAW].pid_Angle.PID_Calc(&Chassis_Motor[YAW].pid_Angle.middle,KF_angle_err) \
											-MPU6050_Real_Data.Gyro_Z;//- Chassis_Motor[YAW].Data.velocity;

		res = Chassis_Motor[YAW].pid_Speed.PID_Calc(&Chassis_Motor[YAW].pid_Speed.middle,speed_err);
		VAL_LIMIT(res,-power_limit*9.f,power_limit*9.f);
	}
	else if(mode == SPIN)//???
	{
		if(power_limit>=120) 		  res = SPIN_SPEED_120W;
		else if(power_limit>=100) res = SPIN_SPEED_100W;
		else if(power_limit>=90)  res = SPIN_SPEED_90W;
		else if(power_limit>=80)  res = SPIN_SPEED_80W;
		else if(power_limit>=70)  res = SPIN_SPEED_70W;
		else if(power_limit>=60)  res = SPIN_SPEED_60W;
		else if(power_limit>=50)  res = SPIN_SPEED_50W;
		else if(power_limit>=40)	res = SPIN_SPEED_40W;
		else res = SPIN_SPEED_40W; 
		
		res= SPIN_Speed;
	}
	
	if(ABS(res) <= 50) res = 0;
	
	return res;
}

/**
 *	@brief	??,????
 */
void CHASSIS_Stop(void)
{
	//??pid??
	for(int i = 0; i < YAW; i++)
	{
		Chassis_Motor[i].pid_Speed.PID_Reset(&Chassis_Motor[i].pid_Speed.middle);
	}
	
	Chassis_Motor[YAW].pid_Speed.PID_Reset(&Chassis_Motor[YAW].pid_Speed.middle);
	Chassis_Motor[YAW].pid_Angle.PID_Reset(&Chassis_Motor[YAW].pid_Angle.middle);
	//CAN????
	for(int i = 0; i < 8; i++)
	{
		CAN_TxMsg[_CAN1][_0x200].Data[i]=0;//???CAN.C???CAN??
	}
}

/**
 *	@brief	
 */
float RAMP_float( float final, float now, float ramp )
{
	float buffer = 0.f;

	buffer = final - now;

	if (buffer > 0){
		if (buffer > ramp){  
			now += ramp;
		}else{
			now += buffer;
		}
	}else{
		if (buffer < -ramp){
			now -= ramp;
		}else{
			now -= buffer;
		}
	}
	return now;
}

