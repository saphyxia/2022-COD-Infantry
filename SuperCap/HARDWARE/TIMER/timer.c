#include "timer.h"
#include "led.h"
#include "usart.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/4
//版本：V1.1
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//********************************************************************************
//V1.1 20120904
//1,增加TIM3_PWM_Init函数。
//2,增加LED0_PWM_VAL宏定义，控制TIM3_CH2脉宽									  
//////////////////////////////////////////////////////////////////////////////////  
   	  
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
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


