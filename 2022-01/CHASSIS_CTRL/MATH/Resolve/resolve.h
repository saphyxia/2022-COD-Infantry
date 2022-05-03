#ifndef _RESOLVE_H
#define _RESOLVE_H
#include "stm32f4xx.h"
#include "pid.h"


void encoder_Extend(volatile int16_t* result_Encoder,int16_t act_Encoder,int16_t torque_Ratio);
#endif

