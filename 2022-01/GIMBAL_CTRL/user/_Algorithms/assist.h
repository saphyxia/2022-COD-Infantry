#ifndef ASSIST_H
#define ASSIST_H
#include "main.h"

float ramp_Float(float now,float final,float ramp);
float encoder_To_Angle(volatile int16_t* encoder,float encoder_Max);
void encoder_Extend(volatile int16_t* result_Encoder,int16_t act_Encoder,int16_t torque_Ratio);
void angle_Extend(volatile float* result_Angle,float act_Angle);
typedef struct 
{

	float LastAngle;       //上一次读取的角度
	float CurAngle;	//当前读取的角度
	double AngleSum;	//角度累加值
	
}Critical_t;
float Gimbal_Yaw_Gryo_AngleSum(Critical_t *critical, float get);
float Trigger_angle_AngleSum(Critical_t *critical, float get);
#endif


