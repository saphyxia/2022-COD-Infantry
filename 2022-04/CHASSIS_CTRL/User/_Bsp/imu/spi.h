#ifndef __SPI_H
#define __SPI_H
#include "main.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//SPI ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
 	    													  
void SPI5_Init(void);			 //��ʼ��SPI5��
void SPI5_SetSpeed(u8 SpeedSet); //����SPI5�ٶ�   
u8 SPI5_ReadWriteByte(u8 TxData);
		 
#endif

