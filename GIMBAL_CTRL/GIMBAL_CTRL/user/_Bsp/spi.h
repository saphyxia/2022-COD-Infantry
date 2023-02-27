#ifndef SPI_H
#define SPI_H
#include "sys.h"





void SPI1_Init(void);
void SPI1_SetSpeed(uint8_t SPI_BaudRatePrescaler);
uint8_t SPI1_ReadWriteByte(uint8_t TxData);


#endif

