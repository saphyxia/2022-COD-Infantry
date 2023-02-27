#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
#include "timer.h"
#include "myiic.h"
#include "ina226.h"
 

uint16_t Set_Voltage_A=0,Set_Voltage_W=0,Set_Voltage = 0;
float INA226_A = 0,INA226_V = 0,INA226_W = 0;
float expect_A=2.4f,expert_W=50.f;
bool IS_PID_ENABLE=true;
uint8_t usart_TX[4]={0x44,0,0,0x13};
bool IF_CAP_INIT = false;

int main(void)
 {
	delay_init();//延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	INA_Init();//功率计初始化
 	TIM4_PWM_Init(899,0);//不分频。PWM频率=72000000/900=808Khz
	PID_Init();//PID初始化
	uart_init(115200);//串口初始化
	LED_Init();//
	
	while(1)
	{
		//读取功率信息
		INA226_A=(float)INA_GET_Current_MA_0()*IAN_I_LSB/1000.f;//电流
		INA226_V=(float)INA_GET_Voltage_MV_0()*INA_V_LSB/1000.f;//电压
		INA226_W=(float)INA_GET_Power_MW_0()*INA_Power_LSB/1000.f;//功率
		
		//等待电容充电至14V以上开启输出使能，防止电调掉线或误判超功率扣血
		if(IF_CAP_INIT == false)
		{
			if(INA226_V < 14.f)
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);//关闭使能
			}else
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_5);//开启使能
				IF_CAP_INIT = true;//电容初始化完毕
			}
		}
		//发送装载
		usart_TX[1] = (uint8_t)(INA_GET_Voltage_MV_0() >> 8);
		usart_TX[2] = (uint8_t)(INA_GET_Voltage_MV_0()     );
		
		for(int i = 0; i < 4; i++)
		{
			//串口发送
			USART_SendData(USART1,usart_TX[i]);
			//等待发送完毕
			while(USART_GetFlagStatus(USART1,USART_FLAG_TC) != SET);
		}
		
		if(IS_PID_ENABLE)
		{
			Set_Voltage_A=	Pid(expect_A-INA226_A,&Current_PID);
			Set_Voltage_W=	Pid(expert_W-INA226_W,&Power_PID);
			Set_Voltage = INA226_V >= 14.f ? Set_Voltage_W : Set_Voltage_A;
		}
		
		TIM_SetCompare3(TIM4,Set_Voltage);		   
	}
}

