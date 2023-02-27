#ifndef JY901_H
#define JY901_H
#include "main.h"


typedef struct{
	int yaw_sign,yaw_int,yaw_decimal;
    float yaw_Angle;
    double yaw_Gyro;
}JY901;
extern JY901 imu_JY901;

void CopeSerial6Data(unsigned char ucData);




#endif


