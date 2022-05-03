#include "gyro.h"
#include "string.h"
#include "motor.h"

uint8_t TxBuffer[256],TxCounter=0,count=0;//���������ݽ�������

struct SGyro  stcGyro;
struct SAngle stcAngle;

/**
  * @name   CopeSerial6Data
  * @brief  �����ǽ�������
  * @param  none
  * @retval none
  * @attention none
*/
void CopeSerial6Data(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x53:  memcpy(&stcAngle,&ucRxBuffer[2],8); 
			W_Temp_PID.Gyro.Angle[PITCH]= (float)stcAngle.Angle[0]/32768*180;
			W_Temp_PID.Gyro.Angle[ROLL] = (float)stcAngle.Angle[1]/32768*180;
			W_Temp_PID.Gyro.Angle[YAW]  = (float)stcAngle.Angle[2]/32768*180;
		
		  	W_Temp_PID.Gyro.Velocity[ROLL] =(double)stcGyro.w[0]/32768*1000;	//X����ٶ�
			W_Temp_PID.Gyro.Velocity[PITCH]=(double)stcGyro.w[1]/32768*1000;	//Y����ٶ�
			W_Temp_PID.Gyro.Velocity[YAW]  =(double)stcGyro.w[2]/32768*1000;	//Z����ٶ�
			break;
		}
		ucRxCnt=0;
	}
}


