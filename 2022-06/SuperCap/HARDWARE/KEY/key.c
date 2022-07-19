#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

PID_Struct Current_PID,Power_PID;
			
void PID_Init(void)
{
	Current_PID.Kp=0.2f;
	Current_PID.Ki=0.6f;
	Current_PID.result_min=460;
	Current_PID.result_max=800;
	
	Power_PID.Kp=0.2f;
	Power_PID.Ki=0.6f;
	Power_PID.result_min=0;
	Power_PID.result_max=800;
}

//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4;//KEY0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4

	
	//��ʼ�� WK_UP-->GPIOA.0	  ��������
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.0

}
//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1��KEY0����
//2��KEY1����
//3��KEY2���� 
//4��KEY3���� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==1||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==1)return KEY0_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==0&&WK_UP==0)key_up=1; 	    
 	return 0;// �ް�������
}

float Pid(float err,PID_Struct *p)
{
		p->err=err;
		
		p->result+=p->Kp*(p->err-p->err_last) \
							+p->Ki*p->err	\
							+p->Kd*(p->err-2*p->err_last+p->err_last_last);
		
		VAL_LIMIT(p->result,p->result_min,p->result_max);

		p->err_last_last=p->err_last;
		p->err_last=p->err;
	
		return p->result;
}


