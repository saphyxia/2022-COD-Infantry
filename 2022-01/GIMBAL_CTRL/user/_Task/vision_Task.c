/*视觉计算任务

*/

#include "FreeRTOS_Init.h"
#include "vision_Task.h"
#include "vision.h"
#include "uart1.h"



/*--------------------------   变量声明   ------------------------------*/
int buf_Count=0;
float forecast_K=1.075;
float vision_Info_Buf[VISION_INFO_BUF_LEN][GIMBAL_MOTOR_NUM];
float Vision_Angle[GIMBAL_MOTOR_NUM];
float Vision_Angle_L[GIMBAL_MOTOR_NUM];
float Vision_Forecast[GIMBAL_MOTOR_NUM];
float Vision_Forecast_L[GIMBAL_MOTOR_NUM];
extKalman_t KF_Vision[GIMBAL_MOTOR_NUM];
extKalman_t KF_Vision_Forecast[GIMBAL_MOTOR_NUM];

kalman_filter_t KF_Filter_Vision_Gimbal[GIMBAL_MOTOR_NUM];
kalman_filter_init_t KF_Filter_Param_Vision_Gimbal[GIMBAL_MOTOR_NUM]={
    [YAW  ]={
        .P_data = {2, 0, 0, 2},
        .A_data = {1, 0.001/*0.001*/, 0, 1},//采样时间间隔
        .H_data = {1, 0, 0, 1},
        .Q_data = {1, 0, 0, 1},
        .R_data = {200, 0, 0, 400}//500 1000
    },
    [PITCH]={
        .P_data = {2, 0, 0, 2},
        .A_data = {1, 0.001/*0.001*/, 0, 1},//采样时间间隔
        .H_data = {1, 0, 0, 1},
        .Q_data = {1, 0, 0, 1},
        .R_data = {200, 0, 0, 400}
    }
};

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void VISION_TASK(void *args)
{
    uint32_t currentTime;
    
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间
//		if(buf_Count>=VISION_INFO_BUF_LEN){
//            buf_Count=0;
//        }
//        //视觉信息缓存(最新帧count%VISION_INFO_BUF_LEN, 最旧帧(count+1)%VISION_INFO_BUF_LEN)
////        vision_Info_Buf[buf_Count%VISION_INFO_BUF_LEN][YAW  ]=vision_Info[YAW  ];
////        vision_Info_Buf[buf_Count%VISION_INFO_BUF_LEN][PITCH]=vision_Info[PITCH];

////        if(vision.tx2->isFind){
//				if(0){
//            Vision_Angle[YAW  ]=KalmanFilter(&KF_Vision[YAW  ],vision_Info_Buf[buf_Count%VISION_BUFFER_LEN][YAW  ]) + Gimbal[YAW  ].Data.angle;
//            Vision_Angle[PITCH]=-KalmanFilter(&KF_Vision[PITCH],vision_Info_Buf[buf_Count%VISION_BUFFER_LEN][PITCH]) + Gimbal[PITCH].Data.angle;
//        } else {
//            Vision_Angle[YAW  ]=Gimbal[YAW  ].Data.angle;
//            Vision_Angle[PITCH]=Gimbal[PITCH].Data.angle;
//        }
//        VAL_LIMIT(Vision_Angle[PITCH],3,50);
//    
//    //误差更新，注意只会涉及到AI开头的类别。
//        Gimbal_PID_Err[YAW  ][AI_STAN] = Vision_Angle[YAW  ] - Gimbal[YAW  ].Data.angle;
//        Gimbal_PID_Err[PITCH][AI_STAN] = Vision_Angle[PITCH] - Gimbal[PITCH].Data.angle;
//				
//			UART1_TX_BUF[0] = 0x69;
//			UART1_TX_BUF[1] = tx2.color_Flag;
//			UART1_TX_BUF[2] = tx2.kf_Flag;
//			UART1_TX_BUF[3] = 0;
//			UART1_TX_BUF[4] = 0;
//			UART1_TX_BUF[5] = 0;
//			UART1_TX_BUF[6] = 0;
//			UART1_TX_BUF[7] = 0x96;
////			UART1_TX_BUF[8] = 0;
////			UART1_TX_BUF[9]= 0x96;
//			DMA_Cmd(DMA2_Stream6,ENABLE);
//				
        vTaskDelayUntil(&currentTime, VISION_TASK_TIM);//绝对延时
	}
}


