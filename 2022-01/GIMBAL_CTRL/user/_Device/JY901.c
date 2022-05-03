#include "JY901.h"
#include "string.h"


JY901 imu_JY901;

struct 
{
	unsigned char ucYear;
	unsigned char ucMonth;
	unsigned char ucDay;
	unsigned char ucHour;
	unsigned char ucMinute;
	unsigned char ucSecond;
	unsigned short usMiliSecond;
}stcTime;
struct 
{
	short a[3];
	short T;
}stcAcc;
struct 
{
	short w[3];
	short T;
}stcGyro;
struct 
{
	short Angle[3];
	short T;
}stcAngle;
struct 
{
	short h[3];
	short T;
}stcMag;
struct 
{
	short sDStatus[4];
}stcDStatus;
struct 
{
	long lPressure;
	long lAltitude;
}stcPress;
struct 
{
	long lLon;
	long lLat;
}stcLonLat;
struct 
{
	short sGPSHeight;
	short sGPSYaw;
	long lGPSVelocity;
}stcGPSV;


void CopeSerial6Data(unsigned char ucData)
{
//	static unsigned char ucRxBuffer[250];
//	static unsigned char ucRxCnt = 0;	
//	ucRxBuffer[ucRxCnt++]=ucData;
//	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
//	{
//		ucRxCnt=0;
//		return;
//	}
//  if (ucRxCnt<11) {return;}//数据不满11个，则返回
//  else
//	{
//		switch(ucRxBuffer[1])
//		{
//			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
//			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
//			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
//			case 0x53:  memcpy(&stcAngle,&ucRxBuffer[2],8); 

//			imu_JY901.yaw_Angle = (float)stcAngle.Angle[2]/32768*180;
//			
//			if(imu_JY901.yaw_Angle>=0) imu_JY901.yaw_sign=1;
//			if(imu_JY901.yaw_Angle<0)  imu_JY901.yaw_sign=0; 
//			imu_JY901.yaw_int=ABS((int)imu_JY901.yaw_Angle);
//			imu_JY901.yaw_decimal=ABS((int)(imu_JY901.yaw_Angle*100)%100);
//			imu_JY901.yaw_Gyro=(double)stcGyro.w[2]/32768*1000;	//Z轴角速度
//			break;
//		}
//		ucRxCnt=0;
//	}
}




