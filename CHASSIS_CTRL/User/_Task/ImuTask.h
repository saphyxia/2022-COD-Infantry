#ifndef IMUTAKS_H
#define IMUTAKS_H
/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "FreeRTOS_Init.h"
/* Exported typedef -----------------------------------------------------------*/
//设备工作状态
typedef enum{
    BLOCKED,//阻塞
    READY,//就绪
    RUNNING,//运行
    STATE_NUM,
}DEVICE_STATE;

/* Exported variables --------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/
extern void ImuTask(void *pvParameters) ;
#endif

