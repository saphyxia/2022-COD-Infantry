#include "main.h"

#define MAG_SEN 0.3f        //   ԭʼ�������ݱ�� ��λut

#define IST8310_WHO_AM_I 0x00       //ist8310 "who am I " 
#define IST8310_WHO_AM_I_VALUE 0x10 //IST8310�Ĵ���ID��ֵַ

#define IST8310_WRITE_REG_NUM 4    //д��Ĵ���������仯��


//��һ��:IST8310�ļĴ���
//�ڶ���:��Ҫд��ļĴ���ֵ
//������:���صĴ�����
static const uint8_t ist8310_write_reg_data_error[IST8310_WRITE_REG_NUM][3] ={
        {0x0B, 0x08, 0x01},     //�����жϣ��������õ͵�ƽ
        {0x41, 0x09, 0x02},     //ƽ����������
        {0x42, 0xC0, 0x03},     //������0xC0
        {0x0A, 0x0B, 0x04}};    //200Hz���Ƶ��
uint8_t res = 0;
uint8_t ist8310_init(void)
{
    static const uint8_t wait_time = 150;
    static const uint8_t sleepTime = 50;
//    uint8_t res = 0;
    uint8_t writeNum = 0;
    ist8310_RST_L();
    ist8310_delay_ms(sleepTime);
    ist8310_RST_H();
    ist8310_delay_ms(sleepTime);
    res = ist8310_IIC_read_single_reg(IST8310_WHO_AM_I);  //��ȡIIC��ַ
    if (res != IST8310_WHO_AM_I_VALUE)   //�жϵ�ַ�Ƿ�һ��
    {
        return IST8310_NO_SENSOR;   //��һ�·��ش���״̬
    }

    //set mpu6500 sonsor config and check
    for (writeNum = 0; writeNum < IST8310_WRITE_REG_NUM; writeNum++)
    {
			/*
			[0][0] ���Ƶ��������Ĵ���
			[0][1] ����DRDY����ʹ��
			[1][0] �������ݵ�ƽ������ʱ��
			[1][1] ƽ����������
			[2][0] ����ʱ������Ĵ���
			[2][1] �������壬�����޸�
			[3][0] �������üĴ���
			[3][1] 200Hz
			*/
        ist8310_IIC_write_single_reg(ist8310_write_reg_data_error[writeNum][0], ist8310_write_reg_data_error[writeNum][1]);
        ist8310_delay_us(wait_time);
        res = ist8310_IIC_read_single_reg(ist8310_write_reg_data_error[writeNum][0]);
        ist8310_delay_us(wait_time);
        if (res != ist8310_write_reg_data_error[writeNum][1])
        {
            return ist8310_write_reg_data_error[writeNum][2];
        }
    }
    return IST8310_NO_ERROR;
}

//���ʹ��DMA�����жϣ����ڳ�ʼ����ȡ�Ժ�ʹ�������������ȡ
void ist8310_read_over(uint8_t *status_buf, ist8310_real_data_t *ist8310_real_data)
{

    if (status_buf[0] & 0x01)
    {
        int16_t temp_ist8310_data = 0;
        ist8310_real_data->status |= 1 << IST8310_DATA_READY_BIT;

        temp_ist8310_data = (int16_t)((status_buf[2] << 8) | status_buf[1]);  //����ļ��㶼�ǲ���2�Ĳ��뷽�����н�����ʹ����BMI088���㷽��һ��
        ist8310_real_data->mag[0] = MAG_SEN * temp_ist8310_data;   //����ת���ɴų���λ��Ҫ�����ֲ����ᵽ��+-0.3ut
        temp_ist8310_data = (int16_t)((status_buf[4] << 8) | status_buf[3]);
        ist8310_real_data->mag[1] = MAG_SEN * temp_ist8310_data;
        temp_ist8310_data = (int16_t)((status_buf[6] << 8) | status_buf[5]);
        ist8310_real_data->mag[2] = MAG_SEN * temp_ist8310_data;
    }
    else
    {
        ist8310_real_data->status &= ~(1 << IST8310_DATA_READY_BIT);
    }
}

//������ȡ������DMA
void ist8310_read_mag(fp32 mag[3])
{
    uint8_t buf[6];
    int16_t temp_ist8310_data = 0;
    //read the "DATAXL" register (0x03)
    ist8310_IIC_read_muli_reg(0x03, buf, 6);

    temp_ist8310_data = (int16_t)((buf[1] << 8) | buf[0]);
    mag[0] = MAG_SEN * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[3] << 8) | buf[2]);
    mag[1] = MAG_SEN * temp_ist8310_data;
    temp_ist8310_data = (int16_t)((buf[5] << 8) | buf[4]);
    mag[2] = MAG_SEN * temp_ist8310_data;
}



