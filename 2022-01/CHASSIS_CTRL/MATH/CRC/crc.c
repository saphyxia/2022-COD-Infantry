#include "crc.h"

//USART8
_POWER power;
float Super_C_Power;
int16_t Receive_Uart8_Buff[10];
int8_t Send8_counter,flag_uart8_conversion_rw;


/* Modbus-RTU读取报文 */
uint8_t r_DPM8600[8] = {0x01,           //地址码
                        0x03,           //功能码，读寄存器数据
                        0x10, 0x01,     //数据地址
                        0x00, 0x02,     //读取数据个数
                        0x00, 0x00};    //CRC码

/* Modbus-RTU写入报文 */
uint8_t w_DPM8600[13] = {0x01,          //地址码
                         0x10,          //功能码，写多个寄存器数据
                         0x00, 0x00,    //寄存器起止地址
                         0x00, 0x02,    //寄存器数目
                         0x04,          //写入的字节数
                         0x00, 0x00,    //设定的电压值 [7-8]
                         0x00, 0x00,    //设定的电流值 [9-10]
                         0x00, 0x00};   //CRC码

/* CRC 高位字节值表 */
static uint8_t auchCRCHi[] =
{
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40
};
/* CRC 低位字节值表 */
static uint8_t auchCRCLo[] =
{
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
	0x40
};

/**
  * @name   N_CRC16
  * @brief  CRC-16校验码运算
  * @param  *updata:需发送的通讯信息数组    len:通讯信息的帧数
  * @retval none
  * @attention none
*/
uint16_t N_CRC16(uint8_t *updata, int len)
{
    uint8_t uchCRCHi = 0xff;//高8位CRC字节初始化
    uint8_t uchCRCLo = 0xff;//低8位CRC字节初始化
    uint16_t uindex;//CRC循环中的索引

    while (len--)//等待通讯数组的CRC-16校验码计算完毕
    {
        uindex = uchCRCHi ^ *updata++;//索引为通讯信息帧的第n(n为从0至len)个字节与CRC寄存器的高8位进行异或运算
        uchCRCHi = uchCRCLo ^ auchCRCHi[uindex];//高八位为低八位与高位字节索引值异或运算
        uchCRCLo = auchCRCLo[uindex];//低八位为高八位字节索引值
    }
    return (uchCRCHi << 8 | uchCRCLo);//合并高低8位，反馈完整的16位CRC校验码
}

/**
  * @name   ProduceCRC
  * @brief  生成对应CRC-16校验码
  * @param  *srcArray:需发送的modbus代码数组  b:srcArray数组的大小
  * @retval none
  * @attention none
*/
void ProduceCRC(uint8_t *srcArray, int b)
{
    int a = (b - 2);    
    unsigned short kk = N_CRC16(srcArray, a);//去除最后两位进行CRC-16校验码运算

    srcArray[a] = kk / 256;
    srcArray[a + 1] = kk % 256;//最后两位赋值为算得的CRC-16校验码
}
/**
  * @name   UART8_SendByte_r
  * @brief  主机发送读取电流电压值函数
  * @param  none
  * @retval none
  * @attention none
*/
void UART8_SendByte_r(void)
{
	ProduceCRC(r_DPM8600, 8);
	USART_SendData(UART8,r_DPM8600[Send8_counter++] );
	if(Send8_counter ==8)
		{   
			USART_ITConfig(UART8, USART_IT_TXE, DISABLE); 
			Send8_counter=0;
		}
}
/**
  * @name   UART8_SendByte_w
  * @brief  主机发送设定电流电压值函数
  * @param  set_U:设定的电压值  set_I:设定的电流值
  * @retval none
  * @attention none
*/
void UART8_SendByte_w(float set_U, float set_I)
{
	uint16_t U, I;

    U = (uint16_t)(set_U * 100);
    I = (uint16_t)(set_I * 1000);
    w_DPM8600[7] = (U >> 8) & 0xFF;
    w_DPM8600[8] = U & 0xFF;
    w_DPM8600[9] = (I >> 8) & 0xFF;
    w_DPM8600[10] = I & 0xFF;
    ProduceCRC(w_DPM8600, 13); //生成CRC检验码
    USART_SendData(UART8,w_DPM8600[Send8_counter++] );
    if(Send8_counter ==13)
    {   
        USART_ITConfig(UART8, USART_IT_TXE, DISABLE); 
        Send8_counter=0;	
    }
}
