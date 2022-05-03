#include "Irqhandler.h"
#include "usart.h"
#include "gyro.h"
#include "state_task.h"
#include "referee.h"
#include "crc.h"

/**
  * @name   USART6_IRQHandler
  * @brief  串口6中断函数
  * @param  none
  * @retval none
  * @attention none
*/
void USART6_IRQHandler(void)
{
//    if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
//    {
//        System_State._Re_Time.USART_RE_time[UART_6]=xTaskGetTickCount()+100;
//        USART_SendData(USART6, TxBuffer[TxCounter++]); 
//        USART_ClearITPendingBit(USART6, USART_IT_TXE);
//        if(TxCounter == count) USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
//    }
//    else 
//		if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
//    {
//        readdata=USART6->DR;//处理数据
//    }
}

/**
  * @name   USART7_IRQHandler
  * @brief  串口7中断函数
  * @param  none
  * @retval none
  * @attention none
*/
void UART7_IRQHandler(void)
{
    if(USART_GetITStatus(UART7, USART_IT_RXNE) != RESET)  //接收中断
    {
        System_State._Re_Time.USART_RE_time[UART_7]=xTaskGetTickCount()+100;
        referee_system_data[referee_system_num]=USART_ReceiveData(UART7);
        if((referee_system_data[0]!=0xA5)&&(referee_system_num==0))
        { 
            return;   //数据头不对，则重新开始寻找0x55数据头
        }
        referee_system_num++;
        if(referee_system_num>6) REFEREE_SYSTEM_ANALYSIS();		
    } 
}

/**
  * @name   UART8_IRQHandler
  * @brief  串口8中断函数
  * @param  none
  * @retval none
  * @attention none
*/
void UART8_IRQHandler(void)
{
  if(USART_GetITStatus(UART8, USART_IT_RXNE) != RESET)  //接收中断
	{
        System_State._Re_Time.USART_RE_time[UART_8]=xTaskGetTickCount()+100;
		USART_ClearITPendingBit(UART8,USART_FLAG_RXNE);//清除标志位，如出现错误即刻删除
		Receive_Uart8_Buff[power.get_count]=USART_ReceiveData(UART8);
		if((Receive_Uart8_Buff[0]!=0x01)&&(power.get_count==0))  return;   //头帧不对，则重新开始寻找数据头
		power.get_count++;//头帧校验完毕
		if ( power.get_count >6)//检测数据6次 
		{
			power.get_count = 0;//清空接收次数		
			if((flag_uart8_conversion_rw==1) && (Receive_Uart8_Buff[0] == 0x01) && (Receive_Uart8_Buff[1] == 0x03) && (Receive_Uart8_Buff[2] == 0x04))
            {//主机读取输出电压和输出电流值时的从机反馈数据
                power.current_U = (float)((int16_t)Receive_Uart8_Buff[4] | ((int16_t)Receive_Uart8_Buff[3] << 8));
                power.current_I = (float)((int16_t)Receive_Uart8_Buff[6] | ((int16_t)Receive_Uart8_Buff[5] << 8));
                power.current_U /= 100.0;//电压精度为0.01V
                power.current_I /= 1000.0;//电流精度为0.001A
                Super_C_Power=power.current_I*power.current_U;//电容功率
                flag_uart8_conversion_rw=0;//置位为0，设定电流电压
            } 		
			if((flag_uart8_conversion_rw==0) && (Receive_Uart8_Buff[0] == 0x01) && (Receive_Uart8_Buff[1] == 0x10) && (Receive_Uart8_Buff[2] == 0x00) && (Receive_Uart8_Buff[3] == 0x00))
            {//主机设定电压和电流值时的从机反馈数据
                flag_uart8_conversion_rw=1;//置位为1，读取电流电压
            }
		}
	}
		
	if(USART_GetITStatus(UART8, USART_IT_TXE) != RESET) //发送寄存器为空(后需关闭发送中断)
	 {    
        if(flag_uart8_conversion_rw==1)
        {
            UART8_SendByte_r();//主机发送，读取输入电压和输出电流显示值
        }
        if(flag_uart8_conversion_rw==0)
        {  
//            UART8_SendByte_w(23.55,C_Power_PID_Realize(referee_robot_status.chassis_power_limit));//主机发送，设定电流电压值
        }
	 }
}





