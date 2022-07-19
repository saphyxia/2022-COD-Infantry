#ifndef ASSIST_H
#define ASSIST_H
#include "main.h"

float ramp_Float(float now,float final,float ramp);
float encoder_To_Angle(volatile int16_t* encoder,float encoder_Max);
int16_t encoder_Extend(int16_t act_Encoder,int16_t torque_Ratio);
void angle_Extend(volatile float* result_Angle,float act_Angle);


#endif


