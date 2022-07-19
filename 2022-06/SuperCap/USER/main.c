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
	delay_init();//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	INA_Init();//���ʼƳ�ʼ��
 	TIM4_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000000/900=808Khz
	PID_Init();//PID��ʼ��
	uart_init(115200);//���ڳ�ʼ��
	LED_Init();//
	
	while(1)
	{
		//��ȡ������Ϣ
		INA226_A=(float)INA_GET_Current_MA_0()*IAN_I_LSB/1000.f;//����
		INA226_V=(float)INA_GET_Voltage_MV_0()*INA_V_LSB/1000.f;//��ѹ
		INA226_W=(float)INA_GET_Power_MW_0()*INA_Power_LSB/1000.f;//����
		
		//�ȴ����ݳ����14V���Ͽ������ʹ�ܣ���ֹ������߻����г����ʿ�Ѫ
		if(IF_CAP_INIT == false)
		{
			if(INA226_V < 14.f)
			{
				GPIO_ResetBits(GPIOB,GPIO_Pin_5);//�ر�ʹ��
			}else
			{
				GPIO_SetBits(GPIOB,GPIO_Pin_5);//����ʹ��
				IF_CAP_INIT = true;//���ݳ�ʼ�����
			}
		}
		//����װ��
		usart_TX[1] = (uint8_t)(INA_GET_Voltage_MV_0() >> 8);
		usart_TX[2] = (uint8_t)(INA_GET_Voltage_MV_0()     );
		
		for(int i = 0; i < 4; i++)
		{
			//���ڷ���
			USART_SendData(USART1,usart_TX[i]);
			//�ȴ��������
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

