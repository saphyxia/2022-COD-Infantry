#ifndef INS_Task_H
#define INS_Task_H
#include "FreeRTOS_Init.h"
#include "main.h"
#include "bmi088.h"
#include "bmi088Middleware.h"
#include "BMI088reg.h"
#include "ist8310.h"
#include "ist8310Middleware.h"
#include "TEMP_pid.h"

typedef struct __BMI088_REAL_Data__
{
    float Accel_X;  //转换成实际的X轴加速度，
    float Accel_Y;  //转换成实际的Y轴加速度，
    float Accel_Z;  //转换成实际的Z轴加速度，
    float Temp;     //转换成实际的温度，单位为摄氏度
    float Gyro_X;   //转换成实际的X轴角加速度，
    float Gyro_Y;   //转换成实际的Y轴角加速度，
    float Gyro_Z;   //转换成实际的Z轴角加速度
    float GyroRawData_Z;
    float GyroRawData_Y;
}BMI088_REAL_DATA_t;

/*卡尔曼滤波*/
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


#define INS_YAW_ADDRESS_OFFSET    0
#define INS_PITCH_ADDRESS_OFFSET  1
#define INS_ROLL_ADDRESS_OFFSET   2		

#define TEMPERATURE_PID_KP 1600.0f //温度控制PID的kp
#define TEMPERATURE_PID_KI 0.2f    //温度控制PID的ki
#define TEMPERATURE_PID_KD 0.0f    //温度控制PID的kd

#define TEMPERATURE_PID_MAX_OUT   4500.0f //温度控制PID的max_out
#define TEMPERATURE_PID_MAX_IOUT 4400.0f  //温度控制PID的max_iout

#define BMI088_TEMP_PWM_MAX 5000 //mpu6500控制温度的设置TIM的重载值，即给PWM最大为 MPU6500_TEMP_PWM_MAX - 1

//外部调用变量
		
int  IMU_Working_Detect(void);
void IMU_getValues(volatile float * values); 
void imu_cali_slove(fp32 gyro[3], fp32 accel[3], fp32 mag[3], bmi088_real_data_t *bmi088, ist8310_real_data_t *ist8310);	
void imu_temp_control(fp32 temp);
void gyro_offset_calc(fp32 gyro_offset[3], fp32 gyro[3], uint16_t *offset_time_count);
void INS_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3], uint16_t *time_count);
void INS_set_cali_gyro(fp32 cali_scale[3], fp32 cali_offset[3]);
void cali_data_write(void);
void cali_data_read(void);


#endif




