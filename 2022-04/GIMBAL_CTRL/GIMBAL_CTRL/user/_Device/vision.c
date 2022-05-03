#include "FreeRTOS_Init.h"
#include "vision.h"

TX2 tx2={
    .isFind=0,
    .color_Flag = 1,//蓝色
    .kf_Flag = 0,
};
float yaw_signed,pit_signed;
uint32_t findtime;
void Vision_Check_Data(uint8_t *ReadFromUsart)
{
    if(ReadFromUsart[0]==0xAA&&ReadFromUsart[9]==0x54){
        if(ReadFromUsart[1]==1){//识别到目标
            findtime = xTaskGetTickCount();
			tx2.isFind=1; 
            
            float pit_unsigned = ReadFromUsart[2] + ReadFromUsart[3]*0.01f;
            float yaw_unsigned = ReadFromUsart[4] + ReadFromUsart[5]*0.01f;
            float distance = ReadFromUsart[7];

            switch (ReadFromUsart[7])
            {
            case 1://++
                yaw_signed =-yaw_unsigned;
                pit_signed =-pit_unsigned;
                break;
            case 2://++
                yaw_signed = yaw_unsigned;
                pit_signed =-pit_unsigned;
                break;
            case 3://-+
                yaw_signed =-yaw_unsigned;
                pit_signed = pit_unsigned;
                break;
            case 4://--
                yaw_signed = yaw_unsigned;
                pit_signed = pit_unsigned;
                break;
            default:
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

            tx2.yaw_Err[0] = sum[0]/8.f;
            tx2.pit_Err[0] = sum[1]/8.f;
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

