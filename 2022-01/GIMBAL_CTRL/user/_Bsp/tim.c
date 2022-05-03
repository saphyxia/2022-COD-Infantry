#include "tim.h"
#include "main.h"


void TIM10_init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    GPIO_PinAFConfig(GPIOF, GPIO_PinSource6, GPIO_AF_TIM10);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, DISABLE);

    TIM_TimeBaseInitStructure.TIM_Period = 4999;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);

    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseInitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC1Init(TIM10, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM10, ENABLE);

    TIM_Cmd(TIM10, ENABLE);
}



