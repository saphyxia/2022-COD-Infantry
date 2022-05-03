#include "main.h"


fp32 BMI088_ACCEL_SEN = BMI088_ACCEL_3G_SEN;  //3������  ACCEL
fp32 BMI088_GYRO_SEN = BMI088_GYRO_2000_SEN;  //2000�ֽڳ���  GYRO
mpu_data_t mpu_data;
uint16_t gyro_offest;
uint16_t accel_offest[3];

#if defined(BMI088_USE_SPI)

#define BMI088_accel_write_single_reg(reg, data) \
    {                                            \
        BMI088_ACCEL_NS_L();                     \
        BMI088_write_single_reg((reg), (data));  \
        BMI088_ACCEL_NS_H();                     \
    }
#define BMI088_accel_read_single_reg(reg, data) \
    {                                           \
        BMI088_ACCEL_NS_L();                    \
        BMI088_read_write_byte((reg) | 0x80);   \
        BMI088_read_write_byte(0x55);           \
        (data) = BMI088_read_write_byte(0x55);  \
        BMI088_ACCEL_NS_H();                    \
    }
//#define BMI088_accel_write_muli_reg( reg,  data, len) { BMI088_ACCEL_NS_L(); BMI088_write_muli_reg(reg, data, len); BMI088_ACCEL_NS_H(); }
#define BMI088_accel_read_muli_reg(reg, data, len) \
    {                                              \
        BMI088_ACCEL_NS_L();                       \
        BMI088_read_write_byte((reg) | 0x80);      \
        BMI088_read_muli_reg(reg, data, len);      \
        BMI088_ACCEL_NS_H();                       \
    }

#define BMI088_gyro_write_single_reg(reg, data) \
    {                                           \
        BMI088_GYRO_NS_L();                     \
        BMI088_write_single_reg((reg), (data)); \
        BMI088_GYRO_NS_H();                     \
    }
#define BMI088_gyro_read_single_reg(reg, data)  \
    {                                           \
        BMI088_GYRO_NS_L();                     \
        BMI088_read_single_reg((reg), &(data)); \
        BMI088_GYRO_NS_H();                     \
    }
//#define BMI088_gyro_write_muli_reg( reg,  data, len) { BMI088_GYRO_NS_L(); BMI088_write_muli_reg( ( reg ), ( data ), ( len ) ); BMI088_GYRO_NS_H(); }
#define BMI088_gyro_read_muli_reg(reg, data, len)   \
    {                                               \
        BMI088_GYRO_NS_L();                         \
        BMI088_read_muli_reg((reg), (data), (len)); \
        BMI088_GYRO_NS_H();                         \
    }

static void BMI088_write_single_reg(uint8_t reg, uint8_t data);
static void BMI088_read_single_reg(uint8_t reg, uint8_t *return_data);
//static void BMI088_write_muli_reg(uint8_t reg, uint8_t* buf, uint8_t len );
static void BMI088_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len);

#endif
		
static uint8_t write_BMI088_accel_reg_data_error[BMI088_WRITE_ACCEL_REG_NUM][3] =
    {
        {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR}, //�򿪼��ټ��Լ�״̬������λ
        {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},  //��ͣģʽ�Լ���Ӧ��״̬��
        {BMI088_ACC_CONF,  BMI088_ACC_NORMAL| BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},  //���ټĴ����Լ���Ӧ��״̬��
        {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},  //���ټ��趨��Χ�Լ���Ӧ״̬��
        {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR}, //�������롢�������INT1�Լ���Ӧ״̬��
        {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}  //�ж�ӳ�������Լ���Ӧ״̬��

};

static uint8_t write_BMI088_gyro_reg_data_error[BMI088_WRITE_GYRO_REG_NUM][3] =
    {
        {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR}, //�������Լ��ֱ��ʣ���Ӧ״̬��
        {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR}, //���ݴ���������������ã���Ӧ״̬��
        {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},   //��Դģʽѡ��Ĵ�������Ӧ״̬��
        {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},   //�����жϴ����Ĵ�������Ӧ״̬��
        {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},  //�ж����Ŵ����Ĵ�������Ӧ״̬��
        {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}   //�ж�ӳ��Ĵ�������Ӧ״̬��

};

uint8_t BMI088_init(void)
{
    uint8_t error = BMI088_NO_ERROR;

    error |= bmi088_accel_init();   //�жϼ��ټƳ�ʼ���Ƿ���ȷ
    error |= bmi088_gyro_init();    //�ж������ǳ�ʼ���Ƿ���ȷ
    delay_ms(5);
	  BMI088_Offset_Cali();
    return error;   //���ض�Ӧ����
}

bool_t bmi088_accel_init(void)
{
    uint8_t res = 0;
    uint8_t write_reg_num = 0;

    //���ͨѶ
	  BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, res);  //��ȡ���ټ�ID��ַ
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);         //�ȴ�150us�����ټ����óɹ�
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, res);   //����һ�飬����Ƿ�����
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    //���ټ������λ
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);  //���������λ��BMI088���������и�λ���ԼĴ���ACC_SOFTRESET,��ַΪ0x7Eд��0xB6�������������λ
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);//�����λ�ȴ�ʱ���ֲ���˵����1ms���������80���������ϵͳ�Ƚϸ��ӵģ�����10ms����20ms���С�

    // ͬ���ģ������λ�Ժ��ڼ��һ��ͨ���Ƿ�����
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // ��� "who am I"����Ĵ����Ƿ���ȷ
    if (res != BMI088_ACC_CHIP_ID_VALUE)  //�����0x00�Ĵ�����ֵ�����ص�ֵ����0x1E����û�и�λ�ɹ�
    {
        return BMI088_NO_SENSOR;
    }

    //�������������ô������Ļ�����Ϣ
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++)
    {
				/*�����񴫸������ڲ�д�����üĴ���ֵ�������ǣ�
				[0][0]  BMI088_ACC_PWR_CTRL 0x7D  //�������߹رռ��ټƣ�����Ϊ��Ӧ���趨��ֵ
			  [0][1]  BMI088_ACC_ENABLE_ACC_ON 0x04    //�򿪼��ټ�
			  [1][0]  BMI088_ACC_PWR_CONF 0x7C  //�����ټ��л�����ͣģʽ������ģʽ�£����ݲɼ�ֹͣ
			  [1][1]  BMI088_ACC_PWR_ACTIVE_MODE 0x00   //��ʼ
			  [2][0]  BMI088_ACC_CONF 0x40    //���ټ����üĴ���
			  [2][1]  BMI088_ACC_NORMAL (0x2 << BMI088_ACC_BWP_SHFITS)  //�������� | BMI088_ACC_800_HZ (0xB << BMI088_ACC_ODR_SHFITS)   //800hz |  BMI088_ACC_CONF_MUST_Set 0x80  ����ֵ0xBA  10111010
			  [3][0]  BMI088_ACC_RANGE 0x41   //���ټƷ�Χ�趨�Ĵ���
			  [3][1]  BMI088_ACC_RANGE_3G (0x0 << BMI088_ACC_RANGE_SHFITS)   //+-3g
			  [4][0]  BMI088_INT1_IO_CTRL 0x53   //�������롢�������INT1
			  [4][1]  BMI088_ACC_INT1_IO_ENABLE (0x1 << BMI088_ACC_INT1_IO_ENABLE_SHFITS)  //INT����Ϊ������� | BMI088_ACC_INT1_GPIO_PP (0x0 << BMI088_ACC_INT1_GPIO_MODE_SHFITS)  //������� | BMI088_ACC_INT1_GPIO_LOW (0x0 << BMI088_ACC_INT1_GPIO_LVL_SHFITS)  //����
			  [5][0]  BMI088_INT_MAP_DATA 0x58  //�ж�ӳ�䵽INT1,INT2�ϣ�֮�����ͨ���ⲿ�жϽ��д���
			  [5][1]  BMI088_ACC_INT1_DRDY_INTERRUPT (0x1 << BMI088_ACC_INT1_DRDY_INTERRUPT_SHFITS)  //ӳ�䵽INT1��֮������ⲿ�жϴ���INT1���й���
				*/
        BMI088_accel_write_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]); 
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);  //�ȴ�150ms

        BMI088_accel_read_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], res); //��ȡ�������üĴ�����ID���Ƿ���ȷ
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        if (res != write_BMI088_accel_reg_data_error[write_reg_num][1])  //����ȡ�����üĴ�����ֵ�������趨��ֵ�Ƚ�
        {
            return write_BMI088_accel_reg_data_error[write_reg_num][2];  //��������Ӧ������Ӧ�Ĵ���
        }
    }
    return BMI088_NO_ERROR;  //�ޣ���û�д��󣬱�־�ų�ʼ���ɹ�
}

bool_t bmi088_gyro_init(void)
{
    uint8_t write_reg_num = 0;
    uint8_t res = 0;

    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, res);  // ��ȡ�����ǵ�ID��ַ
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);  //��ʱ150ms
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    //�������ݴ�����
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);  //������������
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);
    //��λ����ͨ���Ƿ�����
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, res);  //�����Ժ��ٴζ�ȡID��
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    if (res != BMI088_GYRO_CHIP_ID_VALUE)  //�ж�ID���Ƿ���ȷ
    {
        return BMI088_NO_SENSOR;   //����ȷ�������ô���
    }

    //�������ݴ��������ò����
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++)
    {
        /*�����񴫸������ڲ�д�����üĴ���ֵ�������ǣ�
				[0][0]  BMI088_GYRO_RANGE 0x0F  //�����ʷ�Χ�Լ��ֱ���
			  [0][1]  BMI088_GYRO_2000 (0x0 << BMI088_GYRO_RANGE_SHFITS)  //+-2000��/s
			  [1][0]  BMI088_GYRO_BANDWIDTH 0x10   //�������������˲���������������
			  [1][1]  BMI088_GYRO_2000_532_HZ �����ݴ�������Ϊ2kHZ,����Ϊ532hz
			  [2][0]  BMI088_GYRO_LPM1 0x11  //��Դģʽ��ѡ��Ĵ���
			  [2][1]  BMI088_GYRO_NORMAL_MODE 0x00  //����ģʽ
			  [3][0]  BMI088_GYRO_CTRL 0x15   //�����жϴ����Ĵ���
			  [3][1]  BMI088_DRDY_ON 0x80     //�����µ����ݴ����µ������ж�
			  [4][0]  BMI088_GYRO_INT3_INT4_IO_CONF 0x16  //�ж��������üĴ���
			  [4][1]  BMI088_GYRO_INT3_GPIO_PP (0x0 << BMI088_GYRO_INT3_GPIO_MODE_SHFITS)  //INT3������� | BMI088_GYRO_INT3_GPIO_LOW (0x0 << BMI088_GYRO_INT3_GPIO_LVL_SHFITS)  //INT3���� | 
			  [5][0]  BMI088_GYRO_INT3_INT4_IO_MAP 0x18  //�ж�ӳ��Ĵ���
			  [5][1]  #define BMI088_GYRO_DRDY_IO_INT4 0x80  //ӳ�䵽INT4
				*/
        BMI088_gyro_write_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], write_BMI088_gyro_reg_data_error[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        BMI088_gyro_read_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], res);   //��ȡ�������üĴ�����ID��
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1])  //�ж�ID���Ƿ���ȷ
        {
            return write_BMI088_gyro_reg_data_error[write_reg_num][2];  //����Ͷ�Ӧ���ش���
        }
    }

    return BMI088_NO_ERROR;   //�޴��󣬳�ʼ����ȷ
}





void BMI088_read(fp32 gyro[3], fp32 accel[3], fp32 *temperate)   //���ټ��������������ᣬ�����������飬����Ϊ3
{
    uint8_t buf[8] = {0, 0, 0, 0, 0, 0};
    int16_t bmi088_raw_temp;

    BMI088_accel_read_muli_reg(BMI088_ACCEL_XOUT_L, buf, 6);

    bmi088_raw_temp = (int16_t)((buf[1]) << 8) | buf[0]; //���㹫ʽ����ʹ���ֲᣬ���ｫx��ĸ߰�λ����8λ�����൱�ڳ���256��Ҫ�����ƣ����ǳ���256������ע��һ��Ҫǿ�Ƹ�Ϊint�������ݣ���Ϊ���ս���ϴ󣬳���char�����ݷ�Χ�������Ҫת��
    mpu_data.ax = bmi088_raw_temp ;
	//	accel[0] = bmi088_raw_temp * BMI088_ACCEL_SEN;  //�����ϵ��������0x41���ټƷ�Χ�趨�Ĵ�����ַ�ļĴ��������м���ó�
    bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2];
    mpu_data.ay = bmi088_raw_temp ; 
	//	accel[1] = bmi088_raw_temp * BMI088_ACCEL_SEN;
 
		bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
    mpu_data.az = bmi088_raw_temp ;
	//	accel[2] = bmi088_raw_temp * BMI088_ACCEL_SEN;

    BMI088_gyro_read_muli_reg(BMI088_GYRO_CHIP_ID, buf, 8);  //��һ����������Ҫ�ǽ�֮ǰ���ټ�ģʽ�£�����������ģʽ����Ԥ�������ݻ�����
    if(buf[0] == BMI088_GYRO_CHIP_ID_VALUE)   //�����֤��������ģʽ�ˣ��ſ�����
    {
        bmi088_raw_temp = (int16_t)((buf[3]) << 8) | buf[2] ;  //���㷽���ο��ֲᣬ������λ����
      mpu_data.gx = bmi088_raw_temp - mpu_data.gx_offset;
		//	gyro[0] = (bmi088_raw_temp - bmi088_real_data.gyro_offest[0]) * BMI088_GYRO_SEN;     //ͬ���ģ������ϵ�����ݽ����ʷ�Χ�Լ��ֱ�������������
        bmi088_raw_temp = (int16_t)((buf[5]) << 8) | buf[4];
       mpu_data.gy = bmi088_raw_temp - mpu_data.gy_offset;
		  //	gyro[1] = (bmi088_raw_temp - bmi088_real_data.gyro_offest[1]) * BMI088_GYRO_SEN;
        bmi088_raw_temp = (int16_t)((buf[7]) << 8) | buf[6];
       mpu_data.gz = bmi088_raw_temp - mpu_data.gz_offset;
			//gyro[2] = (bmi088_raw_temp - bmi088_real_data.gyro_offest[2]) * BMI088_GYRO_SEN;
    }
    BMI088_accel_read_muli_reg(BMI088_TEMP_M, buf, 2);   //д���¶ȣ���ȡ�¶�

    bmi088_raw_temp = (int16_t)((buf[0] << 3) | (buf[1] >> 5));   //�¶ȼ��㷽��������һ��

    if (bmi088_raw_temp > 1023)   //�ֲ�����������ɼ��ص��¶�ֵ����1023������������´���
    {
        bmi088_raw_temp -= 2048;
    }

	  if(abs(mpu_data.gz)< 10)
	  {
		    mpu_data.gz = 0;
	  }

		
//		memcpy(&accel[0],&mpu_data.ax,6*sizeof(int16_t));
//		memcpy(&accel[1],&mpu_data.ay,6*sizeof(int16_t));
//		memcpy(&accel[2],&mpu_data.az,6*sizeof(int16_t));
    *temperate = bmi088_raw_temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;   //���������¶�ֵ
		accel[0] = mpu_data.ax * BMI088_ACCEL_SEN;
		accel[1] = mpu_data.ay * BMI088_ACCEL_SEN;
		accel[2] = mpu_data.az * BMI088_ACCEL_SEN;
		gyro[0] = mpu_data.gx * BMI088_GYRO_SEN;
	  gyro[1] = mpu_data.gy * BMI088_GYRO_SEN;
	  gyro[2] = mpu_data.gz * BMI088_GYRO_SEN;
		
}

void BMI088_Offset_Cali(void)
{
	
//	uint8_t buf[8] = {0, 0, 0, 0, 0, 0};

//	for(int i=0;i<1000;i++)
//	{
//		BMI088_accel_read_muli_reg(BMI088_ACCEL_XOUT_L, buf, 6);
//		BMI088_gyro_read_muli_reg(BMI088_GYRO_CHIP_ID, buf, 8);
//		
//		mpu_data.ax_offset += (int16_t)((buf[1]) << 8) | buf[0];
//		mpu_data.ay_offset += (int16_t)((buf[3]) << 8) | buf[2];
//		mpu_data.az_offset += (int16_t)((buf[5]) << 8) | buf[4];
//	
//		mpu_data.gx_offset += (int16_t)((buf[3]) << 8) | buf[2];
//		mpu_data.gy_offset += (int16_t)((buf[5]) << 8) | buf[4];
//		mpu_data.gz_offset += (int16_t)((buf[7]) << 8) | buf[6];
//	
//		delay_xms(5);
//	}
//	
//	mpu_data.ax_offset=mpu_data.ax_offset / 1000;
//	mpu_data.ay_offset=mpu_data.ay_offset / 1000;
//	mpu_data.az_offset=mpu_data.az_offset / 1000;
//	mpu_data.gx_offset=mpu_data.gx_offset / 1000;
//	mpu_data.gy_offset=mpu_data.gy_offset / 1000;
//	mpu_data.gz_offset=mpu_data.gz_offset / 1000;
	
	mpu_data.ax_offset=2;
	mpu_data.ay_offset=2;
	mpu_data.az_offset=-6;
	mpu_data.gx_offset=2;
	mpu_data.gy_offset=-6;
	mpu_data.gz_offset=-4;
}
		
#if defined(BMI088_USE_SPI)

static void BMI088_write_single_reg(uint8_t reg, uint8_t data)
{
	
    BMI088_read_write_byte(reg);//�Ƚ���ַд�봫����
    BMI088_read_write_byte(data);//�ڽ��Ĵ���ֵд�봫����
}

static void BMI088_read_single_reg(uint8_t reg, uint8_t *return_data)
{
    BMI088_read_write_byte(reg | 0x80);  //����ΪʲôҪ��������0x80�أ���Ϊ�ֲ������ᵽ����������ģʽ�£�Ҫ�����µ������жϣ�ͨ����0x80д��Ĵ���ȥ����������ģʽҲ��һ����
    *return_data = BMI088_read_write_byte(0x55);
}

//static void BMI088_write_muli_reg(uint8_t reg, uint8_t* buf, uint8_t len )
//{
//    BMI088_read_write_byte( reg );
//    while( len != 0 )
//    {

//        BMI088_read_write_byte( *buf );
//        buf ++;
//        len --;
//    }

//}

// �˺�����Ҫ��ȡ�ļĴ���ֵ���ӳ��������жϣ����ν�����뵽ָ�������ݻ�������
static void BMI088_read_muli_reg(uint8_t reg, uint8_t *buf, uint8_t len)
{
    BMI088_read_write_byte(reg | 0x80);

    while (len != 0)
    {

        *buf = BMI088_read_write_byte(0x55); 
        buf++;
        len--;
    }
}
#endif




