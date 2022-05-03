#include "main.h"


void I2C3_GPIO_INIT(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 
	I2C_InitTypeDef   I2C_InitStructure; 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C3,ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_I2C3);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_I2C3);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  /* I2C ���� */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		 /* �ߵ�ƽ�����ȶ����͵�ƽ���ݱ仯 SCL ʱ���ߵ�ռ�ձ� */
  I2C_InitStructure.I2C_OwnAddress1 =0x1C; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C��Ѱַģʽ */
  I2C_InitStructure.I2C_ClockSpeed = 300000;	                            /* ͨ������ */
  I2C_Init(I2C3, &I2C_InitStructure);	                                      /* I2C1 ��ʼ�� */
  I2C_Cmd(I2C3, ENABLE);  	                                                /* ʹ�� I2C1 */

  I2C_AcknowledgeConfig(I2C3, ENABLE);  
	
}

/**
��MPU6050��ȡ���ݿ顣  
IST8310Ĭ�ϵ�ַ 
pBuffer:ָ����մ�IST8310��ȡ���ݵĻ�������ָ�롣  
readAddr: IST8310Ҫ��ȡ���ڲ���ַ��  
NumByteToRead:��IST8310��ȡ���ֽ�����  
*/

void IST8310_I2C_Read(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t readAddr, uint16_t NumByteToRead)
{
  /* ����Ƿ���� */
  while(I2C_GetFlagStatus( I2C3, I2C_FLAG_BUSY));
  /* ʹ��IIC3 */
  I2C_GenerateSTART(I2C3, ENABLE);

  /* IIC���	*/
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));
  /* ���ʹӻ��豸��ַ */
  I2C_Send7bitAddress(I2C3, slaveAddr, I2C_Direction_Transmitter); 

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /* ͨ����������PEλ�����*/
  I2C_Cmd(I2C3, ENABLE);

  /* ����Ҫд���豸���ڲ���ַ  */
  I2C_SendData(I2C3, readAddr);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* �ڶ��η��Ϳ�ʼ״̬ */
  I2C_GenerateSTART(I2C3, ENABLE);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C3, slaveAddr, I2C_Direction_Receiver);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  /* ��������Ҫ��ȡʱ */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* ��ȡ������  ֹͣʹ�� */
      I2C_AcknowledgeConfig(I2C3, DISABLE);

      /* ����ֹͣ���� */
      I2C_GenerateSTOP(I2C3, ENABLE);
    }

    if(I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* �Ӵӻ��豸��ȡһ���ֽ� */
      *pBuffer = I2C_ReceiveData(I2C3);

      /* ָ����һ�������Ѷ��ֽڵ�λ��  */
      pBuffer++;

      /* ���ٶ��ֽڼ����� */
      NumByteToRead--;
    }
  }

  /* ʹȷ��Ϊ��һ�ν�������׼��   */
  I2C_AcknowledgeConfig(I2C3, ENABLE);
}

/*
��ӻ��豸д��һ���ֽڡ�  
IST8310Ĭ�ϵ�ַ
ָ�򻺳�����ָ�룬�û���������Ҫд��IST8310�����ݡ�  
Ҫд�����ݵļĴ�����ַ  
*/
void IST8310_I2C_ByteWrite(uint8_t slaveAddr, uint8_t pBuffer, uint8_t writeAddr)
{
  I2C_GenerateSTART(I2C3, ENABLE);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C3, slaveAddr, I2C_Direction_Transmitter);
 
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

  I2C_SendData(I2C3, writeAddr);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_SendData(I2C3, pBuffer);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  I2C_GenerateSTOP(I2C3, ENABLE);


}



