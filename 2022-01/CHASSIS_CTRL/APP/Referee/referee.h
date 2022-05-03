#ifndef __REFEREE_H
#define __REFEREE_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Private struct ------------------------------------------------------------*/
//����ϵͳ����
typedef struct  //����ϵͳ���ݰ�
{
    u8 SOF;       //֡ͷ
    u16 Length;   //���ݰ�����
    u16 Info_ID;  //����ID
    u8  Seq;      //�����
}tReferee_System_Data;

//����״̬���ݾݣ�0x0001
typedef __packed struct
{
    uint8_t game_type : 4;      //��������
    uint8_t game_progress : 4;  //��ǰ�����׶�
    uint16_t stage_remain_time; //��ǰ�׶�ʣ��ʱ��
    uint64_t SyncTimeStamp;     //�ľ�ȷ Unix ʱ��
} ext_game_status_t; 

//����������ݾݣ�0x0002
typedef __packed struct
{   
 uint8_t winner;     //0 ƽ�� 1 �췽ʤ�� 2 ����ʤ��
} ext_game_result_t;

//������Ѫ�����ݣ�0x0003
typedef __packed struct
{
    uint16_t red_1_robot_HP;    //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_2_robot_HP;    //��2���̻�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_3_robot_HP;    //��3����������Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_4_robot_HP;    //��4����������Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_5_robot_HP;    //��5����������Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_7_robot_HP;    //��7�ڱ�������Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_outpost_HP;    //�췽ǰ��սѪ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t red_base_HP;       //�췽����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_1_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_2_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_3_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_4_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_5_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_7_robot_HP;   //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_outpost_HP;    //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
    uint16_t blue_base_HP;      //��1Ӣ�ۻ�����Ѫ����δ�ϳ��Լ�����Ѫ��Ϊ 0
} ext_game_robot_HP_t;

//���ڷ���״̬��0x0004
typedef __packed struct
{
    uint8_t dart_belong;            //������ڵĶ��飺 1���췽����  2����������
    uint16_t stage_remaining_time;  //����ʱ��ʣ�����ʱ�䣬��λ s
} ext_dart_status_t;

//�˹�������ս���ӳ���ͷ���״̬��0x0005
typedef __packed struct
{
    uint8_t F1_zone_status:1;              //bit[0, 4, 8, 12, 16, 20]Ϊ F1-F6 ����״̬�� 0Ϊδ����,1Ϊ�Ѽ���
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
    uint16_t red1_bullet_left;             //�췽1��ʣ�൯��
    uint16_t red2_bullet_left;             //�췽2��ʣ�൯��
    uint16_t blue1_bullet_left;            //����1��ʣ�൯��
    uint16_t blue2_bullet_left;            //����2��ʣ�൯��
} ext_ICRA_buff_debuff_zone_status_t;

//����������״̬��0x0201
typedef __packed struct
{
    uint8_t robot_id;                           //��������ID
    uint8_t robot_level;                        //�����˵ȼ�
    uint16_t remain_HP;                         //������ʣ��Ѫ��
    uint16_t max_HP;                            //����������Ѫ��
    uint16_t shooter_id1_17mm_cooling_rate;     //������1��17mmǹ��ÿ����ȴֵ
    uint16_t shooter_id1_17mm_cooling_limit;    //������1��17mmǹ����������
    uint16_t shooter_id1_17mm_speed_limit;     //������1��17mmǹ�������ٶ�
    uint16_t shooter_id2_17mm_cooling_rate;     //������2��17mmǹ��ÿ����ȴֵ
    uint16_t shooter_id2_17mm_cooling_limit;    //������2��17mmǹ����������
    uint16_t shooter_id2_17mm_speed_limit;      //������2��17mmǹ�������ٶ�
    uint16_t shooter_id1_42mm_cooling_rate;     //������42mmǹ��ÿ����ȴֵ
    uint16_t shooter_id1_42mm_cooling_limit;    //������42mmǹ����������
    uint16_t shooter_id1_42mm_speed_limit;    //������42mmǹ�������ٶ�
    uint16_t chassis_power_limit;             //�����˵��̹�����������
    uint8_t mains_power_gimbal_output : 1;      //0 bit��gimbal������� 1Ϊ��24V�����0Ϊ��24v���
    uint8_t mains_power_chassis_output : 1;     //1 bit��chassis�������1Ϊ��24V�����0Ϊ��24v���
    uint8_t mains_power_shooter_output : 1;     //2 bit��shooter�������1Ϊ��24V�����0Ϊ��24v���
} ext_game_robot_status_t;

//ʵʱ�����������ݣ�0x0202 
typedef __packed struct       
{
 uint16_t chassis_volt;                   //���������ѹ ��λ����
 uint16_t chassis_current;                //����������� ��λ����
 float chassis_power;                     //����������� ��λW
 uint16_t chassis_power_buffer;           //���̹��ʻ��� ��λJ   ��ע�����¸��ݹ���������250J
 uint16_t shooter_id1_17mm_cooling_heat;  //1��17mmǹ������
 uint16_t shooter_id2_17mm_cooling_heat;  //2��17mmǹ������
 uint16_t shooter_id1_42mm_cooling_heat;  //42mmǹ������
} ext_power_heat_data_t;

//���������棺0x0204
typedef __packed struct
{
 uint8_t power_rune_buff;  //bit0��������Ѫ����Ѫ״̬�� 1��ǹ��������ȴ���١�2�������˷����ӳɡ�3�������˹����ӳ�
}ext_buff_t;

//���л���������״̬��0x0205
typedef __packed struct    
{
	uint8_t attack_time;      //�ɹ���ʱ�� ��λs:30s�ݼ���0
} aerial_robot_energy_t;

//�˺�״̬��0x0206
typedef __packed struct
{
	uint8_t armor_id : 4;    //bit 0-3���ܻ�װ��ID:0-4�Ŵ�������˵����װ��Ƭ������Ѫ���仯���ͣ��ñ�����ֵΪ0
	uint8_t hurt_type : 4;   //bit 4-7��Ѫ���仯����:0װ���˺���Ѫ 1ģ����߿�Ѫ 2�����ٿ�Ѫ 3��ǹ��������Ѫ 4�����̹��ʿ�Ѫ 5װ��ײ����Ѫ
} ext_robot_hurt_t;

//ʵʱ�����Ϣ��0x0207
typedef __packed struct
{
  uint8_t bullet_type; //�ӵ�����:1:17mm ���� 2:42mm ����
  uint8_t shooter_id;  //������� ID��1:1��17mm �������  2:2��17mm�������  3:42mm�������
  uint8_t bullet_freq; //�ӵ���Ƶ ��λHz
  float bullet_speed;  //�ӵ����� ��λm/s
} ext_shoot_data_t;

//���ڻ����˿ͻ���ָ�����ݣ�0x020A
typedef __packed struct
{
 uint8_t dart_launch_opening_status;   //��ǰ���ڷ���ڵ�״̬ 0:�ر� 1:���ڿ������߹ر��� 2:�Ѿ�����
 uint8_t dart_attack_target;           //���ڵĴ��Ŀ��  1:ǰ��վ  2:����
 uint16_t target_change_time;          //�л����Ŀ��ʱ�ı���ʣ��ʱ��
 uint8_t first_dart_speed;             //��⵽�ĵ�һö�����ٶ�,��λ0.1m/s/LSB
 uint8_t second_dart_speed;            //��⵽�ĵڶ�ö�����ٶ�,��λ0.1m/s/LSB
 uint8_t third_dart_speed;             //��⵽�ĵ���ö�����ٶ�,��λ0.1m/s/LSB
 uint8_t fourth_dart_speed;            //��⵽�ĵ���ö�����ٶ�,��λ0.1m/s/LSB
 uint16_t last_dart_launch_time;       //���һ�εķ�����ڵı���ʣ��ʱ��,��λ��
 uint16_t operate_launch_cmd_time;     //���һ�β�����ȷ������ָ��ʱ�ı���ʣ��ʱ��
} ext_dart_client_cmd_t;

//�����¼����ݣ�0x0101
typedef __packed struct
{
    uint32_t event_type;     //��ϸ������ϵͳͨѶЭ���ֲ�  P10
} ext_event_data_t;

//����վ������ʶ��0x0102
typedef __packed struct
{
    uint8_t supply_projectile_id;    //����վ��ID��  1��1�Ų�����  2��2�Ų�����
    uint8_t supply_robot_id;         //���������� ID��0Ϊ��ǰ�޻����˲�����1Ϊ�췽Ӣ�ۻ����˲�����2Ϊ�췽���̻����˲�����3/4/5Ϊ�췽���������˲�����101Ϊ����Ӣ�ۻ����˲�����102Ϊ�������̻����˲�����103/104/105Ϊ�������������˲���
    uint8_t supply_projectile_step;  //�����ڿ���״̬��0Ϊ�رգ�1Ϊ�ӵ�׼���У�2Ϊ�ӵ�����
    uint8_t supply_projectile_num;    //��������
} ext_supply_projectile_action_t;

//���о�����Ϣ��0x0104
typedef __packed struct
{
    uint8_t level;         //����ȼ���1������ 2������  3���и�
    uint8_t foul_robot_id; //���������ID���и�ʱ��������IDΪ 0  ���ơ�����ʱ��������IDΪ���������ID
} ext_referee_warning_t;

//���ڷ���ڵ���ʱ��0x0105
typedef __packed struct
{
    uint8_t dart_remaining_time;  //15s����ʱ
} ext_dart_remaining_time_t;

//������λ�ã�0x0203
typedef __packed struct
{
    float x;       //λ��x���꣬��λm
    float y;       //λ��y���꣬��λm
    float z;       //λ��z���꣬��λm
    float yaw;     //λ��ǹ�ڣ���λ��
} ext_game_robot_pos_t;

typedef __packed struct//�ӵ�ʣ�෢������0x0208
{
    uint16_t bullet_remaining_num_17mm;   //17mm �ӵ�ʣ�෢����Ŀ
    uint16_t bullet_remaining_num_42mm;   //42mm �ӵ�ʣ�෢����Ŀ
    uint16_t coin_remaining_num;          //ʣ��������
} ext_bullet_remaining_t;

//������ RFID ״̬��0x0209
typedef __packed struct
{
    uint32_t rfid_status;    //��ϸ������ϵͳͨѶЭ���ֲ� P17
} ext_rfid_status_t;

extern uint8_t referee_system_data[60],referee_system_num;  //����ϵͳ������������ݳ���

extern void REFEREE_SYSTEM_ANALYSIS(void);

#endif


