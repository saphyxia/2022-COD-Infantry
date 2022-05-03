#include "state_task.h"
#include "iwdg.h"
#include "oled.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

SYSTEM_STATE System_State;//ϵͳ״̬

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t state_high_water;
#endif
/**
 * @brief ״̬������
 * @param ��������SYSTEM_STATE_TASK_TIM
 */
void state_task(void *pvParameters)
{
  uint32_t currentTime;

	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{
    currentTime = xTaskGetTickCount();//��ǰϵͳʱ��

//		State_Err_judge(&System_State);//ϵͳ�쳣�ж�

//		if(System_State.Remote[Remote_StateNow]==1)//��ң����ͨ���쳣
//		{
////			Chassis_Stop();//����ɲ��

//			if(System_State.Remote[Remote_StateLast]==0)//����һ��ͨ����������Ϊң��������
//			{
//				Remote_Protect(&rc_ctrl);//ң�������ݱ���
//			}
//      //������Ϊң����δ���ӣ������κ������쳣
//			else if(System_State.Usart.USART_State||System_State.Can1.CAN1_State||System_State.Chassis_Motor.Motor_State)
//			{
//				TIM_SetCompare1(TIM12,300);//����������
////				OLED_Report();//OLED��ʾ�쳣
//			}
//		}
//		System_State.Remote[Remote_StateLast]=System_State.Remote[Remote_StateNow];//ˢ��ң����ͨ�����

    vTaskDelayUntil(&currentTime,TIME_STAMP_2MS);//������ʱ
#if INCLUDE_uxTaskGetStackHighWaterMark
      state_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
	}
}

/**
  * @name   State_Err_judge
  * @brief  ϵͳ�쳣�ж�
  * @param  *p:ϵͳ״̬�ṹ��
  * @attention none
*/
void State_Err_judge(SYSTEM_STATE *p)
{
  /* ���̵����CANͨ�� */
  for(int i = Left_Font; i < Chassis_Motor_NUM; i++)
  {
    p->Can1.CAN1_Receive[i]=Judge_Re_Time(p->_Re_Time.CAN1_RE_Time[i]);//����CANͨ��
		p->Chassis_Motor.CHASSISMotor_State[i]=Judge_IF_NAN(Chassis[i].pid[Chassis[i].state].middle.T_OUT)	//pid���NAN
                                          |(Chassis[i].Data.temperature>=80)	//����¶ȴ���80��
                                          |(Chassis[i].Data.current>=2048);	//�������ڵ���2.5A
  }
  /* ����ͨ�� */
	for(int i= UART_6; i < USART_NUM; i++)
	{
		p->Usart.Irqhandler_State[i]=Judge_Re_Time(p->_Re_Time.USART_RE_time[i]);
	}
  /* ң����ͨ�� */
	p->Remote[Remote_StateNow]=Judge_Re_Time(p->_Re_Time.Remote_Re_Time);
  /* ϵͳι�� */
  if(!p->Chassis_Motor.Motor_State)//�������������
  {
   IWDG_Feed();//ι��
  }
}
/**
  * @name   Chassis_Stop
  * @brief  ����ɲ��
  * @param  none
  * @attention none
*/
//void Chassis_Stop(void)
//{
//	int16_t Stop_Chassis[Chassis_Motor_NUM]={0};
//	
//	CAN_Send_Msg(CAN1,0x200,Stop_Chassis);
//}
/**
  * @name   Remote_Protect
  * @brief  ң�������ݱ���
  * @param  *p:ң�������ݽṹ��
  * @attention none
*/
void Remote_Protect(RC_ctrl_t *p)
{
	p->rc.s[0]=0;
  p->rc.s[1]=0;

  p->rc.ch[0]=0;
  p->rc.ch[1]=0;
  p->rc.ch[2]=0;
  p->rc.ch[3]=0;
  p->rc.ch[4]=0;

  p->mouse.x=0;
  p->mouse.y=0;
  p->mouse.z=0;
  p->mouse.press_l=0;
  p->mouse.press_r=0;

  p->key.v=0;
}
/**
  * @name   Judge_Re_Time
  * @brief  ͨ��ʱ���ж�
  * @param  _Re_Time:ͨ��ʱ�䣬Ϊ��ǰϵͳʱ��+100
  * @attention none
*/
bool Judge_Re_Time(uint32_t _Re_Time)
{
  bool res=true;

  res = (bool)(_Re_Time <= xTaskGetTickCount());

  return res;
}
/**
  * @name   Judge_IF_NAN
  * @brief  NAN�ж�
  * @param  _Output:����PID���
  * @attention none
*/
bool Judge_IF_NAN(float _Output)
{
  bool res=true;

  res = (bool)__ARM_isnan(_Output);

  return res;
}


