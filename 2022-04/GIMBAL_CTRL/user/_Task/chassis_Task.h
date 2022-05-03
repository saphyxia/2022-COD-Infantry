#ifndef STATE_TASK_H
#define STATE_TASK_H
#include "FreeRTOS_Init.h"
#include "systemState_Task.h"
#include "can.h"
#include "rc.h"


typedef enum
{
		act_err,
    INVA,
    FOLO,//跟随云台     follow
    SPIN,//小陀螺       spin
}CHASSIS_MODE;

typedef struct
{
    uint8_t state;

    CHASSIS_MODE mode;
		
		uint8_t ctrl;

    CanTxMsg *txMsg;
    
    int16_t vx,vy,key;

    RC_ctrl_t *dr16;
    
    void (*state_Setup)(DEVICE_STATE);
	void (*mode_Setup)(CHASSIS_MODE);
}CHASSIS;

extern CHASSIS chassis;

static void chassis_Ctrl(void);
static void chassis_State_Handoff(DEVICE_STATE mode);
static void chassis_Mode_Handoff(CHASSIS_MODE mode);



#endif



