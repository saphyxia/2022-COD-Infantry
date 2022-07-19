#include "assist.h"


/**
 * @name   encoder_To_Angle
 * @brief  �Ƕ�ת��
 * @param
 *         encoder:�������Ƕ�        encoder_Max:���������ֵ�����̣�
 * @retval result_Angle:�ܽǽǶ�
 * @attention
*/
float encoder_To_Angle(volatile int16_t* encoder,float encoder_Max)
{
   return *encoder/encoder_Max*360.0f;
}


/**
 * @name   RAMP_float
 * @brief  �����Ϣ����ṹ��
 * @param
 *         final:����ֵ		now:��ǰֵ		ramp:�仯��
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
 * @brief  ������������չ��
 * @param
 *         result_Encoder:��չ��ı������������      act_Encoder:ʵ�ʱ�������ֵ������1��      torque_Ratio:��չ����
 * @retval none
 * @attention
 *         �����Ҫ�ϵ�ʱ������ֵ���㣬����ʼangle_Last��ֵΪdebug�����������ɡ�
*/
int16_t encoder_Extend(int16_t act_Encoder,int16_t torque_Ratio)
{
   static int16_t angle_Last=0;
   static int16_t result_Encoder=0;
   int res1,res2;
   if(act_Encoder<angle_Last){
       res1=act_Encoder-angle_Last+8192;
       res2=act_Encoder-angle_Last;
   }else{
       res1=act_Encoder-angle_Last-8192;
       res2=act_Encoder-angle_Last;
   }
   angle_Last=act_Encoder;
	ABS(res1)<ABS(res2) ? (result_Encoder+=res1) : (result_Encoder+=res2);
	if(ABS(result_Encoder)>8192*torque_Ratio) result_Encoder=0;
	return result_Encoder;
}


/**
 * @name   angle_Extend
 * @brief  �Ƕ���չ
 * @param
 *         act_Angle:ʵ�ʽǶȣ�ͨ��Ϊ�����Ƿ��صõ���Ҳ�������ñ��������㡣
 * @retval result_Angle:�����ۼӵĽ�����ܽǽǶȣ�
 * @attention
 *         �������180�ٽ�����ͽ��д���
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
   if(ABS(res1)<0.01f||ABS(res2)<0.01f)res1=res2=0;
   ABS(res1)<ABS(res2) ? (*result_Angle+=res1) : (*result_Angle+=res2);
}


