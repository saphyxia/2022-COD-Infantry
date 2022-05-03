#include "assist.h"


/**
  * @name   encoder_To_Angle
  * @brief  角度转换
  * @param
  *         encoder:编码器角度        encoder_Max:编码器最大值（量程）
  * @retval result_Angle:周角角度
  * @attention
*/
float encoder_To_Angle(volatile int16_t* encoder,float encoder_Max)
{
    return *encoder/encoder_Max*360.0f;
}


/**
  * @name   RAMP_float
  * @brief  电机信息保存结构体
  * @param
  *         final:最终值		now:当前值		ramp:变化量
  * @retval none
  * @attention none
*/
float ramp_Float(float now,float final,float ramp)
{
    float buffer = 0;

    buffer = final - now;
    if (buffer > 0){
        if (buffer > ramp){  now += ramp;}   
        else{now += buffer;}
    }else{
        if (buffer < -ramp){now += -ramp;}
        else{now += buffer;}
    }

    return now;
}


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


/**
  * @name   angle_Extend
  * @brief  角度拓展
  * @param
  *         act_Angle:实际角度，通常为陀螺仪返回得到，也可以是用编码器换算。
  * @retval result_Angle:无穷累加的结果（周角角度）
  * @attention
  *         针对正负180临界点类型进行处理
*/
void angle_Extend(volatile float* result_Angle,float act_Angle)
{
    static float angle_Last=0;
    float res1,res2;
    if(act_Angle<angle_Last){
        res1=act_Angle-angle_Last+360.0f;
        res2=act_Angle-angle_Last;
    }else{
        res1=act_Angle-angle_Last-360.0f;
        res2=act_Angle-angle_Last;
    }
    angle_Last=act_Angle;
    if(ABS(res1)<0.001f||ABS(res2)<0.001f)res1=res2=0;
    ABS(res1)<ABS(res2) ? (*result_Angle+=res1) : (*result_Angle+=res2);
}

float Gimbal_Yaw_Gryo_AngleSum(Critical_t *critical, float get)
{
	critical->CurAngle = get - critical->LastAngle;	//当前陀螺仪角度减去上一次读取的陀螺仪角度，作为你的参考反馈值
	critical->LastAngle = get;
	/*  临界处理，保证每次经过零点反馈角度都是连续的  */
	if(critical->CurAngle <= -180)		//注意此处是陀螺仪角度放大后的临界值
		critical->CurAngle += 360;
	if(critical->CurAngle >= 180)
		critical->CurAngle -= 360;
	critical->AngleSum += critical->CurAngle;	

	return critical->AngleSum;				//返回新的反馈角度，作为PID算法的反馈值
}

float Trigger_angle_AngleSum(Critical_t *critical, float get)
{
	critical->CurAngle = get - critical->LastAngle;	//当前陀螺仪角度减去上一次读取的陀螺仪角度，作为你的参考反馈值
	critical->LastAngle = get;
	/*  临界处理，保证每次经过零点反馈角度都是连续的  */
	if(critical->CurAngle <= -4096)		//注意此处是陀螺仪角度放大后的临界值
		critical->CurAngle += 8192;
	if(critical->CurAngle >= 4096)
		critical->CurAngle -= 8192;
	critical->AngleSum += critical->CurAngle/27.f;	

	return critical->AngleSum;				//返回新的反馈角度，作为PID算法的反馈值
}
