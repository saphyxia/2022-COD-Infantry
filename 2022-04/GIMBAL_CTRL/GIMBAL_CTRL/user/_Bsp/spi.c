#include "main.h"




void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    //1.��ʼ��GPIO   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);  
    /* ʹ�� SPI ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);    		
	
    /* GPIO��ʼ�� */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    /* ����SCK����Ϊ���ù���  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3  ;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* ����MISO����Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* ����MOSI����Ϊ���ù��� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* ���� ����Դ*/
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);

		//����ֻ���SPI�ڳ�ʼ��
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//��λSPI1
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//ֹͣ��λSPI1

    //2.����SPI����ģʽ
    // ��Ƶ
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // ʱ�ӿ���idleʱ�ǵ͵�ƽ
    SPI_InitStructure.SPI_CRCPolynomial = 10; // ����Ҫʹ��CRCУ��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // ����֡����Ϊ8λ
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// ˫��ȫ˫��ģʽ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //���ݴ���� MSB λ��ʼ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS �ź����������
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//����Ϊ���豸

    SPI_Init(SPI1,&SPI_InitStructure); 
    SPI_Cmd(SPI1, ENABLE);   

}


//SPI1�ٶ����ú���
//SPI�ٶ�=fAPB2/��Ƶϵ��
//SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2ʱ��һ��Ϊ84Mhz
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��

	SPI1->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI1->CR1|=SPI_BaudRatePrescaler;	//����SPI5�ٶ� 
	SPI_Cmd(SPI1,ENABLE); //ʹ��SPI1
} 
//SPI1 ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	u8 txflag=1;
	u8 count=0;
	while(1)
	{
		if(txflag==1&&SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		{	
			for(count=0;count<200;count++){}
			SPI_I2S_SendData(SPI1, TxData); //��������
			txflag=0;
		}
		if(txflag==0&&SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) //�ȴ�SPI���ձ�־λ��
		{
			for(count=0;count<200;count++){}
			return SPI_I2S_ReceiveData(SPI1); //��������	
		}		
	}
}



