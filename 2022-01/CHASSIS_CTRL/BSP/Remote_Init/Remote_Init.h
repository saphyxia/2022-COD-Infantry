#ifndef Remote_Init_H
#define Remote_Init_H
#include "stm32f4xx.h"

extern void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void RC_unable(void);
extern void RC_restart(uint16_t dma_buf_num);


#endif
