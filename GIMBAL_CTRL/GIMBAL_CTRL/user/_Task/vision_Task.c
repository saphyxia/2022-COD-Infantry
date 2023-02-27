/*视觉计算任务

*/

#include "vision_Task.h"


/*--------------------------   变量声明   ------------------------------*/

VISION vision={
    .delta_T=0,
    .imu=&imu,
    .tx2=&tx2,
    .send2uart.sign_Flag=0,
};

kalman_filter_t KF_Vision[2];
kalman_filter_init_t KF_Param_Vision[2]={
    //YAW
    [0]={
        .P_data = {2, 0, 0, 2},
        .A_data = {1, 0.001/*0.001*/, 0, 1},//采样时间间隔
        .H_data = {1, 0, 0, 1},
        .Q_data = {1, 0, 0, 1},
        .R_data = {500, 0, 0, 900}//500 1000
    },
    //PITCH
    [1]={
        .P_data = {2, 0, 0, 2},
        .A_data = {1, 0.001/*0.001*/, 0, 1},//采样时间间隔
        .H_data = {1, 0, 0, 1},
        .Q_data = {1, 0, 0, 1},
        .R_data = {200, 0, 0, 400}
    }
};

float k2=1.f,k3=1.f;
float yaw_Predict;

//这两个值在云台不动哨兵动情况下应发生较大变化（随tx2值）而在哨兵不动云台动情况下几乎不发生变化。
//装甲板位置0y1p
float armor_position2vision[2];
//装甲板速度0y1p
float armor_velocity2vision[2];
//预测量0y1p
float predictValue[2];
/*--------------------------   变量声明   ------------------------------*/

//调度任务
void VISION_TASK(void *args)
{
    uint32_t currentTime;

    /*自瞄卡尔曼滤波,二阶*/
	mat_init(&KF_Vision[0].Q,2,2, KF_Param_Vision[0].Q_data);
	mat_init(&KF_Vision[0].R,2,2, KF_Param_Vision[0].R_data);
	kalman_filter_init(&KF_Vision[0], &KF_Param_Vision[0]);
	
	mat_init(&KF_Vision[1].Q,2,2, KF_Param_Vision[1].Q_data);
	mat_init(&KF_Vision[1].R,2,2, KF_Param_Vision[1].R_data);
	kalman_filter_init(&KF_Vision[1], &KF_Param_Vision[1]);

	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
	
    //数据更新耗时（最多3ms）
        vision.delta_T = get_delta_time();
    //识别到装甲板
        if(vision.tx2->isFind){
        //求装甲板位置
            vision.KF.measurement = get_measurement();
            armor_position2vision[0] = vision.KF.measurement[0] - vision.tx2->yaw_Err[0];
            armor_position2vision[1] = vision.KF.measurement[1] + vision.tx2->pit_Err[0];
        //求装甲板速度
            vision.KF.armor_Speed = get_armor_speed(armor_position2vision);
            VAL_LIMIT(vision.KF.armor_Speed[0],-900,900);
            VAL_LIMIT(vision.KF.armor_Speed[1],-900,900);
            armor_velocity2vision[0] = vision.KF.armor_Speed[0] + vision.imu->yaw_Gyro*0.5f;
            armor_velocity2vision[1] = vision.KF.armor_Speed[1] + vision.imu->pit_Gyro*0.5f;
        //卡尔曼计算（kalman返回值：0位置，1速度）
            vision.KF.predict[0] = kalman_filter_calc( &KF_Vision[0],
                                                        armor_position2vision[0],
                                                        armor_velocity2vision[0] );
            vision.KF.predict[1] = kalman_filter_calc( &KF_Vision[1],
                                                        armor_position2vision[1],
                                                        armor_velocity2vision[1] );
        //速度处理
            vision.KF.predict[0][1] *= k2;
            VAL_LIMIT(vision.KF.predict[0][1],-5.f,5.f);
            vision.KF.predict[1][1] *= k3;
            VAL_LIMIT(vision.KF.predict[1][1],-5.f,5.f);
        //预测量计算
            float predict_yaw_temp = vision.KF.predict[0][0] + vision.KF.predict[0][1] - armor_position2vision[0];
            float predict_pit_temp = vision.KF.predict[1][0] + vision.KF.predict[1][1] - armor_position2vision[1];
            predictValue[0] = ramp_Float(predictValue[0],predict_yaw_temp,0.05);
            predictValue[1] = ramp_Float(predictValue[1],predict_pit_temp,0.05);
            vision.yaw = vision.tx2->yaw_Err[0] + predictValue[0];
            vision.pit = vision.tx2->pit_Err[0] + predictValue[1];
            yaw_Predict=vision.KF.predict[0][0] + vision.KF.predict[0][1] - armor_position2vision[0];
		}
        
    //把云台世界角(陀螺仪)发给tx2
        send2uart1();
        vTaskDelayUntil(&currentTime, VISION_TASK_TIM);//绝对延时
	}
}


static float* get_armor_speed(float* nowValue)
{
    static float speed[2][2];
    static float preValue[2];

    for(int i=0;i<2;i++){
        speed[1][i] = speed[0][i];
        if(nowValue[i]==preValue[i]){//两帧数据相同
            return speed[1];
        }else{
            speed[0][i] = (nowValue[i]-preValue[i])/vision.delta_T*3600.f;
            preValue[i] = nowValue[i];
        }
    }

    return speed[0];
}

static float* get_measurement(void)
{
    float delta[2];
    static float result[2];
    static float now[2],pre[2];
    
    pre[0] = now[0];
    pre[1] = now[1];
    now[0] = vision.imu->yaw_Angle;
    now[1] = vision.imu->pit_Angle;
    
    for(int i=0;i<2;i++){
        delta[i] = now[i] - pre[i];
        if(delta[i] > 180.f){
            result[i] -= (delta[i]-360.f);
        }
        else if(delta[i] <-180.f){
            result[i] -= (delta[i]+360.f);
        }
        else{
            result[i] -= delta[i];
        }
    }

    return result;
}

static TickType_t get_delta_time(void)
{
    static TickType_t now,pre,delta;
    
//视觉数据已经更新
    if(vision.tx2->isUpdata==1){
        pre = now;
        now = xTaskGetTickCount();
    //视觉数据置为待更新状态
        vision.tx2->isUpdata = 0;
    }
    delta = now - pre;
    VAL_LIMIT(delta,0.f,3.f);
    return delta;
}

static void send2uart1(void)
{
    if(imu.yaw_Angle>0&&imu.pit_Angle>0){//++
        vision.send2uart.sign_Flag=1;
    }
    else if(imu.yaw_Angle>0&&imu.pit_Angle<0){//+-
        vision.send2uart.sign_Flag=2;
    }
    else if(imu.yaw_Angle<0&&imu.pit_Angle>0){//-+
        vision.send2uart.inform[0] = (uint16_t)(-imu.yaw_Angle);
        vision.send2uart.sign_Flag=3;
    }
    else if(imu.yaw_Angle<0&&imu.pit_Angle<0){//--
        vision.send2uart.sign_Flag=4;
    }
    else{
        vision.send2uart.sign_Flag=0;
    }
    
    vision.send2uart.inform[0] = (uint16_t)ABS(imu.yaw_Angle);
    vision.send2uart.inform[1] = (uint16_t)ABS(((int)(imu.yaw_Angle*100.f)%100));
    vision.send2uart.inform[2] = (uint16_t)ABS(imu.pit_Angle);
    vision.send2uart.inform[3] = (uint16_t)ABS(((int)(imu.pit_Angle*100.f)%100));

//发送部分
    UART1_TX_BUF[0] = 0x69;
    UART1_TX_BUF[1] = tx2.color_Flag;
    UART1_TX_BUF[2] = tx2.kf_Flag;
    UART1_TX_BUF[3] = vision.send2uart.inform[0];
    UART1_TX_BUF[4] = vision.send2uart.inform[1];
    UART1_TX_BUF[5] = vision.send2uart.inform[2];
    UART1_TX_BUF[6] = vision.send2uart.inform[3];
    UART1_TX_BUF[7] = vision.send2uart.sign_Flag;
    UART1_TX_BUF[8]= 0x96;
    DMA_Cmd(DMA2_Stream6,ENABLE);
}








