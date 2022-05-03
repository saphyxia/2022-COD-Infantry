#include "FreeRTOS_Init.h"
#include "vision.h"

TX2 tx2={
    .isFind=0,
    .color_Flag = 1,//1 blue  2 red
    .kf_Flag = 0,
};
int cas_8,cas_3;
float yaw_signed,pit_signed;
uint32_t findtime;
int distance=0;
void Vision_Check_Data(uint8_t *ReadFromUsart)
{
    if(ReadFromUsart[0]==0xAA&&ReadFromUsart[9]==0x54){
        if(ReadFromUsart[1]==1){//识别到目标
            findtime = xTaskGetTickCount();
						tx2.isFind=1; 
            
            float pit_unsigned = ReadFromUsart[2] + ReadFromUsart[3]*0.01f;
            float yaw_unsigned = ReadFromUsart[4] + ReadFromUsart[5]*0.01f;
									distance = (uint16_t)ReadFromUsart[6] <<8 |(uint16_t)ReadFromUsart[7];
            switch (ReadFromUsart[8])
            {
            case 1://++
                pit_signed =  pit_unsigned;
                yaw_signed =  yaw_unsigned;
                break;
            case 2://-+
                pit_signed = -pit_unsigned;
                yaw_signed = yaw_unsigned;
                break;
            case 3://--
                pit_signed = -pit_unsigned;
                yaw_signed = -yaw_unsigned;
                break;
            case 4://+-
                pit_signed =  pit_unsigned;
                yaw_signed = -yaw_unsigned;
                break;
            default:
//							yaw_signed = 0;
//							pit_signed = 0 ;
                break;
            }
            static float arr[2][8];
            float sum[2]={0};
            arr[0][0] = yaw_signed;
            arr[1][0] = pit_signed;

            for(int i=7;i>0;i--){
                arr[0][i] = arr[0][i-1];
                arr[1][i] = arr[1][i-1];
                sum[0] += arr[0][i];
                sum[1] += arr[1][i];
            }
            sum[0] += arr[0][0];
            sum[1] += arr[1][0];

            tx2.yaw_Err[0] = yaw_signed;//sum[0]/8.f;
            tx2.pit_Err[0] = pit_signed;//sum[1]/8.f;
						cas_3 = ReadFromUsart[2];
						cas_8 = ReadFromUsart[8];
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

