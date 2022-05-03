#include "supercap_task.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "CAN_Receive.h"
#include "task.h"

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t supercap_high_water;
#endif

float POWER_Limit=35;//45,50,55Ѫ������ 60��80��100��������(��ƽ�����)
float POWER_DATA[POWER_DATA_NUM]={[ENERGY_Buffer]=60.f};
uint8_t Energy_Cnt=0,Buffer_Cnt,Judge_Power_limit=0;

void supercap_task(void *pvParameters)
{
  uint32_t currentTime;

	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{
    currentTime = xTaskGetTickCount();//��ǰϵͳʱ��

  /*�������ݶ�ȡ*/
   POWER_DATA[VOLTAGE]=(float)INA219_GET.voltage_mv()*INA_V_LSB/1000;
   POWER_DATA[CURRENT]=(float)INA219_GET.current_ma()*IAN_I_LSB/1000;
   POWER_DATA[POWER]  =(float)INA219_GET.power_mw()*INA_Power_LSB/1000;
  
   if(rx_power[VOLTAGE]<=12.5f)//���ݵ�ѹ����12.5V
   {
      Judge_Power_limit=Charging_Mode;//���ģʽ
   }
   else if(POWER_DATA[ENERGY_Buffer]<15.f&&(POWER_DATA[POWER]>POWER_Limit))//�������һ���������������
   {
      Judge_Power_limit=Throttling_Mode;//����ģʽ
   }
   else
   {
     Judge_Power_limit=Output_Mode;//���ģʽ
   }

    vTaskDelayUntil(&currentTime,TIME_STAMP_2MS);
#if INCLUDE_uxTaskGetStackHighWaterM9ark
      supercap_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
	}
}






//    if(Energy_Cnt<50)
//    {
//      if(POWER_Limit < POWER_DATA[POWER])//�������Ļ�������
//      {
//        POWER_DATA[ENERGY_Buffer]-=(POWER_DATA[POWER]-POWER_Limit)*0.002f;//����������������
//        POWER_DATA[ENERGY_Consume]+=(POWER_DATA[POWER]-POWER_Limit)*0.002f;//����������50��(100ms),�ٷ����̹��ʼ��Ƶ��Ϊ10Hz
//        Energy_Cnt++;
//        Buffer_Cnt=0;
//      }else{//�����Ļ�������
//        if(Buffer_Cnt<50){Buffer_Cnt++;}
//        else if(Buffer_Cnt==50){
//          POWER_DATA[ENERGY_Buffer]=60.0f;//һ������������δ���Ļ�������,�ָ�60J
//          Buffer_Cnt=0;
//        }
//				Energy_Cnt=0;
//				POWER_DATA[ENERGY_Consume]=0;//��������0
//				if(rx_power[1]>12.5f||POWER_DATA[POWER]<(POWER_Limit-2))Current_Update=0;//�����ݵ�ѹ����12.5V��δ�����ʣ���Ϊ����Ҫ��������
//      }
//    }else if(Energy_Cnt==50)
//    {
//      if(POWER_DATA[ENERGY_Buffer]-POWER_DATA[ENERGY_Consume]<=7.0f)//Ԥ����һ��100ms�󻺳�����ʣ��С��7J
//      {
//        Current_Update=1;//��Ϊ��Ҫ��������
//      }
//			POWER_DATA[ENERGY_Consume]=0;//��������0
//      Energy_Cnt=0;//������һ�����ڲ���
//    }
//		if(rx_power[1]<=12.5f||POWER_DATA[POWER]>=(POWER_Limit-2)){Current_Update=1;}//�����ݵ�ѹС��12.5V�򳬹��ʣ���Ϊ��Ҫ��������

