#include "FreeRTOS_Init.h"
#include "imu_Task.h"
#include "assist.h"
#include "arm_math.h"

//线性校准标量
#define BMI088_BOARD_INSTALL_SPIN_MATRIX    \
    {0.0f, 1.0f, 0.0f},                     \
    {-1.0f, 0.0f, 0.0f},                    \
    {0.0f, 0.0f, 1.0f}                      \


#define IST8310_BOARD_INSTALL_SPIN_MATRIX   \
    {1.0f, 0.0f, 0.0f},                     \
    {0.0f, 1.0f, 0.0f},                     \
    {0.0f, 0.0f, 1.0f}                      \

#define BMI088_TEMPERATURE_PWM_INIT() TIM10_init() //陀螺仪温度控制PWM初始化
#define IMUTempPWM(pwm) TIM_SetCompare1(TIM10, (pwm))                      //pwm给定
#define INS_GET_CONTROL_TEMPERATURE() get_control_temperate()             //获取控制温度的目标值
		
#define M_PI  (fp32)3.141592653589f
static const float timing_time = 0.000998f;   //tast run time , unit s.任务运行的时间 单位 s
volatile float mygetqval[9];	//用于存放传感器转换结果的数组

bmi088_real_data_t bmi088_real_data;
ist8310_real_data_t ist8310_real_data;

 uint8_t first_temperate;
 fp32 imu_temp_PID[3] = {TEMPERATURE_PID_KP, TEMPERATURE_PID_KI, TEMPERATURE_PID_KD};
 pid_type_def imu_temp_pid;		
		
fp32 INS_gyro[3] = {0.0f, 0.0f, 0.0f};
fp32 INS_accel[3] = {0.0f, 0.0f, 0.0f};
fp32 INS_mag[3] = {0.0f, 0.0f, 0.0f};
fp32 INS_quat[4] = {0.0f, 0.0f, 0.0f, 0.0f};
fp32 INS_angle[3] = {0.0f, 0.0f, 0.0f};      //euler angle, unit rad.欧拉角 单位 rad   

fp32 BMI088_FIFO[6][101] = {0};//[0]-[90]为最近10次数据 [100]为10次数据的平均值	
BMI088_REAL_DATA_t  BMI088_read_data;

fp32 gyro_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};  //校准变量
fp32 gyro_offset[3];
fp32 gyro_cali_offset[3];

fp32 cali_scale[3];  //校准变量，主要这里为了方便观察，这个全局变量与校准函数里面的局部变量一样，但意义有所差别
fp32 cali_offset[3];
uint16_t time_count;

fp32 cali_scale_flash[3];  //校准变量，主要这里为了方便观察，这个全局变量与校准函数里面的局部变量一样，但意义有所差别
fp32 cali_offset_flash[3];
uint16_t flash_len[3];
uint8_t  flash_buf[3];

fp32 accel_scale_factor[3][3] = {BMI088_BOARD_INSTALL_SPIN_MATRIX};
fp32 accel_offset[3];
fp32 accel_cali_offset[3];

ist8310_real_data_t ist8310_real_data;
fp32 mag_scale_factor[3][3] = {IST8310_BOARD_INSTALL_SPIN_MATRIX};
fp32 mag_offset[3];
fp32 mag_cali_offset[3];

//加速度计低通滤波
static fp32 accel_fliter_1[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_2[3] = {0.0f, 0.0f, 0.0f};
static fp32 accel_fliter_3[3] = {0.0f, 0.0f, 0.0f};
static const fp32 fliter_num[3] = {1.929454039488895f, -0.93178349823448126f, 0.002329458745586203f};

fp32 eff_x;
fp32 eff_y;
fp32 eff_z;
fp32 gy_z;
float C_Gyo_z;
fp32 Eff[80] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
							  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
							  20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
							  30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 
							  40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
							  50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
							  60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
							  70, 71, 72, 73, 74, 75, 76, 77, 78, 79}; 

kalman_t kalman;
Critical_t Yaw_Gyro_Angle;				 
//外部调用变量   
fp32 yaw_Angel;
fp32 pit_Angel;
fp32 rol_Angel;									 
fp32 yaw_Gyro;
fp32 pit_Gyro;
fp32 rol_Gyro;									 
int cali_flag=0;
int flash_flag =0;
int IMU_Working=0;
void IMU_TASK(void *args)
{
    uint32_t currentTime;
	
    while(BMI088_init())  
    {
        vTaskDelay(100);
    }
    while(ist8310_init())
    {
        vTaskDelay(100);
    }
    BMI088_read(bmi088_real_data.gyro, bmi088_real_data.accel, &bmi088_real_data.temp);
    PID_init(&imu_temp_pid, PID_POSITION, imu_temp_PID, TEMPERATURE_PID_MAX_OUT, TEMPERATURE_PID_MAX_IOUT);
    imu_cali_slove(INS_gyro, INS_accel, INS_mag, &bmi088_real_data, &ist8310_real_data);
    AHRS_init(INS_quat, INS_accel,INS_mag);

    accel_fliter_1[0] = accel_fliter_2[0] = accel_fliter_3[0] = INS_accel[0];
    accel_fliter_1[1] = accel_fliter_2[1] = accel_fliter_3[1] = INS_accel[1];
    accel_fliter_1[2] = accel_fliter_2[2] = accel_fliter_3[2] = INS_accel[2];
		
    SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
		
    while(1)
    {
        currentTime = xTaskGetTickCount();//当前系统时间
			  IMU_Working=1;
        IMU_getValues(mygetqval);
        imu_cali_slove(INS_gyro, INS_accel, INS_mag, &bmi088_real_data, &ist8310_real_data);
        imu_temp_control(bmi088_real_data.temp);

        accel_fliter_1[0] = accel_fliter_2[0];
        accel_fliter_2[0] = accel_fliter_3[0];
        accel_fliter_3[0] = accel_fliter_2[0] * fliter_num[0] + accel_fliter_1[0] * fliter_num[1] + INS_accel[0] * fliter_num[2];

        accel_fliter_1[1] = accel_fliter_2[1];
        accel_fliter_2[1] = accel_fliter_3[1];
        accel_fliter_3[1] = accel_fliter_2[1] * fliter_num[0] + accel_fliter_1[1] * fliter_num[1] + INS_accel[1] * fliter_num[2];

        accel_fliter_1[2] = accel_fliter_2[2];
        accel_fliter_2[2] = accel_fliter_3[2];
        accel_fliter_3[2] = accel_fliter_2[2] * fliter_num[0] + accel_fliter_1[2] * fliter_num[1] + INS_accel[2] * fliter_num[2];			

        AHRS_update(INS_quat,timing_time,INS_gyro,accel_fliter_3,INS_mag);
        get_angle(INS_quat, INS_angle+ INS_YAW_ADDRESS_OFFSET , INS_angle + INS_PITCH_ADDRESS_OFFSET, INS_angle + INS_ROLL_ADDRESS_OFFSET);

//        yaw_Angel = INS_angle[0]*180.0f/3.1415926f;
        rol_Angel = INS_angle[1]*180.0f/3.1415926f;
        pit_Angel = INS_angle[2]*180.0f/3.1415926f;
        pit_Gyro  = INS_gyro [0]*180.0f;
        rol_Gyro  = INS_gyro [1]*180.0f;
        yaw_Gyro  = INS_gyro [2]*180.0f;
				
			  yaw_Angel = Gimbal_Yaw_Gryo_AngleSum(&Yaw_Gyro_Angle , INS_angle[0]*180.0f/3.1415926f);
				
        vTaskDelayUntil(&currentTime, IMU_TASK_TIM);//绝对延时
    }
}

void IMU_getValues(volatile float * values) 
{
    uint8_t i = 0;
    fp32 bmi088_sum0=0;
    fp32 bmi088_sum1=0;
    fp32 bmi088_sum2=0;
    fp32 bmi088_sum3=0;
    fp32 bmi088_sum4=0;
    fp32 bmi088_sum5=0;
    int j;
    int k;
    int l;
    int c;
    BMI088_read(bmi088_real_data.gyro, bmi088_real_data.accel, &bmi088_real_data.temp);   //循环读取加速计、陀螺仪、温度的值
    for(i=1;i<100;i++)
    {
        BMI088_FIFO[0][i-1]=BMI088_FIFO[0][i];
        BMI088_FIFO[1][i-1]=BMI088_FIFO[1][i];
        BMI088_FIFO[2][i-1]=BMI088_FIFO[2][i];
        BMI088_FIFO[3][i-1]=BMI088_FIFO[3][i];
        BMI088_FIFO[4][i-1]=BMI088_FIFO[4][i];
        BMI088_FIFO[5][i-1]=BMI088_FIFO[5][i];
    }
    BMI088_FIFO[0][99] = bmi088_real_data.accel[0];  
    BMI088_FIFO[1][99] = bmi088_real_data.accel[1];
    BMI088_FIFO[2][99] = bmi088_real_data.accel[2];
    BMI088_FIFO[3][99] = bmi088_real_data.gyro[0];
    BMI088_FIFO[4][99] = bmi088_real_data.gyro[1];
    BMI088_FIFO[5][99] = bmi088_real_data.gyro[2];

    for(i=0;i<100;i++)//求当前数组（加速、陀螺仪的100个值）的和，再取平均值
    {	
        c++;
        bmi088_sum0 += BMI088_FIFO[0][i];	
    }
    BMI088_FIFO[0][100] = bmi088_sum0/100;
    bmi088_sum0 = 0;

    for(i=0;i<100;i++)
    {	
        bmi088_sum1 += BMI088_FIFO[1][i];
    }
    BMI088_FIFO[1][100] = bmi088_sum1/100;
    bmi088_sum1 = 0;

    for(i=0;i<100;i++)
    {	
        bmi088_sum2 += BMI088_FIFO[2][i];
    }
    BMI088_FIFO[2][100] = bmi088_sum2/100;
    bmi088_sum2 = 0;

    for(i=0;i<100;i++)
    {	
        bmi088_sum3 += BMI088_FIFO[3][i];
    }
    BMI088_FIFO[3][100] = bmi088_sum3/100;
    bmi088_sum3 = 0;

    for(i=0;i<100;i++)
    {	
        bmi088_sum4 += BMI088_FIFO[4][i];
    }
    BMI088_FIFO[4][100] = bmi088_sum4/100;
    bmi088_sum4 = 0;

    for(i=0;i<100;i++)
    {	
        bmi088_sum5 += BMI088_FIFO[5][i];
    }
    BMI088_FIFO[5][100] = bmi088_sum5/100;
    bmi088_sum5 = 0; 

    BMI088_read_data.Accel_X = BMI088_FIFO[0][100] ;
    BMI088_read_data.Accel_Y = BMI088_FIFO[1][100] ;
    BMI088_read_data.Accel_Z = BMI088_FIFO[2][100] ;	
    BMI088_read_data.Gyro_X = BMI088_FIFO[3][100]  ;
    BMI088_read_data.Gyro_Y = BMI088_FIFO[4][100]  ;
    BMI088_read_data.Gyro_Z = BMI088_FIFO[5][100]  ;	

    for(j = 0; j <50; j++)
    {
        if(BMI088_read_data.Gyro_X ==  Eff[j])
        {
            eff_x = Eff[j];
            break;
        }
        if(-BMI088_read_data.Gyro_X ==  Eff[j])
        {
            eff_x = -Eff[j];
            break;
        }
    }		
    for(k = 0; k <25; k++)
    {
        if(BMI088_read_data.Gyro_Y ==  Eff[k])
        {
            eff_y = Eff[k];
            break;
        }
        if(-BMI088_read_data.Gyro_Y ==  Eff[k])
        {
            eff_y = -Eff[k];
            break;
        }
    }	
    for(l = 0; l<23; l++)
    {
        if(BMI088_read_data.Gyro_Z ==  Eff[l])
        {
            eff_z = Eff[l];
            break;
        }
        if(-BMI088_read_data.Gyro_Z ==  Eff[l])
        {
            eff_z = -Eff[l];
            break;
        }
    }			

    kalman.Xk_accelX = kalman.Xk_accelX;   //先验估计  x(k|k-1) = A*X(k-1|k-1)+B*U(k)+W(K)
    kalman.Q_accelX = 0.018f;
    kalman.R_accelX = 0.542f;
    kalman.pk_accelX = kalman.pk_accelX + kalman.Q_accelX;   //先验误差 p(k|k-1) = A*p(k-1|k-1)*A'+Q
    kalman.Kk_accelX = kalman.pk_accelX / (kalman.pk_accelX + kalman.R_accelX);   //卡尔曼增益 kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
    kalman.Xk_accelX = kalman.Xk_accelX + kalman.Kk_accelX*(BMI088_read_data.Accel_X - kalman.Xk_accelX);  //卡尔曼增益 kg(k) = p(k|k-1)*H'/(H*p(k|k-1)*H'+R)
    kalman.pk_accelX = (1 - kalman.Kk_accelX)*kalman.pk_accelX;	  //状态更新 p(k|k) = (I-kg(k)*H)*P(k|k-1)

    kalman.Xk_accelY = kalman.Xk_accelY;
    kalman.Q_accelY = 0.018f;
    kalman.R_accelY = 0.542f;
    kalman.Pk_accelY = kalman.Pk_accelY + kalman.Q_accelY;
    kalman.Kk_accelY = kalman.Pk_accelY / (kalman.Pk_accelY + kalman.R_accelY);
    kalman.Xk_accelY = kalman.Xk_accelY + kalman.Kk_accelY*(BMI088_read_data.Accel_Y - kalman.Xk_accelY);
    kalman.Pk_accelY = (1 -kalman. Kk_accelY)*kalman.Pk_accelY;

    kalman.Xk_accelZ = kalman.Xk_accelZ;
    kalman.Q_accelZ = 0.018f;
    kalman.R_accelZ = 0.542f;
    kalman.Pk_accelZ = kalman.Pk_accelZ + kalman.Q_accelZ;
    kalman.Kk_accelZ = kalman.Pk_accelZ / (kalman.Pk_accelZ + kalman.R_accelZ);
    kalman.Xk_accelZ = kalman.Xk_accelZ + kalman.Kk_accelZ*(BMI088_read_data.Accel_Z - kalman.Xk_accelZ);
    kalman.Pk_accelZ = (1 -kalman. Kk_accelZ)*kalman.Pk_accelZ;		

    values[0] = kalman.Xk_accelX;
    values[1] = kalman.Xk_accelY;
    values[2] = kalman.Xk_accelZ;

    BMI088_read_data.Gyro_X=BMI088_read_data.Gyro_X - eff_x;
    values[3]=BMI088_read_data.Gyro_X;
    BMI088_read_data.Gyro_Y=BMI088_read_data.Gyro_Y - eff_y;
    values[4]=BMI088_read_data.Gyro_Y;
    BMI088_read_data.Gyro_Z=BMI088_read_data.Gyro_Z - eff_z;
    values[5]=BMI088_read_data.Gyro_Z;


		INS_accel[0] = (9.80f/0.110263973f)*(values[0] -1.912530660f);
		INS_accel[1] = (9.80f/0.0444717556f)*(values[1] -9.94185543f);
		INS_accel[2] = (9.80f/0.05551989f)*(values[2] -3.72850895f);
//    INS_accel[0] = values[0];
//    INS_accel[1] = values[1];
//    INS_accel[2] = values[2];
    INS_gyro[0] =  values[3];
    INS_gyro[1] =  values[4];
    INS_gyro[2] =  values[5];

}

void imu_cali_slove(fp32 gyro[3], fp32 accel[3], fp32 mag[3], bmi088_real_data_t *bmi088, ist8310_real_data_t *ist8310)
{
    for (uint8_t i = 0; i < 3; i++)
    {
        gyro[i] = bmi088->gyro[0] * gyro_scale_factor[i][0] + bmi088->gyro[1] * gyro_scale_factor[i][1] + bmi088->gyro[2] * gyro_scale_factor[i][2] + gyro_offset[i];
        accel[i] = bmi088->accel[0] * accel_scale_factor[i][0] + bmi088->accel[1] * accel_scale_factor[i][1] + bmi088->accel[2] * accel_scale_factor[i][2] + accel_offset[i];
        mag[i] = ist8310->mag[0] * mag_scale_factor[i][0] + ist8310->mag[1] * mag_scale_factor[i][1] + ist8310->mag[2] * mag_scale_factor[i][2] + mag_offset[i];
    }
}

void imu_temp_control(fp32 temp)
{
    uint16_t tempPWM;
    static uint8_t temp_constant_time = 0;
    if (first_temperate)
    {
        PID_calc(&imu_temp_pid, temp, 40.0f);
        if (imu_temp_pid.out < 0.0f)
        {
            imu_temp_pid.out = 0.0f;
        }
        tempPWM = (uint16_t)imu_temp_pid.out;
        IMUTempPWM(tempPWM);
    }
    else
    {
        //在没有达到设置的温度，一直最大功率加热
        if (temp > 40.0f)
        {
            temp_constant_time++;
            if (temp_constant_time > 200)
            {
                //达到设置温度，将积分项设置为一半最大功率，加速收敛
                //
                first_temperate = 1;
                imu_temp_pid.Iout = BMI088_TEMP_PWM_MAX / 2.0f;
            }
        }
        IMUTempPWM(BMI088_TEMP_PWM_MAX - 1);
    }
}












