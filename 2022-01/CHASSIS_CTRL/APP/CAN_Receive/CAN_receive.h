#ifndef CANTASK_H
#define CANTASK_H
#include "main.h"
#include "motor.h"
#include "chassis_task.h"


typedef enum 
{
    INVA=0,//卸力
    FOLO,//跟随云台
    SPIN,//小陀螺
    _RESET//重置
}CHASSIS_MODE;

typedef enum
{
	X=0,
	Y,
	W
}CHASSIS_CONTROL;

typedef struct
{
	  uint8_t ctrl_Flag; 
    uint8_t mode_Flag;
    int16_t v[3];
}CHASSIS_Info;

extern float rx_power[4],rx_p;
extern CHASSIS_Info Chassis_Ctrl;
extern struct Wulie_A110_6 A110_6;


extern void CAN_Send_Msg(CAN_TypeDef* CAN,int32_t CAN_ID,int16_t *P);
extern void CAN_Send_SetPower(CAN_TypeDef* CAN,int32_t CAN_StdID,int16_t tx_power);
extern void CAN2_Send_Msg(void);
#endif
