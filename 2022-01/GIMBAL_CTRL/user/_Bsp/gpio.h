#ifndef __LED_H__
#define __LED_H__
#include "sys.h"


#define REG_led    PHout(11) //SCL





void gpio_Init(void);
void TIM1_PWM_Init(void);

#endif


