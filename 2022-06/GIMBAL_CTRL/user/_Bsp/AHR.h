#ifndef AHR_H
#define AHR_H
#include "main.h"
/*�������˲�*/
		typedef struct
		{
		   float Xk_accelX;
		   float pk_accelX;
		   float Q_accelX;
		   float R_accelX;
		   float Kk_accelX;
		   
		   float Xk_accelY;
		   float Pk_accelY;
			 float R_accelY;
		   float Q_accelY;
		   float Kk_accelY;
		
		   float Xk_accelZ;
		   float Pk_accelZ;
		   float Q_accelZ;
			 float R_accelZ;
		   float Kk_accelZ;		
			 
		   float Xk_magX;
		   float Pk_magX;
		   float Q_magX;
			 float R_magX;
		   float Kk_magX;
		   
		   float Xk_magY;
		   float Pk_magY;
		   float Q_magY;
			 float R_magY;
		   float Kk_magY;
		
		   float Xk_magZ;
		   float Pk_magZ;
		   float Q_magZ;
			 float R_magZ;
		   float Kk_magZ;				 
		}kalman_t;

#define M_PI  (fp32)3.141592653589f

typedef struct __BMI088_REAL_Data__
{
    float Accel_X;  //ת����ʵ�ʵ�X����ٶȣ�
    float Accel_Y;  //ת����ʵ�ʵ�Y����ٶȣ�
    float Accel_Z;  //ת����ʵ�ʵ�Z����ٶȣ�
    float Temp;     //ת����ʵ�ʵ��¶ȣ���λΪ���϶�
    float Gyro_X;   //ת����ʵ�ʵ�X��Ǽ��ٶȣ�
    float Gyro_Y;   //ת����ʵ�ʵ�Y��Ǽ��ٶȣ�
    float Gyro_Z;   //ת����ʵ�ʵ�Z��Ǽ��ٶ�
		float GyroRawData_Z;
		float GyroRawData_Y;
}BMI088_REAL_DATA;

typedef struct __IST8310_REAL_Data__
{
    float MAG_X;  
    float MAG_Y;  
    float MAG_Z;  
}IST8310_REAL_DATA;

extern volatile float q0, q1, q2, q3,w1,w2,w3; // ȫ����Ԫ�������ٶȲ������

extern void IMU_getYawPitchRoll(volatile float * angles); 
extern void IMU_getQ(volatile float * q);
extern void IMU_getValues(volatile float * values) ; 
extern void IMU_AHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
extern float invSqrt(float x);
extern void GetPitchYawGxGyGz(void);
uint32_t Get_Time_Micros(void);
		
#endif


