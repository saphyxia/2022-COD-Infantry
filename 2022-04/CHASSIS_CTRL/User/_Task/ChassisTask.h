#ifndef CHASSISTASK_H
#define CHASSISTASK_H
/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "rc.h" 
#include "ImuTask.h"
#include "FreeRTOS_Init.h"
#include "motor.h"

/* Exported macro -------------------------------------------------------------*/
//自然数e
#define NATURAL_NUMBER  2.718281828f
//交互丢失时间,单位ms
#define INTERAC_LOST_TIME 150

/* Exported typedef -----------------------------------------------------------*/
//底盘状态枚举
typedef enum
{
	ERR,
	INVA,
	FOLO,
	SPIN,
	STATE_CNT,
}CHAS_MODE;
//底盘信息封装
typedef struct
{
	//底盘状态
	DEVICE_STATE state;
    CHAS_MODE mode;
	//交互方式：CAN
	CAN_TypeDef *CANx;
	uint32_t stdid;
	//期望解算
	float target_FR,target_LR,target_GYRO;
	float midangle;
	//控制信息
	int16_t ch2,ch3;
	RC_KEY_T key;
	//底盘电机封装
	DJI_MOTOR *LF,*LR,*RF,*RR,*FOLO;
}CHASSIS_INFO_T;

/* Exported variables --------------------------------------------------------*/
extern CHASSIS_INFO_T chassis;

/* Exported functions --------------------------------------------------------*/
extern void ChassisTask(void *pvParameters);
extern void get_Chas_INFO(CAN_TypeDef *CANx,CanRxMsg *RxMsg,CHASSIS_INFO_T *std);
extern float RAMP_float( float final, float now, float ramp );
#endif


