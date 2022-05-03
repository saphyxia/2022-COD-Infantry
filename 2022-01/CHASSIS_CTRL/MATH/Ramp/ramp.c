#include "ramp.h"
#include "pid.h"


/**
  * @name   Ramp_Calculate
  * @brief  斜坡曲线计算
  * @param  *p:斜坡曲线结构体       data:输入数据 
  * @retval none
  * @attention none
  */
float Ramp_Calculate(RAMP_C *p,float data)
{
    p->Data = data;

    if(ABS(p->Data)-ABS(p->Data_last)>1000)
    {
        p->accelerating=1;
        p->decelerating=0;
        p->accCnt=0;
        p->k=4.2f;
    }
    else if(ABS(p->Data)-ABS(p->Data_last)<-1000)
    {
        p->accelerating=0;
        p->decelerating=1;
        p->accCnt=0;
        p->k=20;
    }

    if(p->accelerating==1)
    {
        p->accCnt += 0.005f;
        p->y=1/(1+exp((-p->k*(p->accCnt-2/p->k))));
        if(p->y>0.999f)
        {
            p->accelerating=0;
        }
    }
    else if(p->decelerating==1)
    {
        p->accCnt += 0.005f;
        p->y=1/(1+exp((-p->k*(-p->accCnt+3/p->k))));
        if(p->y<0.01f)
        {
            p->decelerating=0;
        }
    }

    if(p->accelerating!=1&&p->decelerating!=1)
    {
        p->accCnt=0;
        p->y=0;
    }

    p->Data_last=p->Data;

    if(p->y>0){return p->y*p->Data;}
    else{return p->Data;}
}


