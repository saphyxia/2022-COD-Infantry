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
//	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
//	{
//		ucRxCnt=0;
//		return;
//	}
//  if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
//  else
//	{
//		switch(ucRxBuffer[1])
//		{
//			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
//			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
//			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
//			case 0x53:  memcpy(&stcAngle,&ucRxBuffer[2],8); 

//			imu_JY901.yaw_Angle = (float)stcAngle.Angle[2]/32768*180;
//			
//			if(imu_JY901.yaw_Angle>=0) imu_JY901.yaw_sign=1;
//			if(imu_JY901.yaw_Angle<0)  imu_JY901.yaw_sign=0; 
//			imu_JY901.yaw_int=ABS((int)imu_JY901.yaw_Angle);
//			imu_JY901.yaw_decimal=ABS((int)(imu_JY901.yaw_Angle*100)%100);
//			imu_JY901.yaw_Gyro=(double)stcGyro.w[2]/32768*1000;	//Z����ٶ�
//			break;
//		}
//		ucRxCnt=0;
//	}
}




