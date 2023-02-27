#ifndef MAIN_H
#define MAIN_H
#include "stm32f4xx.h"
#include "sys.h"
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h> 
#include "math.h"

#include "gpio.h"
#include "tim.h"
#include "flash.h"

#include "spi.h"
#include "myiic.h"

#include "AHRS_MiddleWare.h"
#include "AHRS.h"

#include "FreeRTOS_Init.h"

#define ABS(x) ((x)>0? (x):(-(x)))       //绝对值宏定义






#endif


