#ifndef ASSIST_H
#define ASSIST_H
#include "main.h"

float ramp_Float(float now,float final,float ramp);
float encoder_To_Angle(volatile int16_t* encoder,float encoder_Max);
void encoder_Extend(volatile int16_t* result_Encoder,int16_t act_Encoder,int16_t torque_Ratio);
void angle_Extend(volatile float* result_Angle,float act_Angle);
typedef struct 
{

	float LastAngle;       //��һ�ζ�ȡ�ĽǶ�
	float CurAngle;	//��ǰ��ȡ�ĽǶ�
	double AngleSum;	//�Ƕ��ۼ�ֵ
	
}Critical_t;
float Gimbal_Yaw_Gryo_AngleSum(Critical_t *critical, float get);
float Trigger_angle_AngleSum(Critical_t *critical, float get);
#endif


