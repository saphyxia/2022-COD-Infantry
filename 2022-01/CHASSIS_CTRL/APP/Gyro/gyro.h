#ifndef __GYRO_H
#define __GYRO_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

typedef enum
{
	PITCH=0,
	YAW,
	ROLL,
	GYRO_DATA_NUM
}GYRO_ENUM_T;

/* Private struct ------------------------------------------------------------*/
typedef struct
{
	float Angle[GYRO_DATA_NUM];
	double Velocity[GYRO_DATA_NUM];
}GYRO_DATA_T;

//陀螺仪解算
struct STime
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
};
struct SAcc
{
	short a[3];
	short T;
};
struct SGyro
{
	short w[3];
	short T;
};
struct SAngle
{
	short Angle[3];
	short T;
};
struct SMag
{
	short h[3];
	short T;
};
struct SDStatus
{
	short sDStatus[4];
};
struct SPress
{
	long lPressure;
	long lAltitude;
};
struct SLonLat
{
	long lLon;
	long lLat;
};
struct SGPSV
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
};

extern uint8_t TxBuffer[256],TxCounter,count;//陀螺仪数据接收数组

extern void CopeSerial6Data(unsigned char ucData);

#endif


