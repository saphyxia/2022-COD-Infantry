#ifndef __LED_H__
#define __LED_H__
#include "sys.h"


#define REG_led    PHout(11) //SCL





void gpio_Init(void);
void Beep_Init(void);
void Beep_Ctrl(uint16_t tune, uint16_t sound);

#endif


