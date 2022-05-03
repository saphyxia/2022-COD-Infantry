#include "main.h"




void SPI1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef SPI_InitStructure;

    //1.初始化GPIO   
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB,ENABLE);  
    /* 使能 SPI 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);    		
	
    /* GPIO初始化 */
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    /* 配置SCK引脚为复用功能  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3  ;  
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* 配置MISO引脚为复用功能 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* 配置MOSI引脚为复用功能 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* 连接 引脚源*/
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_SPI1);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);

		//这里只针对SPI口初始化
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位SPI1
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位SPI1

    //2.配置SPI工作模式
    // 分频
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; 
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; // 时钟空闲idle时是低电平
    SPI_InitStructure.SPI_CRCPolynomial = 10; // 不需要使用CRC校验
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 数据帧长度为8位
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// 双线全双工模式
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //数据传输从 MSB 位开始
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS 信号由软件管理
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;	//设置为主设备

    SPI_Init(SPI1,&SPI_InitStructure); 
    SPI_Cmd(SPI1, ENABLE);   

}


//SPI1速度设置函数
//SPI速度=fAPB2/分频系数
//SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
//fAPB2时钟一般为84Mhz
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性

	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI5速度 
	SPI_Cmd(SPI1,ENABLE); //使能SPI1
} 
//SPI1 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
uint8_t SPI1_ReadWriteByte(uint8_t TxData)
{		
	u8 txflag=1;
	u8 count=0;
	while(1)
	{
		if(txflag==1&&SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE))
		{	
			for(count=0;count<200;count++){}
			SPI_I2S_SendData(SPI1, TxData); //发送数据
			txflag=0;
		}
		if(txflag==0&&SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE)) //等待SPI接收标志位空
		{
			for(count=0;count<200;count++){}
			return SPI_I2S_ReceiveData(SPI1); //接收数据	
		}		
	}
}



