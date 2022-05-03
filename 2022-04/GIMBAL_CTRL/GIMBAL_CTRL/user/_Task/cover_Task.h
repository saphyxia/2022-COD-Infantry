#ifndef COVER_TASK_H
#define COVER_TASK_H

#include "FreeRTOS_Init.h"
#include "systemState_Task.h"


typedef struct
{
    DEVICE_STATE state;

    void (*state_Setup)(DEVICE_STATE);
}COVER;
extern COVER cover;

static void cover_Ctrl(void);
static void cover_State_Handoff(DEVICE_STATE state);


#endif



