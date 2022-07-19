#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdio.h>
#include "stdbool.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx.h"
#include "stdint.h"
#include "sys.h" 
#include "string.h"
#include "delay.h"

/**
 * @brief 
 * @param 
 */
extern volatile uint32_t sysTickUptime;

#define abs(x) ((x)>0? (x):(-(x)))       //绝对值宏定义

/**
 * @@@@辅助函数
 */
float constrain(float amt,float high,float low);
float anti_constrain(float amt,float high,float low);
float Slope(float M ,float *queue ,uint16_t len);


uint32_t micros(void);

uint32_t millis(void);
void systemInit(void);
void Time_Init(void);

void Single_time_cal(void);
bool Judge_IF_NAN(float x );
#endif


