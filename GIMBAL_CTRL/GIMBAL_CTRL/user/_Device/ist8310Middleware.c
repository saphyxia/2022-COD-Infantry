#include "ist8310Middleware.h"

/*IST8310��I2C��ַ,������Ҫע����ǣ���Ӳ��IIC��Э���£�ֻ����7λ����10λ�ĵ�ַ����������ַд��IICʱ����Ҫ����һλ����������λ��������ʹ�õ�������һλ��������������΢СһЩ
��IIC��ʼ��ʱ�����ԭ��ͼsda��scl����һ����������4.7k�ģ���˿���ֱ��ʹ��Ӳ��IIC�䵽400K��������4.7K����������Ӧ�ÿ�����1M�����ǿ��Գ���һ��  
��ʼ��ʱ��Ҫ�������ƺõ�����ֱ��д���ȥ��Ҳ����д10λ��ַ�����ǲ������޸ģ���Ϊһ���޸ģ���ist8310��д�Ͷ��Ĵ����о͵��޸�
����ڵ��Թ����п�������̨���񲻹��������ܾ���ist8310��iic�����⣬���Ҷ�Ӧ������Ϳ����ˣ�������������iic���ĵط�*/
#define IST8310_IIC_ADDRESS 0x0E  

//��ist8310�����ֽ�
uint8_t ist8310_IIC_read_single_reg(uint8_t reg)
{
	u8 res;
    IIC_Start(); 
	IIC_Send_Byte(0x1C);
	IIC_Wait_Ack();		
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte(0x1D);
    IIC_Wait_Ack();		
	res=IIC_Read_Byte(0);
    IIC_Stop();
	return res;	     
}

//��ist8310����ֽ�
void ist8310_IIC_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
   	IIC_Start(); 
	IIC_Send_Byte(0x1C);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 	
	}
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();		
    IIC_Start();
	IIC_Send_Byte(0x1D);
    IIC_Wait_Ack();	
	while(len)
	{
		if(len==1)*buf=IIC_Read_Byte(0);
		else *buf=IIC_Read_Byte(1);	
		len--;
		buf++; 
	}    
    IIC_Stop();	
}

//дist8310�����ֽ�
void ist8310_IIC_write_single_reg(uint8_t reg, uint8_t data)
{
  IIC_Start(); 
	IIC_Send_Byte(0x1C);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();		 	
	}
    IIC_Send_Byte(reg);	
    IIC_Wait_Ack();		
	IIC_Send_Byte(data);
	if(IIC_Wait_Ack())	
	{
		IIC_Stop();	 	 
	}		 
    IIC_Stop();	 

}

//дist8310����ֽ�
void ist8310_IIC_write_muli_reg(uint8_t reg, uint8_t *data, uint8_t len)
{
    //ist8310����д����ֽڣ���ʱû�б�дʹ��
}

void ist8310_RST_H(void)
{
     GPIO_SetBits(GPIOG,GPIO_Pin_6);
}

void ist8310_RST_L(void)
{
     GPIO_ResetBits(GPIOG,GPIO_Pin_6);
}

void ist8310_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        ist8310_delay_us(1000);
    }
}


void ist8310_delay_us(uint16_t us)
{
    uint32_t ticks = 0;
    uint32_t told = 0, tnow = 0, tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = us * 72;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}
