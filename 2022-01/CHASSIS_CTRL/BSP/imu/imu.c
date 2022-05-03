#include "stm32f4xx.h"
#include "stm32f4xx_it.h"

#include "imu.h"
#include "spi.h"
#include "sys.h"
#include "delay.h"
#include "mpu6500_reg.h"
#include "stm32f4xx_conf.h"

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>

volatile float yaw_angle,pitch_angle,roll_angle,gg_x,gg_z,gg_y,ga_x,ga_y,ga_z,angle1, angle_dot,ga_y1,kg,angle2, angle_dot2,ga_y2,kg2,angle3, angle_dot3,ga_y3,kg3,ga_y22,ga_y33; //ʹ�õ��ĽǶ�ֵ


volatile float exInt, eyInt, ezInt;  // ������
volatile float q0 = 1.0f;
volatile float q1 = 0.0f;
volatile float q2 = 0.0f;
volatile float q3 = 0.0f;

volatile float mygetqval[9];	//���ڴ�Ŵ�����ת�����������
static volatile float gx, gy, gz, ax, ay, az, mx, my, mz;   //��������ڴ��ļ���

static volatile float q[4]; //����Ԫ��
volatile uint32_t lastUpdate, now; // �������ڼ��� ��λ us
volatile float angle[3] = {0};
volatile float yaw_temp,pitch_temp,roll_temp;
volatile float last_yaw_temp,last_pitch_temp,last_roll_temp;
uint8_t MPU_id = 0;

IMUDataTypedef imu_data = {0,0,0,0,0,0,0,0,0,0};

IMUDataTypedef imu_data_offest = {0,0,0,0,0,0,0,0,0,0};

//Write a register to MPU6500
uint8_t MPU6500_Write_Reg(uint8_t const reg, uint8_t const data)
{
  static uint8_t MPU_Tx;
  
  MPU6500=0;
  
  MPU_Tx = reg&0x7f;
  SPI5_ReadWriteByte(MPU_Tx);
  MPU_Tx = data;
  SPI5_ReadWriteByte(MPU_Tx);
  
  MPU6500=1;
  return 0;
}

//Read a register from MPU6500
uint8_t MPU6500_Read_Reg(uint8_t const reg)
{
  static uint8_t MPU_Rx, MPU_Tx;
  
  MPU6500=0;
  
  MPU_Tx = reg|0x80;
  SPI5_ReadWriteByte(MPU_Tx);
  MPU_Rx=SPI5_ReadWriteByte(reg|0x80);
  
  MPU6500=1;
  return  MPU_Rx;
}
uint8_t MPU6500_Read_Regs(uint8_t const reg)
{
  static uint8_t MPU_Rx, MPU_Tx;
  MPU_Tx = reg|0x80;
  MPU_Rx=SPI5_ReadWriteByte(MPU_Tx);
  return  MPU_Rx;
}

//Write IST8310 register through MPU6500
//static void IST_Reg_Write_By_MPU(uint8_t addr, uint8_t data)
//{
//MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
//delay_ms(2);
//MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, addr);
//delay_ms(2);
//MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, data);
//delay_ms(2);

//MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x080 | 0x01);
//delay_ms(10);
//}

//Write IST8310 register through MPU6500
//static uint8_t IST_Reg_Read_By_MPU(uint8_t addr)
//{
//uint8_t data;

//MPU6500_Write_Reg(MPU6500_I2C_SLV4_REG, addr);
//delay_ms(10);
//MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x80);
//delay_ms(10);
//data = MPU6500_Read_Reg(MPU6500_I2C_SLV4_DI);
//MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
//delay_ms(10);
//return data;
//}

//Initialize the MPU6500 I2C Slave0 for I2C reading
//static void MPU_Auto_Read_IST_config(uint8_t device_address, uint8_t reg_base_addr, uint8_t data_num)
//{
//  MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, device_address);
//  delay_ms(2);
////  MPU6500_Write_Reg(MPU6500_I2C_SLV1_REG, IST8310_R_CONFA);
////  delay_ms(2);
////  MPU6500_Write_Reg(MPU6500_I2C_SLV1_DO, IST8310_ODR_MODE);
//  delay_ms(2);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV0_ADDR, 0x80 | device_address);
//  delay_ms(2);
//  MPU6500_Write_Reg(MPU6500_I2C_SLV0_REG, reg_base_addr);
//  delay_ms(2);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x03);
//  delay_ms(2);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_MST_DELAY_CTRL, 0x01 | 0x02);
//  delay_ms(2);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x80 | 0x01);
//  delay_ms(6);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV0_CTRL, 0x80 | data_num);
//  delay_ms(7);
//}

//Initialize the IST8310
uint8_t IST8310_Init(void)
{
//  MPU6500_Write_Reg(MPU6500_USER_CTRL, 0x30);
//  delay_ms(10);
//  MPU6500_Write_Reg(MPU6500_I2C_MST_CTRL, 0x0d);
//  delay_ms(10);
  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV1_ADDR, IST8310_ADDRESS);
//  delay_ms(10);
//  MPU6500_Write_Reg(MPU6500_I2C_SLV4_ADDR, 0x80 | IST8310_ADDRESS);
//  delay_ms(10);
//  
////  IST_Reg_Write_By_MPU(IST8310_R_CONFB, 0x01);
//  if(IST8310_DEVICE_ID_A != IST_Reg_Read_By_MPU(IST8310_WHO_AM_I))
//    return 1; //error
//  delay_ms(10);
//  
////  IST_Reg_Write_By_MPU(IST8310_R_CONFA, 0x00);
////  if(IST_Reg_Read_By_MPU(IST8310_R_CONFA) != 0x00)
//    return 2;
//  delay_ms(10);
//  
//  IST_Reg_Write_By_MPU(IST8310_R_CONFB, 0x00);
//  if(IST_Reg_Read_By_MPU(IST8310_R_CONFB) != 0x00)
//    return 3;
//  delay_ms(10);
//  
//  IST_Reg_Write_By_MPU(IST8310_AVGCNTL, 0x24);
//  if(IST_Reg_Read_By_MPU(IST8310_AVGCNTL) != 0x24)
//    return 4;
//  delay_ms(10);
  
//  IST_Reg_Write_By_MPU(IST8310_PDCNTL, 0xc0);
//  if(IST_Reg_Read_By_MPU(IST8310_PDCNTL) != 0xc0)
//    return 5;
//  delay_ms(10);
//  
//  MPU6500_Write_Reg(MPU6500_I2C_SLV1_CTRL, 0x00);
//  delay_ms(10);
//  MPU6500_Write_Reg(MPU6500_I2C_SLV4_CTRL, 0x00);
//  delay_ms(10);
//  
//  MPU_Auto_Read_IST_config(IST8310_ADDRESS, IST8310_R_XL, 0x06);
//  delay_ms(100);
  return 0;
}

//Set the accelerated velocity resolution
uint8_t MPU6500_Set_Accel_Fsr(uint8_t fsr)
{
  return MPU6500_Write_Reg(MPU6500_ACCEL_CONFIG, fsr<<3);
}

//Set the angular velocity resolution
uint8_t MPU6500_Set_Gyro_Fsr(uint8_t fsr)
{
  return MPU6500_Write_Reg(MPU6500_GYRO_CONFIG, fsr<<3);
}


MPU6050_REAL_DATA MPU6050_read_data;
volatile MPU6050_REAL_DATA   MPU6050_Real_Data;
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void Init_Quaternion
*��������:	 ��ʼ����Ԫ��
��������� ��ǰ�Ĳ���ֵ��
���������û��
*******************************************************************************/
//��ʼ��IMU����
#define BOARD_DOWN 1   //�������泯�°ڷ�

void Init_Quaternion()//���ݲ������ݣ���ʼ��q0,q1,q2.q3���Ӷ��ӿ������ٶ�
{
	int16_t hx,hy;
//HMC58X3_getlastValues(&hx,&hy,&hz);
	#ifdef BOARD_DOWN
	if(hx<0 && hy <0)   //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.005;
			q1 = -0.199;
			q2 = 0.979;
			q3 = -0.0089;
		}
		else
		{
			q0 = -0.008;
			q1 = -0.555;
			q2 = 0.83;
			q3 = -0.002;
		}
		
	}
	else if (hx<0 && hy > 0) //OK
	{
		if(fabs(hx/hy)>=1)   
		{
			q0 = 0.005;
			q1 = -0.199;
			q2 = -0.978;
			q3 = 0.012;
		}
		else
		{
			q0 = 0.005;
			q1 = -0.553;
			q2 = -0.83;
			q3 = -0.0023;
		}
		
	}
	else if (hx > 0 && hy > 0)   //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.0012;
			q1 = -0.978;
			q2 = -0.199;
			q3 = -0.005;
		}
		else
		{
			q0 = 0.0023;
			q1 = -0.83;
			q2 = -0.553;
			q3 = 0.0023;
		}
		
	}
	else if (hx > 0 && hy < 0)     //OK
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.0025;
			q1 = 0.978;
			q2 = -0.199;
			q3 = 0.008;			
		}
		else
		{
			q0 = 0.0025;
			q1 = 0.83;
			q2 = -0.56;
			q3 = 0.0045;
		}		
	}
	#else
		if(hx<0 && hy <0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = 0.195;
			q1 = -0.015;
			q2 = 0.0043;
			q3 = 0.979;
		}
		else
		{
			q0 = 0.555;
			q1 = -0.015;
			q2 = 0.006;
			q3 = 0.829;
		}
		
	}
	else if (hx<0 && hy > 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.193;
			q1 = -0.009;
			q2 = -0.006;
			q3 = 0.979;
		}
		else
		{
			q0 = -0.552;
			q1 = -0.0048;
			q2 = -0.0115;
			q3 = 0.8313;
		}
		
	}
	else if (hx>0 && hy > 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.9785;
			q1 = 0.008;
			q2 = -0.02;
			q3 = 0.195;
		}
		else
		{
			q0 = -0.9828;
			q1 = 0.002;
			q2 = -0.0167;
			q3 = 0.5557;
		}
		
	}
	else if (hx > 0 && hy < 0)
	{
		if(fabs(hx/hy)>=1)
		{
			q0 = -0.979;
			q1 = 0.0116;
			q2 = -0.0167;
			q3 = -0.195;			
		}
		else
		{
			q0 = -0.83;
			q1 = 0.014;
			q2 = -0.012;
			q3 = -0.556;
		}		
	}
	#endif
	
	//����hx hy hz���ж�q��ֵ��ȡ�ĸ������ֵ���ƽ�����,��ʼֵ������ŷ����ת������Ԫ������õ�
	 
}
int32_t MPU6050_FIFO[6][101] = {0};//[0]-[9]Ϊ���10������ [10]Ϊ10�����ݵ�ƽ��ֵ						 
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getValues(volatile float * values)
*��������:	 ��ȡ���ٶ� ������ ������ �ĵ�ǰֵ  
��������� �������ŵ������׵�ַ
���ٶ�ֵ��ԭʼ���ݣ�-8192-+8192
���ٶ�ֵ��deg/s
������ֵ��ԭʼ����
���������û��
*******************************************************************************/
int16_t eff;
int16_t eff2;
int16_t eff3;
int16_t gy_z;
float C_Gyo_z;
int16_t Eff[80] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
									 10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
									 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
									 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
									 40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
									 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
									 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
									 70, 71, 72, 73, 74, 75, 76, 77, 78, 79}; 

//void IMU_Get_Data()
void IMU_getValues(volatile float * values) 
{
	uint8_t i = 0;
	int32_t sum=0;
	
	uint8_t mpu_buff[14];
	MPU6500=0;
	SPI5_ReadWriteByte(MPU6500_ACCEL_XOUT_H|0x80);

  mpu_buff[0]=MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_H); 
  mpu_buff[1]=MPU6500_Read_Regs(MPU6500_ACCEL_XOUT_L); 
  imu_data.ax = mpu_buff[0]<<8 |mpu_buff[1];
	mpu_buff[2]=MPU6500_Read_Regs(MPU6500_ACCEL_YOUT_H); 
  mpu_buff[3]=MPU6500_Read_Regs(MPU6500_ACCEL_YOUT_L); 
  imu_data.ay = mpu_buff[2]<<8 |mpu_buff[3];
	mpu_buff[4]=MPU6500_Read_Regs(MPU6500_ACCEL_ZOUT_H); 
  mpu_buff[5]=MPU6500_Read_Regs(MPU6500_ACCEL_ZOUT_L); 	
  imu_data.az = mpu_buff[4]<<8 |mpu_buff[5];
	
	mpu_buff[6]=MPU6500_Read_Regs(MPU6500_TEMP_OUT_H); 
  mpu_buff[7]=MPU6500_Read_Regs(MPU6500_TEMP_OUT_L); 	
  imu_data.temp = mpu_buff[6]<<8 |mpu_buff[7];
  
	
	mpu_buff[8]=MPU6500_Read_Regs(MPU6500_GYRO_XOUT_H);
  mpu_buff[9]=MPU6500_Read_Regs(MPU6500_GYRO_XOUT_L); 	
  imu_data.gx = mpu_buff[8]<<8 |mpu_buff[9] - imu_data_offest.gx;
	
	mpu_buff[10]=MPU6500_Read_Regs(MPU6500_GYRO_YOUT_H); 
  mpu_buff[11]=MPU6500_Read_Regs(MPU6500_GYRO_YOUT_L); 	
	
  imu_data.gy = mpu_buff[10]<<8 |mpu_buff[11] - imu_data_offest.gy;
	mpu_buff[12]=MPU6500_Read_Regs(MPU6500_GYRO_ZOUT_H); 
  mpu_buff[13]=MPU6500_Read_Regs(MPU6500_GYRO_ZOUT_L);
  imu_data.gz = mpu_buff[12]<<8 |mpu_buff[13] - imu_data_offest.gz;



	  if(abs(imu_data.gz)< 20)
	  {
		    imu_data.gz = 0;
	  }

	MPU6500=1;
		int j;
	int k;
	int l;
  for(i=1;i<100;i++)
	{
//		MPU6050_FIFO[0][i-1]=MPU6050_FIFO[0][i];
//		MPU6050_FIFO[1][i-1]=MPU6050_FIFO[1][i];
//		MPU6050_FIFO[2][i-1]=MPU6050_FIFO[2][i];
//		MPU6050_FIFO[3][i-1]=MPU6050_FIFO[3][i];
//		MPU6050_FIFO[4][i-1]=MPU6050_FIFO[4][i];
		MPU6050_FIFO[5][i-1]=MPU6050_FIFO[5][i];
	}
	
//	MPU6050_FIFO[0][99] = imu_data.ax;
//	MPU6050_FIFO[1][99] = imu_data.ay;
//	MPU6050_FIFO[2][99] = imu_data.az;
//	MPU6050_FIFO[3][99] = imu_data.gx;
//	MPU6050_FIFO[4][99] = imu_data.gy;
	MPU6050_FIFO[5][99] = imu_data.gz;
	
//for(i=0;i<100;i++)//��ǰ����ĺϣ���ȡƽ��ֵ
//	{	
//		 sum+=MPU6050_FIFO[0][i];
//	}
//	MPU6050_FIFO[0][100]=sum/100;

//	sum=0;
//	for(i=0;i<100;i++){
//		 sum+=MPU6050_FIFO[1][i];
//	}
//	MPU6050_FIFO[1][100]=sum/100;

//	sum=0;
//	for(i=0;i<100;i++){
//		 sum+=MPU6050_FIFO[2][i];
//	}
//	MPU6050_FIFO[2][100]=sum/100;

//	sum=0;
//	for(i=0;i<100;i++){
//		 sum+=MPU6050_FIFO[3][i];
//	}
//	MPU6050_FIFO[3][100]=sum/100;

//	sum=0;
//	for(i=0;i<100;i++){
//		 sum+=MPU6050_FIFO[4][i];
//	}
//	MPU6050_FIFO[4][100]=sum/100;

//	sum=0;
	for(i=0;i<100;i++){
		 sum+=MPU6050_FIFO[5][i];
	}
	MPU6050_FIFO[5][100]=sum/100;
	sum=0;

//	MPU6050_read_data.Accel_X = MPU6050_FIFO[0][100] ;
//	MPU6050_read_data.Accel_Y = MPU6050_FIFO[1][100] ;
//	MPU6050_read_data.Accel_Z = MPU6050_FIFO[2][100] ;	
//	MPU6050_read_data.Gyro_X = MPU6050_FIFO[3][100]  ;
//	MPU6050_read_data.Gyro_Y = MPU6050_FIFO[4][100]  ;
	MPU6050_read_data.Gyro_Z = MPU6050_FIFO[5][100]  ;
//	for(j = 0; j <50; j++)
//		{
//				if(MPU6050_read_data.Gyro_X ==  Eff[j])
//				{
//						eff = Eff[j];
//						break;
//				}
//				if(-MPU6050_read_data.Gyro_X ==  Eff[j])
//				{
//						eff = -Eff[j];
//						break;
//				}
//		}		
//		for(k = 0; k <25; k++)
//		{
//				if(MPU6050_read_data.Gyro_Y ==  Eff[k])
//				{
//						eff2 = Eff[k];
//						break;
//				}
//				if(-MPU6050_read_data.Gyro_Y ==  Eff[k])
//				{
//						eff2 = -Eff[k];
//						break;
//				}
//		}	
		for(l = 0; l<23; l++)
		{
				if(MPU6050_read_data.Gyro_Z ==  Eff[l])
				{
						eff3 = Eff[l];
						break;
				}
				if(-MPU6050_read_data.Gyro_Z ==  Eff[l])
				{
						eff3 = -Eff[l];
						break;
				}
		}	
//		ga_y=ga_y;
//		ga_y1=ga_y1+0.018f;//������
//		kg=ga_y1/(ga_y1+0.542f);//������
//		angle1=MPU6050_read_data.Accel_X;
//		ga_y=ga_y+kg*(angle1-ga_y);//����ֵ
//		ga_y1=(1-kg)*ga_y1;
//		
//		ga_y22=ga_y22;
//		ga_y2=ga_y2+0.018f;
//		kg2=ga_y2/(ga_y2+0.542f);
//		angle2=MPU6050_read_data.Accel_Y;
//		ga_y22=ga_y22+kg2*(angle2-ga_y22);
//		ga_y2=(1-kg2)*ga_y2;
//		
//		ga_y33=ga_y33;
//		ga_y3=ga_y3+0.018f;
//		kg3=ga_y3/(ga_y3+0.542f);
//		angle3=MPU6050_read_data.Accel_Z;
//		ga_y33=ga_y33+kg3*(angle3-ga_y33);
//		ga_y3=(1-kg3)*ga_y3;
//		
//		values[0]=ga_y;
//		values[1]=ga_y22;
//		values[2]=ga_y33;
		
//		MPU6050_read_data.Gyro_X=MPU6050_read_data.Gyro_X-eff;
//		values[3]=MPU6050_read_data.Gyro_X=MPU6050_read_data.Gyro_X/32.8f;
//		MPU6050_read_data.Gyro_Y=MPU6050_read_data.Gyro_Y-eff2;
//		values[4]=MPU6050_read_data.Gyro_Y=MPU6050_read_data.Gyro_Y/32.8f;
		MPU6050_read_data.Gyro_Z=MPU6050_read_data.Gyro_Z-eff3;
		values[5]=MPU6050_read_data.Gyro_Z;
		
		
//	one.d=imu_data.gx;
//	one.y=imu_data.gy;
//	one.th=imu_data.gz;
	//printf("%d   %d\r\n",MPU6050_FIFO[0][10],imu_data.gx);
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   float invSqrt(float x)
*��������:	   ���ټ��� 1/Sqrt(x) 	
��������� Ҫ�����ֵ
��������� ���
*******************************************************************************/
float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}
/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_AHRSupdate
*��������:	 ����AHRS ������Ԫ�� 
��������� ��ǰ�Ĳ���ֵ��
���������û��
*******************************************************************************/
uint32_t Get_Time_Micros(void)
{
	return TIM2->CNT;
}

#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.01f   // integral gain governs rate of convergence of gyroscope biases
void IMU_AHRSupdate(void) {
    float norm;
    float hx, hy, hz, bx, bz;
    float vx, vy, vz, wx, wy, wz;
    float ex, ey, ez,halfT;
    float tempq0,tempq1,tempq2,tempq3;

    float q0q0 = q0*q0;
    float q0q1 = q0*q1;
    float q0q2 = q0*q2;
    float q0q3 = q0*q3;
    float q1q1 = q1*q1;
    float q1q2 = q1*q2;
    float q1q3 = q1*q3;
    float q2q2 = q2*q2;   
    float q2q3 = q2*q3;
    float q3q3 = q3*q3;   

    gx = mygetqval[3] * M_PI/180;
    gy = mygetqval[4] * M_PI/180;
    gz = mygetqval[5] * M_PI/180;
    ax = mygetqval[0];
    ay = mygetqval[1];
    az = mygetqval[2];
    mx = mygetqval[6];
    my = mygetqval[7];
    mz = mygetqval[8];		

    now = Get_Time_Micros();  //��ȡʱ�� ��λ��us   
    if(now<lastUpdate)
    {
    //halfT =  ((float)(now + (0xffffffff- lastUpdate)) / 2000000.0f);   //  uint 0.5s
    }
    else	
    {
        halfT =  ((float)(now - lastUpdate) / 2000000.0f);
    }
    lastUpdate = now;	//����ʱ��
    //������ƽ�����㷨
    norm = invSqrt(ax*ax + ay*ay + az*az);       
    ax = ax * norm;
    ay = ay * norm;
    az = az * norm;
    //�ѼӼƵ���ά����ת�ɵ�λ������
    norm = invSqrt(mx*mx + my*my + mz*mz);          
    mx = mx * norm;
    my = my * norm;
    mz = mz * norm; 
    // compute reference direction of flux
    hx = 2.0f*mx*(0.5f - q2q2 - q3q3) + 2.0f*my*(q1q2 - q0q3) + 2.0f*mz*(q1q3 + q0q2);
    hy = 2.0f*mx*(q1q2 + q0q3) + 2.0f*my*(0.5f - q1q1 - q3q3) + 2.0f*mz*(q2q3 - q0q1);
    hz = 2.0f*mx*(q1q3 - q0q2) + 2.0f*my*(q2q3 + q0q1) + 2.0f*mz*(0.5f - q1q1 - q2q2);         
    bx = sqrt((hx*hx) + (hy*hy));
    bz = hz; 
    // estimated direction of gravity and flux (v and w)
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;
    wx = 2.0f*bx*(0.5f - q2q2 - q3q3) + 2.0f*bz*(q1q3 - q0q2);
    wy = 2.0f*bx*(q1q2 - q0q3) + 2.0f*bz*(q0q1 + q2q3);
    wz = 2.0f*bx*(q0q2 + q1q3) + 2.0f*bz*(0.5f - q1q1 - q2q2);  
    // error is sum of cross product between reference direction of fields and direction measured by sensors
    ex = (ay*vz - az*vy) + (my*wz - mz*wy);
    ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
    ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;
        // �ò���������PI����������ƫ
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
    }
    // ��Ԫ��΢�ַ���
    tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

    // ��Ԫ���淶��
    norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
    q0 = tempq0 * norm;
    q1 = tempq1 * norm;
    q2 = tempq2 * norm;
    q3 = tempq3 * norm;

}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getQ(float * q)
*��������:	 ������Ԫ�� ���ص�ǰ����Ԫ����ֵ
��������� ��Ҫ�����Ԫ���������׵�ַ
���������û��
*******************************************************************************/

void IMU_getQ(volatile float * q) {

    IMU_getValues(mygetqval);	 //��ȡԭʼ����,���ٶȼƺʹ�������ԭʼֵ��������ת������deg/s
    IMU_AHRSupdate();
//    q[0] = q0; //���ص�ǰֵ
//    q[1] = q1;
//    q[2] = q2;
//    q[3] = q3;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:	   void IMU_getYawPitchRoll(float * angles)
*��������:	 ������Ԫ�� ���ص�ǰ��������̬����
��������� ��Ҫ�����̬�ǵ������׵�ַ
���������û��
*******************************************************************************/
void IMU_getYawPitchRoll(volatile float * angles) 
{  
    // volatile float gx=0.0, gy=0.0, gz=0.0; //������������
    IMU_getQ(q); //����ȫ����Ԫ��
    //��Ԫ��ת����ŷ���ǣ��������Ǻ������㼴��
//    angles[0] = -atan2(2 * q[1] * q[2] + 2 * q[0] * q[3], -2 * q[2]*q[2] - 2 * q[3] * q[3] + 1)* 180/M_PI; // yaw        -pi----pi
//    angles[1] = -asin(-2 * q[1] * q[3] + 2 * q[0] * q[2])* 180/M_PI; // pitch    -pi/2    --- pi/2 
//    angles[2] = atan2(2 * q[2] * q[3] + 2 * q[0] * q[1], -2 * q[1] * q[1] - 2 * q[2] * q[2] + 1)* 180/M_PI; // roll       -pi-----pi  
		MPU6050_Real_Data.Gyro_Z = mygetqval[5]/180.f;
//		MPU6050_Real_Data.Gyro_X = mygetqval[3]/180.f;
//  	MPU6050_Real_Data.Gyro_Y = -mygetqval[4]/180.f;

}

static int yaw_count = 0;
void GetPitchYawGxGyGz()
{


	last_yaw_temp = yaw_temp;
	yaw_temp = angle[0]; 
	if(yaw_temp-last_yaw_temp>=330)  //yaw��ǶȾ����������������
	{
		yaw_count--;
	}
	else if (yaw_temp-last_yaw_temp<=-330)
	{
		yaw_count++;
	}
	yaw_angle = yaw_temp ;//+ yaw_count*360;  //yaw��Ƕ�
	pitch_angle = angle[1];
	roll_angle=angle[2];

}


//int16_t Error_Date[80] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
//										10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
//										20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
//										30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
//										40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
//										50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
//										60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
//										70, 71, 72, 73, 74, 75, 76, 77, 78, 79}; 
///*---------------------------------------------------------
//��������:������ֵ�˲��������ǽ��ٶ�ֵ
//��������:��ȥ�����ǽ��ٶȾ�ֹ״̬��ƫ��ֵ��ʹ����ֲ���ƫ��	
//				 ������У׼yaw���ٶ�ֵ
//�������:���������Ľ��ٶ�ֵ	
//										
//�ܼ�Ч��Ҳ������~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~�ǳ���nice��
//������Ҳ���õ��Ļ��ֺ�yaw�ǶȻ�Ư����  (*^__^*) 						
//----------------------------------------------------------*/				
//float Get_Yaw_Angle(int16_t GyroscopeRawDate)
//{
//		u8 i;	
//		int16_t error_date;		//ƫ��ֵ
//		int16_t Correct_Date;	//����ֵ
//		float Correct_Date_Output;//�������ֵ
//	
//		for(i = 0; i <50; i++)
//		{
//				if(GyroscopeRawDate ==  Error_Date[i])
//				{
//						error_date = Error_Date[i];
//						break;
//				}
//		}		
//		Correct_Date = GyroscopeRawDate - error_date;
//		
//		if(Correct_Date == 0)		Correct_Date_Output = Correct_Date;
//		if(Correct_Date < 0)		Correct_Date_Output =(float) Correct_Date / 32.8f; //ת�ɶ�ÿ��
//		if(Correct_Date > 0)		Correct_Date_Output =(float) Correct_Date / 32.8f; //ת�ɶ�ÿ��
//		
//		return Correct_Date_Output;
//}


//Initialize the MPU6500
uint8_t MPU6500_Init(void)
{
  uint8_t index = 0;
  uint8_t MPU6500_Init_Data[10][2] = 
  {
    {MPU6500_PWR_MGMT_1,    0x80},      // Reset Device
    {MPU6500_PWR_MGMT_1,    0x03},      // Clock Source - Gyro-Z
    {MPU6500_PWR_MGMT_2,    0x00},      // Enable Acc & Gyro
    {MPU6500_CONFIG,        0x02},      // LPF 98Hz
    {MPU6500_GYRO_CONFIG,   0x10},      // +-1000dps
    {MPU6500_ACCEL_CONFIG,  0x10},      // +-2G
    {MPU6500_ACCEL_CONFIG_2,0x02},      // enable LowPassFilter  Set Acc LPF
    {MPU6500_USER_CTRL,     0x20},      // Enable AUX
  };
  
  delay_ms(100);
  MPU_id = MPU6500_Read_Reg(MPU6500_WHO_AM_I);  //read id of device,check if MPU6500 or not
  
  for(index = 0; index < 10; index++)
  {
    MPU6500_Write_Reg(MPU6500_Init_Data[index][0], MPU6500_Init_Data[index][1]);
    delay_ms(1);
  }

  return 0;
}


