#ifndef _IO_INIT_H
#define _IO_INIT_H
#include "stm32f4xx.h"
#include "delay.h"
#include "stdbool.h"


#define POWER1_CTRL_SWITCH 0
#define POWER2_CTRL_SWITCH 1
#define POWER3_CTRL_SWITCH 2
#define POWER4_CTRL_SWITCH 3

void gpio_Init(void);
void TIM2_PWM_Init(uint16_t arr,uint16_t psc);
void power_ctrl_configuration(void);
void power_ctrl_on(uint8_t num);
void power_ctrl_off(uint8_t num);
void power_ctrl_toggle(uint8_t num);
#endif

