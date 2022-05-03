#ifndef _RAMP_H
#define _RAMP_H
#include "stm32f4xx.h"
#include "stdbool.h"

typedef struct 
{
		bool accelerating,decelerating;
    float Data,Data_last,Cnt_BAP;
    float y,accCnt,k;
}RAMP_C;

extern float Ramp_Calculate(RAMP_C *p,float data);

#endif

