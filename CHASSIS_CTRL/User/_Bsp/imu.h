/**
  *@file test_imu.h
  *@date 2016-12-12
  *@author Albert.D
  *@brief 
  */
  #define abs(x) ((x)>0? (x):(-(x)))
#ifndef _TEST__IMU_H
#define _TEST__IMU_H
#define M_PI  (float)3.1415926535
#include "main.h"
#include "filter.h"

#define MPU6500     PFout(6)
/*

#define MPU6500_NSS_High()     GPIO_ResetBits(GPIOF,GPIO_Pin_6)
#define MPU6500_NSS_Low()    GPIO_SetBits(GPIOF,GPIO_Pin_6)
*/
//volatile float yaw_angle,pitch_angle,roll_angle,gg_x,gg_z,gg_y,ga_x,ga_y,ga_z,angle1, angle_dot,ga_y1,kg,angle2, angle_dot2,ga_y2,kg2,angle3, angle_dot3,ga_y3,kg3,ga_y22,ga_y33; //ʹ�õ��ĽǶ�ֵ

typedef struct
{
  int16_t ax;
  int16_t ay;
  int16_t az;
  
  int16_t temp;
  
  int16_t gx;
  int16_t gy;
  int16_t gz;
  
  int16_t mx;
  int16_t my;
  int16_t mz;
}IMUDataTypedef;


extern uint8_t MPU_id;
extern int16_t test;
uint8_t MPU6500_Init(void);
uint8_t MPU6500_Write_Reg(uint8_t const reg, uint8_t const data);
uint8_t MPU6500_Read_Reg(uint8_t const reg);
void IMU_getValues(volatile float * values);
uint8_t IST8310_Init(void);

extern volatile float angle[3];
extern volatile float yaw_angle,pitch_angle,roll_angle,gg_x,gg_y,gg_z,ga_x,ga_y,ga_z,ga_y1,kg,ga_y3,kg3,ga_y22,ga_y33; //ʹ�õ��ĽǶ�ֵ
extern extKalman_t KF_IMU_GYRO;

typedef struct __MPU6050_REAL_Data__
{
    float Accel_X;  //ת����ʵ�ʵ�X����ٶȣ�
    float Accel_Y;  //ת����ʵ�ʵ�Y����ٶȣ�
    float Accel_Z;  //ת����ʵ�ʵ�Z����ٶȣ�
    float Temp;     //ת����ʵ�ʵ��¶ȣ���λΪ���϶�
    float Gyro_X;   //ת����ʵ�ʵ�X��Ǽ��ٶȣ�
    float Gyro_Y;   //ת����ʵ�ʵ�Y��Ǽ��ٶȣ�
    float Gyro_Z;   //ת����ʵ�ʵ�Z��Ǽ��ٶ�
		int16_t GyroRawData_Z;
		int16_t GyroRawData_Y;
}MPU6050_REAL_DATA;
extern MPU6050_REAL_DATA MPU6050_read_data;
extern volatile MPU6050_REAL_DATA   MPU6050_Real_Data;
void IMU_getYawPitchRoll(volatile float * ypr); //������̬
void GetPitchYawGxGyGz(void);
float Get_Yaw_Angle(int16_t Gyro_Z);

#endif

