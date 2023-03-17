/*视觉计算任务

*/

#include "vision_Task.h"
#include "systemState_Task.h"
#include "gimbal_Task.h"
#include "imu_Task.h"

/*--------------------------   变量声明   ------------------------------*/

VISION vision={
    .delta_T=0,
    .imu=&imu,
    .tx2=&tx2,
    .send2uart.sign_Flag=0,
};

bool IS_SENTRY_MODE = false;

////装甲板位置0y1p
//float armor_position2vision[2];
////装甲板速度0y1p
//float armor_velocity2vision[2];
////装甲板加速度0y1p
//float armor_accel2vision[2];
////预测量0y1p
//float predictValue[2];
////是否开启预测
//bool key_KF=false;
//uint16_t KF_Cnt=0;
//uint16_t Fire_Cnt=0;
////预测量控制系数
//float yaw_feed=0.58f,accel_k=0.f,speed_k=0.35f,dis_k=0.45;
////视觉误差更新时的陀螺仪角度
//float imu_yaw_angle;
////开火判断
//bool IF_Fire_Ready=false;
//extKalman_t KF_YAW_FEED,KF_YAW_ACCEL,KF_YAW_DIS,KF_YAW_ANGLE,KF_YAW_SEED;
//QueueObj yaw_Queue_speed={.queueLength=60 },yaw_Queue_accel={.queueLength=60 };
/*--------------------------   变量声明   ------------------------------*/

//调度任务
void VISION_TASK(void *args)
{
  uint32_t currentTime;
	
//	KalmanCreate(&KF_YAW_ANGLE,1,0);
//	KalmanCreate(&KF_YAW_DIS,1,100);
//	KalmanCreate(&KF_YAW_ACCEL,1,10);
//	KalmanCreate(&KF_YAW_SEED,1,100);

	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
	
		vision.delta_T = get_delta_time();    //数据更新耗时
//		if(!vision.tx2->isFind)
//		{
////			key_KF=0;
//		}
		if(vision.tx2->isUpdata && vision.tx2->isFind && vision_Flag)//识别到装甲板并且视觉数据更新
		{
//			if(key_KF!=1)
//			{
//				KF_Cnt++;
//				if(KF_Cnt>=30)
//				{
////					key_KF=1;
//					KF_Cnt=0;
//				}
//			}
//			//求装甲板位置
//			armor_position2vision[0] = KalmanFilter(&KF_YAW_ANGLE,vision.tx2->yaw_Err[0]) + imu_yaw_angle;
//			imu_yaw_angle = vision.imu->yaw_Angle;//回溯视觉误差更新时的陀螺仪角度
//			//求装甲板速度
//			armor_velocity2vision[0] = Get_Diff(2,&yaw_Queue_speed,armor_position2vision[0])*1000.f/(vision.delta_T*1.f);
//			armor_velocity2vision[0] = KalmanFilter(&KF_YAW_SEED,armor_velocity2vision[0]);
//			//VAL_LIMIT(vision.KF.armor_Speed,-3,3);
//			//求装甲板加速度
//			armor_accel2vision[0] = Get_Diff(3,&yaw_Queue_accel,armor_velocity2vision[0])*1000.f/(vision.delta_T*1.f);
//			armor_accel2vision[0] = KalmanFilter(&KF_YAW_ACCEL,armor_accel2vision[0]);
//			VAL_LIMIT(armor_accel2vision[0],-10,10);
//			//距离在有效值
//			if(distance>0.3f&&distance<10.f)
//			{
//				distance = KalmanFilter(&KF_YAW_DIS,distance);
//			}
//			//计算预测量
//			predictValue[0] = accel_k*armor_accel2vision[0]+speed_k*armor_velocity2vision[0];
//			if((distance*dis_k)!=0)
//			{
//				predictValue[0] *= distance*dis_k;//根据距离调整预测量
//			}
//			//火控
//			if((ABS(vision.tx2->yaw_Err[0])<2.f)&&(ABS(vision.tx2->pit_Err[0])<2.f))//视觉误差在有效范围内
//			{
////				if((vision.tx2->yaw_Err[0] * armor_velocity2vision[0])<=0)//装甲板速度与视觉误差异号，判断超前可开火
//				if(ABS(vision.tx2->yaw_Err[0])<= ABS(predictValue[0]*1.5f) \
//				&& ABS(gimbal.yaw->pid_Angle.middle.Err)<=ABS(predictValue[0]*0.8f))
//				{
//					IF_Fire_Ready=true;
//					Fire_Cnt=0;
//				}
//				else
//				{
//					Fire_Cnt++;
//					if(Fire_Cnt>=30)
//					{
//						IF_Fire_Ready=false;
//						Fire_Cnt=0;
//					}
//				}
//			}else{
//					IF_Fire_Ready=false;
//			}
		}
//		//使用预测量补偿视觉误差
//		vision.yaw = vision.tx2->yaw_Err[0]+predictValue[0]*yaw_feed;
//		vision.tx2->isUpdata = 0;
    //把云台世界角(陀螺仪)发给tx2
		if(Key_X()==false)
		{
			tx2.kf_Flag = 0;//地面
			IS_SENTRY_MODE = false;
		}else if(Key_X()==true)
		{
			tx2.kf_Flag = 1;//高地
			IS_SENTRY_MODE = true;
		}
		send2uart1();
		vTaskDelayUntil(&currentTime, VISION_TASK_TIM);//绝对延时
	}
}
/**
* @brief  获取目标的差分
* @param  uint8_t QueueObj float
* @return float
*	以队列的逻辑
*/
float Get_Diff(uint8_t queue_len, QueueObj *Data,float add_data)
{
    if(queue_len>=Data->queueLength)
        queue_len=Data->queueLength;
    //防止溢出
    Data->queueTotal-=Data->queue[Data->nowLength];
    Data->queueTotal+=add_data;

    Data->queue[Data->nowLength]=add_data;
			
    Data->nowLength++;

    if(Data->full_flag==0)//初始队列未满
    {
        Data->aver_num=Data->queueTotal/Data->nowLength;
    }else if(Data->full_flag == 1)
	{
	    Data->aver_num=(Data->queueTotal)/queue_len;	
		
	}
    if(Data->nowLength>=queue_len)
    {
        Data->nowLength=0;
        Data->full_flag=1;
    }

    Data->Diff=add_data - Data->aver_num;
    return Data->Diff;
}

static TickType_t get_delta_time(void)
{
   static  TickType_t now,pre,delta;
	
    delta = now - pre-1;
    VAL_LIMIT(delta,0.f,15.f);

//视觉数据已经更新
    if(vision.tx2->isUpdata==1){
        pre = now;
        now = xTaskGetTickCount();
    //视觉数据置为待更新状态
    }
    return delta;
}
static void send2uart1(void)
{
		
    UART1_TX_BUF[0]  = 0x69;
    UART1_TX_BUF[1]  = 2;//robot.id;
    UART1_TX_BUF[2]  = 1;
    UART1_TX_BUF[3]  = tx2.kf_Flag;
    UART1_TX_BUF[4]  = (int)ABS(14.5f);
    UART1_TX_BUF[5]  = (int)ABS(14.5f*100)%100;
    UART1_TX_BUF[6]  = (int16_t) (INS_angle[2]);
    UART1_TX_BUF[7]  = (int16_t)((INS_angle[2])*100.f)%100;
		UART1_TX_BUF[8]  = (int16_t)((INS_angle[2])*10000.f)%100;
    UART1_TX_BUF[9]  = (int16_t)(-INS_angle[0]);
    UART1_TX_BUF[10] = (int16_t)((-INS_angle[0])*100.f)%100;
    UART1_TX_BUF[11] = (int16_t)((-INS_angle[0])*10000.f)%100;
		UART1_TX_BUF[12] = (UART1_TX_BUF[1]+UART1_TX_BUF[2]+UART1_TX_BUF[3] + UART1_TX_BUF[4]+UART1_TX_BUF[5] + UART1_TX_BUF[6]
					 + UART1_TX_BUF[7]+UART1_TX_BUF[8] + UART1_TX_BUF[9]+UART1_TX_BUF[10]+UART1_TX_BUF[11])/11;
	
		UART1_TX_BUF[13] = 0x96;
		
    DMA_Cmd(DMA2_Stream6,ENABLE);
}








