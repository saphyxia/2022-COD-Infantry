#ifndef STATE_TASK_H
#define STATE_TASK_H
#include "can1.h"


typedef enum
{
		mode_err,
    INVA,//卸力         invalidated
    FOLO,//跟随云台     follow
    SPIN,//小陀螺       spin
    CAPA,//电容         capacitance

     //重置
}CHASSIS_MODE;


typedef struct
{
    uint8_t ctrl_Flag,mode_Flag;
    int16_t vx,vy,key;
}CHASSIS_Info;

extern CHASSIS_Info chas_Ctrl;

extern uint8_t RC_Info_Buf[8];

void RC_Info_SendtoChassis(void);





#endif



