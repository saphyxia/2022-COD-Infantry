#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//��ʱ�� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/4
//�汾��V1.1
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,����TIM3_PWM_Init������
//2,����LED0_PWM_VAL�궨�壬����TIM3_CH2����									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
void TIM2_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOA,&GPIO_InitStructure);             
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); 

	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);  
 
  TIM_ARRPreloadConfig(TIM2,ENABLE);
	
	TIM_Cmd(TIM2, ENABLE);  


}

void TIM4_PWM_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 	

	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;           
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOB,&GPIO_InitStructure);             
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
		 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  
	TIM_OC4Init(TIM4, &TIM_OCInitStructure); 

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);  


}


