#include "gyro.h"
#include "string.h"
#include "motor.h"

uint8_t TxBuffer[256],TxCounter=0,count=0;//陀螺仪数据接收数组

struct SGyro  stcGyro;
struct SAngle stcAngle;

/**
  * @name   CopeSerial6Data
  * @brief  陀螺仪解析函数
  * @param  none
  * @retval none
  * @attention none
*/
void CopeSerial6Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//数据不满11个，则返回
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据共同体里面，从而实现数据的解析。
			case 0x53:  memcpy(&stcAngle,&ucRxBuffer[2],8); 
			W_Temp_PID.Gyro.Angle[PITCH]= (float)stcAngle.Angle[0]/32768*180;
			W_Temp_PID.Gyro.Angle[ROLL] = (float)stcAngle.Angle[1]/32768*180;
			W_Temp_PID.Gyro.Angle[YAW]  = (float)stcAngle.Angle[2]/32768*180;
		
		  	W_Temp_PID.Gyro.Velocity[ROLL] =(double)stcGyro.w[0]/32768*1000;	//X轴角速度
			W_Temp_PID.Gyro.Velocity[PITCH]=(double)stcGyro.w[1]/32768*1000;	//Y轴角速度
			W_Temp_PID.Gyro.Velocity[YAW]  =(double)stcGyro.w[2]/32768*1000;	//Z轴角速度
			break;
		}
		ucRxCnt=0;
	}
}


