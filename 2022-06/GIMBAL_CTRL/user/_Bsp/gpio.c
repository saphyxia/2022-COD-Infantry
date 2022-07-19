#include "main.h"
#include "gpio.h"


void gpio_Init(void)
{    	 
    GPIO_InitTypeDef   GPIO_InitStructure;
//    NVIC_InitTypeDef   NVIC_InitStructure;
//    EXTI_InitTypeDef   EXTI_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    //CS_ACCEL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOH, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;    //CS_ACCEL
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;   //CS_GYRO
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   //RESET IST8310
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
//    GPIO_Init(GPIOG, &GPIO_InitStructure);//初始化

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;   //INT1 INT2
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8);
	
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;   // INT  ist8310
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GPIOG, &GPIO_InitStructure);
	
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource3);  //连接PG3到中断线6
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource4);  //连接PC4到中断线4
//    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);  //连接PC5到中断线5

//    /* 配置EXTI_Line3、4、5 */
//    EXTI_InitStructure.EXTI_Line = EXTI_Line3 | EXTI_Line4 | EXTI_Line5;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //下降沿触发
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
//    EXTI_Init(&EXTI_InitStructure);//配置

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);	

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;  
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
}

void EXTI3_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位  
}


void EXTI4_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}


void EXTI9_5_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line5);//清除LINE5上的中断标志位  
}

/**
 *@Function:		Beep_Init()
 *@Description:	蜂鸣器初始化
 *@Param:       形参
 *@Return:	  	返回值
 */
void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_TIM4);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period=600-1;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_CtrlPWMOutputs(TIM4,ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);
}
/**
 *@Function:		Beep_Ctrl(uint16_t tune, uint16_t sound)	
 *@Description:	蜂鸣器控制
 *@Param:       形参
 *@Return:	  	返回值
 */
void Beep_Ctrl(uint16_t tune, uint16_t sound)	
{
  TIM4->ARR = tune;
  TIM4->CCR3 = sound;
}




