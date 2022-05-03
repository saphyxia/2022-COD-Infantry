#include "supercap_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "CAN_Receive.h"
#include "task.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t supercap_high_water;
#endif

float POWER_Limit=35;//45,50,55血量优先 60，80，100功率优先(或平衡底盘)
float POWER_DATA[POWER_DATA_NUM]={[ENERGY_Buffer]=60.f};
uint8_t Energy_Cnt=0,Buffer_Cnt,Judge_Power_limit=0;

void supercap_task(void *pvParameters)
{
  uint32_t currentTime;

	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{
    currentTime = xTaskGetTickCount();//当前系统时间

  /*功率数据读取*/
   POWER_DATA[VOLTAGE]=(float)INA219_GET.voltage_mv()*INA_V_LSB/1000;
   POWER_DATA[CURRENT]=(float)INA219_GET.current_ma()*IAN_I_LSB/1000;
   POWER_DATA[POWER]  =(float)INA219_GET.power_mw()*INA_Power_LSB/1000;
  
   if(rx_power[VOLTAGE]<=12.5f)//电容电压低于12.5V
   {
      Judge_Power_limit=Charging_Mode;//充电模式
   }
   else if(POWER_DATA[ENERGY_Buffer]<15.f&&(POWER_DATA[POWER]>POWER_Limit))//超功率且缓冲能量余量不足
   {
      Judge_Power_limit=Throttling_Mode;//节流模式
   }
   else
   {
     Judge_Power_limit=Output_Mode;//输出模式
   }

    vTaskDelayUntil(&currentTime,TIME_STAMP_2MS);
#if INCLUDE_uxTaskGetStackHighWaterM9ark
      supercap_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
	}
}






//    if(Energy_Cnt<50)
//    {
//      if(POWER_Limit < POWER_DATA[POWER])//若需消耗缓冲能量
//      {
//        POWER_DATA[ENERGY_Buffer]-=(POWER_DATA[POWER]-POWER_Limit)*0.002f;//缓冲能量减消耗量
//        POWER_DATA[ENERGY_Consume]+=(POWER_DATA[POWER]-POWER_Limit)*0.002f;//消耗量采样50次(100ms),官方底盘功率检测频率为10Hz
//        Energy_Cnt++;
//        Buffer_Cnt=0;
//      }else{//不消耗缓冲能量
//        if(Buffer_Cnt<50){Buffer_Cnt++;}
//        else if(Buffer_Cnt==50){
//          POWER_DATA[ENERGY_Buffer]=60.0f;//一整个采样周期未消耗缓冲能量,恢复60J
//          Buffer_Cnt=0;
//        }
//				Energy_Cnt=0;
//				POWER_DATA[ENERGY_Consume]=0;//消耗量置0
//				if(rx_power[1]>12.5f||POWER_DATA[POWER]<(POWER_Limit-2))Current_Update=0;//若电容电压大于12.5V或未超功率，认为不需要电流分配
//      }
//    }else if(Energy_Cnt==50)
//    {
//      if(POWER_DATA[ENERGY_Buffer]-POWER_DATA[ENERGY_Consume]<=7.0f)//预测下一个100ms后缓冲能量剩余小于7J
//      {
//        Current_Update=1;//认为需要电流分配
//      }
//			POWER_DATA[ENERGY_Consume]=0;//消耗量置0
//      Energy_Cnt=0;//进行下一次周期采样
//    }
//		if(rx_power[1]<=12.5f||POWER_DATA[POWER]>=(POWER_Limit-2)){Current_Update=1;}//若电容电压小于12.5V或超功率，认为需要电流分配

