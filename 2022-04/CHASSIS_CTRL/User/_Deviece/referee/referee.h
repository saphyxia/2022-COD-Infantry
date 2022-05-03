#ifndef __REFEREE_H
#define __REFEREE_H

#include "main.h"

#define REFEREE_DATALENGTH 200


void Referee_Init(void);
void Referee_Data_Send(uint16_t cmd_id, uint8_t* p_data);

#endif


