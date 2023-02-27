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
	
  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;		 /* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_OwnAddress1 =0x1C; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;	
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_ClockSpeed = 300000;	                            /* 通信速率 */
  I2C_Init(I2C3, &I2C_InitStructure);	                                      /* I2C1 初始化 */
  I2C_Cmd(I2C3, ENABLE);  	                                                /* 使能 I2C1 */

  I2C_AcknowledgeConfig(I2C3, ENABLE);  
	
}

/**
从MPU6050读取数据块。  
IST8310默认地址 
pBuffer:指向接收从IST8310读取数据的缓冲区的指针。  
readAddr: IST8310要读取的内部地址。  
NumByteToRead:从IST8310读取的字节数。  
*/

void IST8310_I2C_Read(uint8_t slaveAddr, uint8_t* pBuffer, uint8_t readAddr, uint16_t NumByteToRead)
{
  /* 检测是否空闲 */
  while(I2C_GetFlagStatus( I2C3, I2C_FLAG_BUSY));
  /* 使能IIC3 */
  I2C_GenerateSTART(I2C3, ENABLE);

  /* IIC检查	*/
  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));
  /* 发送从机设备地址 */
  I2C_Send7bitAddress(I2C3, slaveAddr, I2C_Direction_Transmitter); 

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  /* 通过重新设置PE位来清除*/
  I2C_Cmd(I2C3, ENABLE);

  /* 发送要写入设备的内部地址  */
  I2C_SendData(I2C3, readAddr);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

  /* 第二次发送开始状态 */
  I2C_GenerateSTART(I2C3, ENABLE);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_MODE_SELECT));

  I2C_Send7bitAddress(I2C3, slaveAddr, I2C_Direction_Receiver);

  while(!I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  /* 当有数据要读取时 */
  while(NumByteToRead)
  {
    if(NumByteToRead == 1)
    {
      /* 读取到数据  停止使能 */
      I2C_AcknowledgeConfig(I2C3, DISABLE);

      /* 发送停止条件 */
      I2C_GenerateSTOP(I2C3, ENABLE);
    }

    if(I2C_CheckEvent(I2C3, I2C_EVENT_MASTER_BYTE_RECEIVED))
    {
      /* 从从机设备读取一个字节 */
      *pBuffer = I2C_ReceiveData(I2C3);

      /* 指向下一个保存已读字节的位置  */
      pBuffer++;

      /* 减少读字节计数器 */
      NumByteToRead--;
    }
  }

  /* 使确认为另一次接收做好准备   */
  I2C_AcknowledgeConfig(I2C3, ENABLE);
}

/*
向从机设备写入一个字节。  
IST8310默认地址
指向缓冲区的指针，该缓冲区包含要写入IST8310的数据。  
要写入数据的寄存器地址  
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



