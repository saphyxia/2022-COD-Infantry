#ifndef BMI088REG_H
#define BMI088REG_H

// 这部分为加速度计的寄存器
#define BMI088_ACC_CHIP_ID 0x00 // the register is  " Who am I "
#define BMI088_ACC_CHIP_ID_VALUE 0x1E

#define BMI088_ACC_ERR_REG 0x02
#define BMI088_ACCEL_CONGIF_ERROR_SHFITS 0x2
#define BMI088_ACCEL_CONGIF_ERROR (1 << BMI088_ACCEL_CONGIF_ERROR_SHFITS)
#define BMI088_FATAL_ERROR_SHFITS 0x0
#define BMI088_FATAL_ERROR (1 << BMI088_FATAL_ERROR)

#define BMI088_ACC_STATUS 0x03
#define BMI088_ACCEL_DRDY_SHFITS 0x7
#define BMI088_ACCEL_DRDY (1 << BMI088_ACCEL_DRDY_SHFITS)

#define BMI088_ACCEL_XOUT_L 0x12   //加速计三轴输出寄存器
#define BMI088_ACCEL_XOUT_M 0x13
#define BMI088_ACCEL_YOUT_L 0x14
#define BMI088_ACCEL_YOUT_M 0x15
#define BMI088_ACCEL_ZOUT_L 0x16
#define BMI088_ACCEL_ZOUT_M 0x17

#define BMI088_SENSORTIME_DATA_L 0x18
#define BMI088_SENSORTIME_DATA_M 0x19
#define BMI088_SENSORTIME_DATA_H 0x1A

#define BMI088_ACC_INT_STAT_1 0x1D
#define BMI088_ACCEL_DRDY_INTERRUPT_SHFITS 0x7
#define BMI088_ACCEL_DRDY_INTERRUPT (1 << BMI088_ACCEL_DRDY_INTERRUPT_SHFITS)

#define BMI088_TEMP_M 0x22

#define BMI088_TEMP_L 0x23

#define BMI088_ACC_CONF 0x40    //加速计配置寄存器
//bit7-bit4
#define BMI088_ACC_CONF_MUST_Set 0x80
#define BMI088_ACC_BWP_SHFITS 0x4
#define BMI088_ACC_OSR4 (0x0 << BMI088_ACC_BWP_SHFITS)  //4倍采样频率
#define BMI088_ACC_OSR2 (0x1 << BMI088_ACC_BWP_SHFITS)  //2倍采样频率
#define BMI088_ACC_NORMAL (0x2 << BMI088_ACC_BWP_SHFITS)  //正常采样

//bit3-bit0,输出速率
#define BMI088_ACC_ODR_SHFITS 0x0
#define BMI088_ACC_12_5_HZ (0x5 << BMI088_ACC_ODR_SHFITS)  //12.5hz
#define BMI088_ACC_25_HZ (0x6 << BMI088_ACC_ODR_SHFITS)    //25hz
#define BMI088_ACC_50_HZ (0x7 << BMI088_ACC_ODR_SHFITS)    //50hz
#define BMI088_ACC_100_HZ (0x8 << BMI088_ACC_ODR_SHFITS)   //100hz
#define BMI088_ACC_200_HZ (0x9 << BMI088_ACC_ODR_SHFITS)   //200hz
#define BMI088_ACC_400_HZ (0xA << BMI088_ACC_ODR_SHFITS)   //400hz
#define BMI088_ACC_800_HZ (0xB << BMI088_ACC_ODR_SHFITS)   //800hz
#define BMI088_ACC_1600_HZ (0xC << BMI088_ACC_ODR_SHFITS)  //1.6khz

#define BMI088_ACC_RANGE 0x41   //加速计范围设定寄存器
#define BMI088_ACC_RANGE_SHFITS 0x0  //移位的位
#define BMI088_ACC_RANGE_3G (0x0 << BMI088_ACC_RANGE_SHFITS)   //+-3g
#define BMI088_ACC_RANGE_6G (0x1 << BMI088_ACC_RANGE_SHFITS)   //+-6g
#define BMI088_ACC_RANGE_12G (0x2 << BMI088_ACC_RANGE_SHFITS)  //+-12g
#define BMI088_ACC_RANGE_24G (0x3 << BMI088_ACC_RANGE_SHFITS)  //+-24g

#define BMI088_INT1_IO_CTRL 0x53   //配置输入、输出引脚INT1
#define BMI088_ACC_INT1_IO_ENABLE_SHFITS 0x3
#define BMI088_ACC_INT1_IO_ENABLE (0x1 << BMI088_ACC_INT1_IO_ENABLE_SHFITS)  //INT配置为输出引脚
#define BMI088_ACC_INT1_GPIO_MODE_SHFITS 0x2
#define BMI088_ACC_INT1_GPIO_PP (0x0 << BMI088_ACC_INT1_GPIO_MODE_SHFITS)  //推挽输出
#define BMI088_ACC_INT1_GPIO_OD (0x1 << BMI088_ACC_INT1_GPIO_MODE_SHFITS)  //开漏输出
#define BMI088_ACC_INT1_GPIO_LVL_SHFITS 0x1
#define BMI088_ACC_INT1_GPIO_LOW (0x0 << BMI088_ACC_INT1_GPIO_LVL_SHFITS)  //下拉
#define BMI088_ACC_INT1_GPIO_HIGH (0x1 << BMI088_ACC_INT1_GPIO_LVL_SHFITS) //上拉

#define BMI088_INT2_IO_CTRL 0x54
#define BMI088_ACC_INT2_IO_ENABLE_SHFITS 0x3
#define BMI088_ACC_INT2_IO_ENABLE (0x1 << BMI088_ACC_INT2_IO_ENABLE_SHFITS)
#define BMI088_ACC_INT2_GPIO_MODE_SHFITS 0x2
#define BMI088_ACC_INT2_GPIO_PP (0x0 << BMI088_ACC_INT2_GPIO_MODE_SHFITS)
#define BMI088_ACC_INT2_GPIO_OD (0x1 << BMI088_ACC_INT2_GPIO_MODE_SHFITS)
#define BMI088_ACC_INT2_GPIO_LVL_SHFITS 0x1
#define BMI088_ACC_INT2_GPIO_LOW (0x0 << BMI088_ACC_INT2_GPIO_LVL_SHFITS)
#define BMI088_ACC_INT2_GPIO_HIGH (0x1 << BMI088_ACC_INT2_GPIO_LVL_SHFITS)

#define BMI088_INT_MAP_DATA 0x58  //中断映射到INT1,INT2上，之后可以通过外部中断进行触发，当然这里当然是映射到INT1了，因为就只配置了INT1，嘿嘿
#define BMI088_ACC_INT2_DRDY_INTERRUPT_SHFITS 0x6
#define BMI088_ACC_INT2_DRDY_INTERRUPT (0x1 << BMI088_ACC_INT2_DRDY_INTERRUPT_SHFITS)  //映射到INT2
#define BMI088_ACC_INT1_DRDY_INTERRUPT_SHFITS 0x2
#define BMI088_ACC_INT1_DRDY_INTERRUPT (0x1 << BMI088_ACC_INT1_DRDY_INTERRUPT_SHFITS)  //映射到INT1

#define BMI088_ACC_SELF_TEST 0x6D
#define BMI088_ACC_SELF_TEST_OFF 0x00
#define BMI088_ACC_SELF_TEST_POSITIVE_SIGNAL 0x0D
#define BMI088_ACC_SELF_TEST_NEGATIVE_SIGNAL 0x09

#define BMI088_ACC_PWR_CONF 0x7C  //将加速计切换到暂停模式，这种模式下，数据采集停止
#define BMI088_ACC_PWR_SUSPEND_MODE 0x03  //暂停
#define BMI088_ACC_PWR_ACTIVE_MODE 0x00   //开始

#define BMI088_ACC_PWR_CTRL 0x7D  //开启或者关闭加速计，底下为其应该设定的值
#define BMI088_ACC_ENABLE_ACC_OFF 0x00   //关闭加速计
#define BMI088_ACC_ENABLE_ACC_ON 0x04    //打开加速计

#define BMI088_ACC_SOFTRESET 0x7E   //软件复位寄存器
#define BMI088_ACC_SOFTRESET_VALUE 0xB6  //软件复位寄存器写入的值


//这部分为陀螺仪的寄存器
#define BMI088_GYRO_CHIP_ID 0x00
#define BMI088_GYRO_CHIP_ID_VALUE 0x0F

#define BMI088_GYRO_X_L 0x02
#define BMI088_GYRO_X_H 0x03
#define BMI088_GYRO_Y_L 0x04
#define BMI088_GYRO_Y_H 0x05
#define BMI088_GYRO_Z_L 0x06
#define BMI088_GYRO_Z_H 0x07

#define BMI088_GYRO_INT_STAT_1 0x0A
#define BMI088_GYRO_DYDR_SHFITS 0x7
#define BMI088_GYRO_DYDR (0x1 << BMI088_GYRO_DYDR_SHFITS)

#define BMI088_GYRO_RANGE 0x0F  //角速率范围以及分辨率
#define BMI088_GYRO_RANGE_SHFITS 0x0
#define BMI088_GYRO_2000 (0x0 << BMI088_GYRO_RANGE_SHFITS)  //+-2000°/s
#define BMI088_GYRO_1000 (0x1 << BMI088_GYRO_RANGE_SHFITS)  //+-1000°/s
#define BMI088_GYRO_500 (0x2 << BMI088_GYRO_RANGE_SHFITS)   //+-500°/s
#define BMI088_GYRO_250 (0x3 << BMI088_GYRO_RANGE_SHFITS)   //+-250°/s
#define BMI088_GYRO_125 (0x4 << BMI088_GYRO_RANGE_SHFITS)   //+-125°/s

#define BMI088_GYRO_BANDWIDTH 0x10   //设置速率数据滤波器带宽和输出速率
// 下列第一个表示传输数据速率、第二表示带宽，例如BMI088_GYRO_2000_532_HZ ，数据传输速率为2kHZ,带宽为532hz
#define BMI088_GYRO_BANDWIDTH_MUST_Set 0x80
#define BMI088_GYRO_2000_532_HZ 0x00
#define BMI088_GYRO_2000_230_HZ 0x01
#define BMI088_GYRO_1000_116_HZ 0x02
#define BMI088_GYRO_400_47_HZ 0x03
#define BMI088_GYRO_200_23_HZ 0x04
#define BMI088_GYRO_100_12_HZ 0x05
#define BMI088_GYRO_200_64_HZ 0x06
#define BMI088_GYRO_100_32_HZ 0x07

#define BMI088_GYRO_LPM1 0x11  //电源模式的选择寄存器
#define BMI088_GYRO_NORMAL_MODE 0x00  //正常模式
#define BMI088_GYRO_SUSPEND_MODE 0x80   //挂起模式
#define BMI088_GYRO_DEEP_SUSPEND_MODE 0x20  //深度挂起模式

#define BMI088_GYRO_SOFTRESET 0x14  //重置传感器寄存器
#define BMI088_GYRO_SOFTRESET_VALUE 0xB6  //发送0xB6到此寄存器进行重置

#define BMI088_GYRO_CTRL 0x15   //数据中断触发寄存器
#define BMI088_DRDY_OFF 0x00    //没有数据，准备中断触发
#define BMI088_DRDY_ON 0x80     //允许新的数据触发新的数据中断

#define BMI088_GYRO_INT3_INT4_IO_CONF 0x16  //中断引脚配置寄存器
#define BMI088_GYRO_INT4_GPIO_MODE_SHFITS 0x3
#define BMI088_GYRO_INT4_GPIO_PP (0x0 << BMI088_GYRO_INT4_GPIO_MODE_SHFITS)  //INT4推挽输出
#define BMI088_GYRO_INT4_GPIO_OD (0x1 << BMI088_GYRO_INT4_GPIO_MODE_SHFITS)  //INT4开漏输出
#define BMI088_GYRO_INT4_GPIO_LVL_SHFITS 0x2
#define BMI088_GYRO_INT4_GPIO_LOW (0x0 << BMI088_GYRO_INT4_GPIO_LVL_SHFITS)  //INT4下拉
#define BMI088_GYRO_INT4_GPIO_HIGH (0x1 << BMI088_GYRO_INT4_GPIO_LVL_SHFITS) //INT4上拉
#define BMI088_GYRO_INT3_GPIO_MODE_SHFITS 0x1
#define BMI088_GYRO_INT3_GPIO_PP (0x0 << BMI088_GYRO_INT3_GPIO_MODE_SHFITS)  //INT3推挽输出
#define BMI088_GYRO_INT3_GPIO_OD (0x1 << BMI088_GYRO_INT3_GPIO_MODE_SHFITS)  //INT3开漏输出
#define BMI088_GYRO_INT3_GPIO_LVL_SHFITS 0x0
#define BMI088_GYRO_INT3_GPIO_LOW (0x0 << BMI088_GYRO_INT3_GPIO_LVL_SHFITS)  //INT3下拉
#define BMI088_GYRO_INT3_GPIO_HIGH (0x1 << BMI088_GYRO_INT3_GPIO_LVL_SHFITS) //INT3上拉

#define BMI088_GYRO_INT3_INT4_IO_MAP 0x18  //中断映射寄存器
#define BMI088_GYRO_DRDY_IO_OFF 0x00  //无映射
#define BMI088_GYRO_DRDY_IO_INT3 0x01  //映射到INT3
#define BMI088_GYRO_DRDY_IO_INT4 0x80  //映射到INT4
#define BMI088_GYRO_DRDY_IO_BOTH (BMI088_GYRO_DRDY_IO_INT3 | BMI088_GYRO_DRDY_IO_INT4)   //映射到INT3和INT4

#define BMI088_GYRO_SELF_TEST 0x3C
#define BMI088_GYRO_RATE_OK_SHFITS 0x4
#define BMI088_GYRO_RATE_OK (0x1 << BMI088_GYRO_RATE_OK_SHFITS)
#define BMI088_GYRO_BIST_FAIL_SHFITS 0x2
#define BMI088_GYRO_BIST_FAIL (0x1 << BMI088_GYRO_BIST_FAIL_SHFITS)
#define BMI088_GYRO_BIST_RDY_SHFITS 0x1
#define BMI088_GYRO_BIST_RDY (0x1 << BMI088_GYRO_BIST_RDY_SHFITS)
#define BMI088_GYRO_TRIG_BIST_SHFITS 0x0
#define BMI088_GYRO_TRIG_BIST (0x1 << BMI088_GYRO_TRIG_BIST_SHFITS)

#endif
