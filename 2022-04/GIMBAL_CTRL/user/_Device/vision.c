#include "FreeRTOS_Init.h"
#include "vision.h"
#include "vision_Task.h"

TX2 tx2={
    .isFind=0,
    .color_Flag = 0,//蓝色0绿色1红色2
    .kf_Flag = 0,
};
float yaw_signed,pit_signed;
float pit_unsigned =0;
float yaw_unsigned=0;
uint32_t findtime;
uint8_t vision_fire = 0.f;
uint8_t sum_check=0;
float distance;
int jjj;
void Vision_Check_Data(uint8_t *ReadFromUsart)
{
    if(ReadFromUsart[0]==0xAA&&ReadFromUsart[11]==0x54)
	{
		//加和校验
		sum_check = ReadFromUsart[1]+ReadFromUsart[2]+ReadFromUsart[3]+ReadFromUsart[4]
				  + ReadFromUsart[5]+ReadFromUsart[6]+ReadFromUsart[7];
		if(sum_check!=ReadFromUsart[8]) return;
		//识别到目标
		if(ReadFromUsart[1]==1)
		{
			findtime = xTaskGetTickCount();
			tx2.isFind=1; 
			 pit_unsigned = ReadFromUsart[4] + ReadFromUsart[5]*0.01f;
			 yaw_unsigned = ReadFromUsart[2] + ReadFromUsart[3]*0.01f;
			 vision_fire = ReadFromUsart[7] ;
			distance= ReadFromUsart[9]+((int)(ReadFromUsart[10]*0.1))*0.1f;

            switch (ReadFromUsart[6])
            {
            case 1://++
                pit_signed = -pit_unsigned;
                yaw_signed = -yaw_unsigned;
                break;
            case 2://+-
                pit_signed = -pit_unsigned;
                yaw_signed = +yaw_unsigned;
                break;
            case 3://--
                pit_signed = +pit_unsigned;
                yaw_signed = +yaw_unsigned;
                break;
            case 4://-+
                pit_signed = +pit_unsigned;
                yaw_signed = -yaw_unsigned;
                break;
            default:
                break;
            }
//            static float arr[2][8];
//            float sum[2]={0};
//            arr[0][0] = yaw_signed;
//            arr[1][0] = pit_signed;

//            for(int i=7;i>0;i--){
//                arr[0][i] = arr[0][i-1];
//                arr[1][i] = arr[1][i-1];
//                sum[0] += arr[0][i];
//                sum[1] += arr[1][i];
//            }
//            sum[0] += arr[0][0];
//            sum[1] += arr[1][0];

            tx2.yaw_Err[0] = yaw_signed;
            tx2.pit_Err[0] = pit_signed;
        }
        else{
            if(xTaskGetTickCount()-findtime>256){
                tx2.isFind = 0;
                tx2.yaw_Err[0] = 0;
                tx2.pit_Err[0] = 0;
            }
		}
        tx2.isUpdata=1;//视觉数据更新完成
    }
}

