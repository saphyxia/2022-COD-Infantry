#include "state_task.h"
#include "iwdg.h"
#include "oled.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"

SYSTEM_STATE System_State;//系统状态

#if INCLUDE_uxTaskGetStackHighWaterMark
uint32_t state_high_water;
#endif
/**
 * @brief 状态机任务
 * @param 任务间隔：SYSTEM_STATE_TASK_TIM
 */
void state_task(void *pvParameters)
{
  uint32_t currentTime;

	vTaskDelay(TIME_STAMP_200MS);
	while(1)
	{
    currentTime = xTaskGetTickCount();//当前系统时间

//		State_Err_judge(&System_State);//系统异常判断

//		if(System_State.Remote[Remote_StateNow]==1)//若遥控器通信异常
//		{
////			Chassis_Stop();//底盘刹车

//			if(System_State.Remote[Remote_StateLast]==0)//若上一次通信正常，视为遥控器掉线
//			{
//				Remote_Protect(&rc_ctrl);//遥控器数据保护
//			}
//      //否则视为遥控器未连接，若有任何其他异常
//			else if(System_State.Usart.USART_State||System_State.Can1.CAN1_State||System_State.Chassis_Motor.Motor_State)
//			{
//				TIM_SetCompare1(TIM12,300);//蜂鸣器报警
////				OLED_Report();//OLED显示异常
//			}
//		}
//		System_State.Remote[Remote_StateLast]=System_State.Remote[Remote_StateNow];//刷新遥控器通信情况

    vTaskDelayUntil(&currentTime,TIME_STAMP_2MS);//绝对延时
#if INCLUDE_uxTaskGetStackHighWaterMark
      state_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
	}
}

/**
  * @name   State_Err_judge
  * @brief  系统异常判断
  * @param  *p:系统状态结构体
  * @attention none
*/
void State_Err_judge(SYSTEM_STATE *p)
{
  /* 地盘电机及CAN通信 */
  for(int i = Left_Font; i < Chassis_Motor_NUM; i++)
  {
    p->Can1.CAN1_Receive[i]=Judge_Re_Time(p->_Re_Time.CAN1_RE_Time[i]);//地盘CAN通信
		p->Chassis_Motor.CHASSISMotor_State[i]=Judge_IF_NAN(Chassis[i].pid[Chassis[i].state].middle.T_OUT)	//pid输出NAN
                                          |(Chassis[i].Data.temperature>=80)	//电机温度大于80度
                                          |(Chassis[i].Data.current>=2048);	//电流大于等于2.5A
  }
  /* 串口通信 */
	for(int i= UART_6; i < USART_NUM; i++)
	{
		p->Usart.Irqhandler_State[i]=Judge_Re_Time(p->_Re_Time.USART_RE_time[i]);
	}
  /* 遥控器通信 */
	p->Remote[Remote_StateNow]=Judge_Re_Time(p->_Re_Time.Remote_Re_Time);
  /* 系统喂狗 */
  if(!p->Chassis_Motor.Motor_State)//若电机正常工作
  {
   IWDG_Feed();//喂狗
  }
}
/**
  * @name   Chassis_Stop
  * @brief  底盘刹车
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
  * @brief  遥控器数据保护
  * @param  *p:遥控器数据结构体
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
  * @brief  通信时间判断
  * @param  _Re_Time:通信时间，为当前系统时间+100
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
  * @brief  NAN判断
  * @param  _Output:最终PID输出
  * @attention none
*/
bool Judge_IF_NAN(float _Output)
{
  bool res=true;

  res = (bool)__ARM_isnan(_Output);

  return res;
}


