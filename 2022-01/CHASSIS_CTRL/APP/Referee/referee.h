#ifndef __REFEREE_H
#define __REFEREE_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private struct ------------------------------------------------------------*/
//裁判系统数据
typedef struct  //裁判系统数据包
{
    u8 SOF;       //帧头
    u16 Length;   //数据包长度
    u16 Info_ID;  //命令ID
    u8  Seq;      //包序号
}tReferee_System_Data;

//比赛状态数据据：0x0001
typedef __packed struct
{
    uint8_t game_type : 4;      //比赛类型
    uint8_t game_progress : 4;  //当前比赛阶段
    uint16_t stage_remain_time; //当前阶段剩余时间
    uint64_t SyncTimeStamp;     //的精确 Unix 时间
} ext_game_status_t; 

//比赛结果数据据：0x0002
typedef __packed struct
{   
 uint8_t winner;     //0 平局 1 红方胜利 2 蓝方胜利
} ext_game_result_t;

//机器人血量数据：0x0003
typedef __packed struct
{
    uint16_t red_1_robot_HP;    //红1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t red_2_robot_HP;    //红2工程机器人血量，未上场以及罚下血量为 0
    uint16_t red_3_robot_HP;    //红3步兵机器人血量，未上场以及罚下血量为 0
    uint16_t red_4_robot_HP;    //红4步兵机器人血量，未上场以及罚下血量为 0
    uint16_t red_5_robot_HP;    //红5步兵机器人血量，未上场以及罚下血量为 0
    uint16_t red_7_robot_HP;    //红7哨兵机器人血量，未上场以及罚下血量为 0
    uint16_t red_outpost_HP;    //红方前哨战血量，未上场以及罚下血量为 0
    uint16_t red_base_HP;       //红方基地血量，未上场以及罚下血量为 0
    uint16_t blue_1_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_2_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_3_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_4_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_5_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_7_robot_HP;   //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_outpost_HP;    //蓝1英雄机器人血量，未上场以及罚下血量为 0
    uint16_t blue_base_HP;      //蓝1英雄机器人血量，未上场以及罚下血量为 0
} ext_game_robot_HP_t;

//飞镖发射状态：0x0004
typedef __packed struct
{
    uint8_t dart_belong;            //发射飞镖的队伍： 1：红方飞镖  2：蓝方飞镖
    uint16_t stage_remaining_time;  //发射时的剩余比赛时间，单位 s
} ext_dart_status_t;

//人工智能挑战赛加成与惩罚区状态：0x0005
typedef __packed struct
{
    uint8_t F1_zone_status:1;              //bit[0, 4, 8, 12, 16, 20]为 F1-F6 激活状态： 0为未激活,1为已激活
    uint8_t F1_zone_buff_debuff_status:3; 
    uint8_t F2_zone_status:1;
    uint8_t F2_zone_buff_debuff_status:3; 
    uint8_t F3_zone_status:1;
    uint8_t F3_zone_buff_debuff_status:3; 
    uint8_t F4_zone_status:1;
    uint8_t F4_zone_buff_debuff_status:3; 
    uint8_t F5_zone_status:1;
    uint8_t F5_zone_buff_debuff_status:3; 
    uint8_t F6_zone_status:1;
    uint8_t F6_zone_buff_debuff_status:3;
    uint16_t red1_bullet_left;             //红方1号剩余弹量
    uint16_t red2_bullet_left;             //红方2号剩余弹量
    uint16_t blue1_bullet_left;            //蓝方1号剩余弹量
    uint16_t blue2_bullet_left;            //蓝方2号剩余弹量
} ext_ICRA_buff_debuff_zone_status_t;

//比赛机器人状态：0x0201
typedef __packed struct
{
    uint8_t robot_id;                           //本机器人ID
    uint8_t robot_level;                        //机器人等级
    uint16_t remain_HP;                         //机器人剩余血量
    uint16_t max_HP;                            //机器人上限血量
    uint16_t shooter_id1_17mm_cooling_rate;     //机器人1号17mm枪口每秒冷却值
    uint16_t shooter_id1_17mm_cooling_limit;    //机器人1号17mm枪口热量上限
    uint16_t shooter_id1_17mm_speed_limit;     //机器人1号17mm枪口上限速度
    uint16_t shooter_id2_17mm_cooling_rate;     //机器人2号17mm枪口每秒冷却值
    uint16_t shooter_id2_17mm_cooling_limit;    //机器人2号17mm枪口热量上限
    uint16_t shooter_id2_17mm_speed_limit;      //机器人2号17mm枪口上限速度
    uint16_t shooter_id1_42mm_cooling_rate;     //机器人42mm枪口每秒冷却值
    uint16_t shooter_id1_42mm_cooling_limit;    //机器人42mm枪口热量上限
    uint16_t shooter_id1_42mm_speed_limit;    //机器人42mm枪口上限速度
    uint16_t chassis_power_limit;             //机器人底盘功率限制上限
    uint8_t mains_power_gimbal_output : 1;      //0 bit：gimbal口输出： 1为有24V输出，0为无24v输出
    uint8_t mains_power_chassis_output : 1;     //1 bit：chassis口输出：1为有24V输出，0为无24v输出
    uint8_t mains_power_shooter_output : 1;     //2 bit：shooter口输出：1为有24V输出，0为无24v输出
} ext_game_robot_status_t;

//实时功率热量数据：0x0202 
typedef __packed struct       
{
 uint16_t chassis_volt;                   //底盘输出电压 单位毫伏
 uint16_t chassis_current;                //底盘输出电流 单位毫安
 float chassis_power;                     //底盘输出功率 单位W
 uint16_t chassis_power_buffer;           //底盘功率缓冲 单位J   备注：飞坡根据规则增加至250J
 uint16_t shooter_id1_17mm_cooling_heat;  //1号17mm枪口热量
 uint16_t shooter_id2_17mm_cooling_heat;  //2号17mm枪口热量
 uint16_t shooter_id1_42mm_cooling_heat;  //42mm枪口热量
} ext_power_heat_data_t;

//机器人增益：0x0204
typedef __packed struct
{
 uint8_t power_rune_buff;  //bit0：机器人血量补血状态、 1：枪口热量冷却加速、2：机器人防御加成、3：机器人攻击加成
}ext_buff_t;

//空中机器人能量状态：0x0205
typedef __packed struct    
{
	uint8_t attack_time;      //可攻击时间 单位s:30s递减至0
} aerial_robot_energy_t;

//伤害状态：0x0206
typedef __packed struct
{
	uint8_t armor_id : 4;    //bit 0-3：受击装甲ID:0-4号代表机器人的五个装甲片，其他血量变化类型，该变量数值为0
	uint8_t hurt_type : 4;   //bit 4-7：血量变化类型:0装甲伤害扣血 1模块掉线扣血 2超射速扣血 3超枪口热量扣血 4超底盘功率扣血 5装甲撞击扣血
} ext_robot_hurt_t;

//实时射击信息：0x0207
typedef __packed struct
{
  uint8_t bullet_type; //子弹类型:1:17mm 弹丸 2:42mm 弹丸
  uint8_t shooter_id;  //发射机构 ID：1:1号17mm 发射机构  2:2号17mm发射机构  3:42mm发射机构
  uint8_t bullet_freq; //子弹射频 单位Hz
  float bullet_speed;  //子弹射速 单位m/s
} ext_shoot_data_t;

//飞镖机器人客户端指令数据：0x020A
typedef __packed struct
{
 uint8_t dart_launch_opening_status;   //当前飞镖发射口的状态 0:关闭 1:正在开启或者关闭中 2:已经开启
 uint8_t dart_attack_target;           //飞镖的打击目标  1:前哨站  2:基地
 uint16_t target_change_time;          //切换打击目标时的比赛剩余时间
 uint8_t first_dart_speed;             //检测到的第一枚飞镖速度,单位0.1m/s/LSB
 uint8_t second_dart_speed;            //检测到的第二枚飞镖速度,单位0.1m/s/LSB
 uint8_t third_dart_speed;             //检测到的第三枚飞镖速度,单位0.1m/s/LSB
 uint8_t fourth_dart_speed;            //检测到的第四枚飞镖速度,单位0.1m/s/LSB
 uint16_t last_dart_launch_time;       //最近一次的发射飞镖的比赛剩余时间,单位秒
 uint16_t operate_launch_cmd_time;     //最近一次操作手确定发射指令时的比赛剩余时间
} ext_dart_client_cmd_t;

//场地事件数据：0x0101
typedef __packed struct
{
    uint32_t event_type;     //详细见裁判系统通讯协议手册  P10
} ext_event_data_t;

//补给站动作标识：0x0102
typedef __packed struct
{
    uint8_t supply_projectile_id;    //补给站口ID：  1：1号补给口  2：2号补给口
    uint8_t supply_robot_id;         //补弹机器人 ID：0为当前无机器人补弹，1为红方英雄机器人补弹，2为红方工程机器人补弹，3/4/5为红方步兵机器人补弹，101为蓝方英雄机器人补弹，102为蓝方工程机器人补弹，103/104/105为蓝方步兵机器人补弹
    uint8_t supply_projectile_step;  //出弹口开闭状态：0为关闭，1为子弹准备中，2为子弹下落
    uint8_t supply_projectile_num;    //补弹数量
} ext_supply_projectile_action_t;

//裁判警告信息：0x0104
typedef __packed struct
{
    uint8_t level;         //警告等级：1：黄牌 2：红牌  3：判负
    uint8_t foul_robot_id; //犯规机器人ID：判负时，机器人ID为 0  黄牌、红牌时，机器人ID为犯规机器人ID
} ext_referee_warning_t;

//飞镖发射口倒计时：0x0105
typedef __packed struct
{
    uint8_t dart_remaining_time;  //15s倒计时
} ext_dart_remaining_time_t;

//机器人位置：0x0203
typedef __packed struct
{
    float x;       //位置x坐标，单位m
    float y;       //位置y坐标，单位m
    float z;       //位置z坐标，单位m
    float yaw;     //位置枪口，单位度
} ext_game_robot_pos_t;

typedef __packed struct//子弹剩余发射数：0x0208
{
    uint16_t bullet_remaining_num_17mm;   //17mm 子弹剩余发射数目
    uint16_t bullet_remaining_num_42mm;   //42mm 子弹剩余发射数目
    uint16_t coin_remaining_num;          //剩余金币数量
} ext_bullet_remaining_t;

//机器人 RFID 状态：0x0209
typedef __packed struct
{
    uint32_t rfid_status;    //详细见裁判系统通讯协议手册 P17
} ext_rfid_status_t;

extern uint8_t referee_system_data[60],referee_system_num;  //裁判系统接收数组和数据长度

extern void REFEREE_SYSTEM_ANALYSIS(void);

#endif


