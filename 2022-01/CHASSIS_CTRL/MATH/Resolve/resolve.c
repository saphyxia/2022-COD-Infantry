#include "resolve.h"
#include "pid.h"

/**
  * @name   encoder_Extend
  * @brief  编码器码盘拓展，
  * @param
  *         result_Encoder:拓展后的编码器（结果）      act_Encoder:实际编码器的值（参数1）      torque_Ratio:拓展比例
  * @retval none
  * @attention
  *         如果需要上电时编码器值归零，把起始angle_Last赋值为debug看到的数即可。
*/
void encoder_Extend(volatile int16_t* result_Encoder,int16_t act_Encoder,int16_t torque_Ratio)
{
    static int16_t angle_Last=0;
    int res1,res2;
    if(act_Encoder<angle_Last){
        res1=act_Encoder-angle_Last+8192;
        res2=act_Encoder-angle_Last;
    }else{
        res1=act_Encoder-angle_Last-8192;
        res2=act_Encoder-angle_Last;
    }
    angle_Last=act_Encoder;
    ABS(res1)<ABS(res2) ? (*result_Encoder+=res1) : (*result_Encoder+=res2);
    if(ABS(*result_Encoder)>8192*torque_Ratio) *result_Encoder=0;
}



