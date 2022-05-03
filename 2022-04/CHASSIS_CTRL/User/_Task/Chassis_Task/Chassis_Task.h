#ifndef CHASSISTASK_H
#define CHASSISTASK_H
#include "main.h"
#include "FreeRTOS_Init.h"
#include "motor.h"
#include "stm32f4xx.h"
#include "can.h"
#include "chassis.h"

#define IF_DR16_CONNECT 0	

#define IS_CHASSISTASK_ENABLE 1   //使能底盘任务

void CHASSIS_Test(void);
void CHASSIS_TASK(void *pvParameters);
void CHASSIS_Ctrl(void);
void CHASSIS_Reset(CHASSIS_INFO_T *CHAS);
void CHASSIS_RcCtrl(void);
void CHASSIS_KeyCtrl(void);
void get_Chassis_Info(CAN_TypeDef *CANx,CanRxMsg *RxMsg);

#endif

