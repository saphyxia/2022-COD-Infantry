#include "referee.h"
#include "Irqhandler.h"
#include  "string.h"

uint8_t referee_system_data[60],referee_system_num=0;  //裁判系统接收数组和数据长度

tReferee_System_Data  Referee_System_Data;//裁判系统数据
ext_game_status_t referee_game_status; //比赛状态
ext_game_result_t referee_game_result; //比赛结果
ext_game_robot_HP_t referee_game_robot_HP; //机器人血量
ext_dart_status_t referee_game_dart_status; //飞镖发射状态
ext_ICRA_buff_debuff_zone_status_t referee_game_buff_debuff_zone; //加成与惩罚区状态
ext_game_robot_status_t referee_robot_status; //机器人状态
ext_power_heat_data_t referee_robot_power_heat; //实时功率热量
ext_buff_t referee_game_buff; //机器人增益
aerial_robot_energy_t referee_aerial_robot_energy; //空中机器人能量状态
ext_robot_hurt_t referee_robot_hurt;
ext_shoot_data_t referee_robot_shoot; //实时射击信息
ext_dart_client_cmd_t referee_dart_client_cmd; //飞镖机器人客户端指令

/**
  * @name   REFEREE_SYSTEM_ANALYSIS
  * @brief  裁判系统解析函数
  * @param  none
  * @retval none
  * @attention none
*/
void REFEREE_SYSTEM_ANALYSIS(void)
{
	if(referee_system_data[0]==0xA5)
	{
        Referee_System_Data.SOF     = referee_system_data[0];
        Referee_System_Data.Length  = referee_system_data[1] |referee_system_data[2]<<8;
        Referee_System_Data.Seq     = referee_system_data[3];	
        Referee_System_Data.Info_ID = referee_system_data[5] |referee_system_data[6]<<8;			
				
        switch(Referee_System_Data.Info_ID)
        {
					
            case 0x0001:			
                if(referee_system_num>=20)
                {
                    referee_system_num=0;
                    referee_game_status.game_type=referee_system_data[7]%16;
                    referee_game_status.game_progress=referee_system_data[7]/16;
                    referee_game_status.stage_remain_time=referee_system_data[8]|referee_system_data[9]<<8;
                } 
            break;

			case 0x0002:
							
                if(referee_system_num>=10)
                {
                    referee_system_num=0;
                    referee_game_result.winner=referee_system_data[7];
                } 
            break;
											
            case 0x0003:		
                if(referee_system_num>=41)
                {
                    referee_system_num=0;
                    referee_game_robot_HP.red_1_robot_HP=referee_system_data[7] |referee_system_data[8]<<8;
                    referee_game_robot_HP.red_2_robot_HP=referee_system_data[9] |referee_system_data[10]<<8;
                    referee_game_robot_HP.red_3_robot_HP=referee_system_data[11] |referee_system_data[12]<<8;
                    referee_game_robot_HP.red_4_robot_HP=referee_system_data[13] |referee_system_data[14]<<8;
                    referee_game_robot_HP.red_5_robot_HP=referee_system_data[15] |referee_system_data[16]<<8;
                    referee_game_robot_HP.red_7_robot_HP=referee_system_data[17] |referee_system_data[18]<<8;
                    referee_game_robot_HP.red_outpost_HP=referee_system_data[19] |referee_system_data[20];
                    referee_game_robot_HP.red_base_HP=referee_system_data[21] |referee_system_data[22]<<8;
                    referee_game_robot_HP.blue_1_robot_HP=referee_system_data[23] |referee_system_data[24]<<8;
                    referee_game_robot_HP.blue_2_robot_HP=referee_system_data[25] |referee_system_data[26]<<8;
                    referee_game_robot_HP.blue_3_robot_HP=referee_system_data[27] |referee_system_data[28]<<8;
                    referee_game_robot_HP.blue_4_robot_HP=referee_system_data[29] |referee_system_data[30]<<8;
                    referee_game_robot_HP.blue_5_robot_HP=referee_system_data[31] |referee_system_data[32]<<8;
                    referee_game_robot_HP.blue_7_robot_HP=referee_system_data[33] |referee_system_data[34]<<8;
                    referee_game_robot_HP.blue_outpost_HP=referee_system_data[35] |referee_system_data[36]<<8;
                    referee_game_robot_HP.blue_base_HP=referee_system_data[37] |referee_system_data[38]<<8;
                } 
            break;
											
            case 0x0004:			
                if(referee_system_num>=12)
                {
                    referee_system_num=0;
                    referee_game_dart_status.dart_belong=referee_system_data[7];
                    referee_game_dart_status.stage_remaining_time=referee_system_data[8] |referee_system_data[9]<<8;
                } 
            break;
											
            case 0x0005:
						
                if(referee_system_num>=20)
                {
                    referee_system_num=0;
                    referee_game_buff_debuff_zone.F1_zone_status = referee_system_data[7]%2;
                    referee_game_buff_debuff_zone.F2_zone_status = (referee_system_data[7]>>4)%2;
                    referee_game_buff_debuff_zone.F3_zone_status = referee_system_data[8]%2;
                    referee_game_buff_debuff_zone.F4_zone_status = (referee_system_data[8]>>4)%2;
                    referee_game_buff_debuff_zone.F5_zone_status = referee_system_data[9]%2;
                    referee_game_buff_debuff_zone.F6_zone_status = (referee_system_data[9]>>4)%2;
                    referee_game_buff_debuff_zone.F1_zone_buff_debuff_status = (referee_system_data[7]%16)>>1;
                    referee_game_buff_debuff_zone.F2_zone_buff_debuff_status = (referee_system_data[7]/16)>>1;
                    referee_game_buff_debuff_zone.F3_zone_buff_debuff_status = (referee_system_data[8]%16)>>1;
                    referee_game_buff_debuff_zone.F4_zone_buff_debuff_status = (referee_system_data[8]/16)>>1;
                    referee_game_buff_debuff_zone.F5_zone_buff_debuff_status = (referee_system_data[9]%16)>>1;
                    referee_game_buff_debuff_zone.F6_zone_buff_debuff_status = (referee_system_data[9]/16)>>1;
                    referee_game_buff_debuff_zone.red1_bullet_left=referee_system_data[10] | referee_system_data[11]<<8;
                    referee_game_buff_debuff_zone.red2_bullet_left=referee_system_data[12] | referee_system_data[13]<<8;
                    referee_game_buff_debuff_zone.blue1_bullet_left=referee_system_data[14] | referee_system_data[15]<<8;
                    referee_game_buff_debuff_zone.blue2_bullet_left=referee_system_data[16] | referee_system_data[17]<<8;
                }  
            break;
						
            case 0x0201:
                if(referee_system_num>=36)
                {
                    referee_system_num=0;
                    referee_robot_status.robot_id=referee_system_data[7];
                    referee_robot_status.robot_level=referee_system_data[8];
                    referee_robot_status.remain_HP=(referee_system_data[10]<<8)|referee_system_data[9]  ;
                    referee_robot_status.max_HP=(referee_system_data[12]<<8 )| referee_system_data[11];
                    referee_robot_status.shooter_id1_17mm_cooling_rate=(referee_system_data[14]<<8 )| referee_system_data[13];
                    referee_robot_status.shooter_id1_17mm_cooling_limit=(referee_system_data[16]<<8 )| referee_system_data[15];
                    referee_robot_status.shooter_id1_17mm_speed_limit=referee_system_data[17];
                    referee_robot_status.shooter_id2_17mm_cooling_rate=(referee_system_data[20]<<8 )| referee_system_data[19];
                    referee_robot_status.shooter_id2_17mm_cooling_limit=(referee_system_data[22]<<8 )| referee_system_data[21];
                    referee_robot_status.shooter_id2_17mm_speed_limit=(referee_system_data[24]<<8 )| referee_system_data[23];
                    referee_robot_status.shooter_id1_42mm_cooling_rate=(referee_system_data[26]<<8 )| referee_system_data[25];
                    referee_robot_status.shooter_id1_42mm_cooling_limit=(referee_system_data[28]<<8)| referee_system_data[27];
                    referee_robot_status.shooter_id1_42mm_speed_limit=referee_system_data[30];										
                    referee_robot_status.chassis_power_limit=referee_system_data[32] | referee_system_data[31]; 	
                    referee_robot_status.mains_power_gimbal_output=referee_system_data[33]&0x01;		
                    referee_robot_status.mains_power_chassis_output=(referee_system_data[33]>>1)&0x01;
                    referee_robot_status.mains_power_shooter_output=(referee_system_data[33]>>2)&0x01;
                } 
			break;
									
			case 0x0202:		
				if(referee_system_num>=23)
                {
                    referee_system_num=0;
                    referee_robot_power_heat.chassis_volt=referee_system_data[7] |referee_system_data[8]<<8;
                    referee_robot_power_heat.chassis_current=referee_system_data[9] |referee_system_data[10]<<8;
//					referee_robot_power_heat.chassis_power=(referee_system_data[11] +referee_system_data[12])/100.0f+(referee_system_data[13] +referee_system_data[14]);
                    referee_robot_power_heat.chassis_power=referee_robot_power_heat.chassis_volt*referee_robot_power_heat.chassis_current/1000000.0f;
                    referee_robot_power_heat.chassis_power_buffer=referee_system_data[15] |referee_system_data[16]<<8;
//					referee_robot_power_heat.shooter_id1_42mm_cooling_heat=referee_system_data[17] |referee_system_data[18]<<8;
                    referee_robot_power_heat.shooter_id1_17mm_cooling_heat=referee_system_data[17] |referee_system_data[18]<<8;										
                    referee_robot_power_heat.shooter_id2_17mm_cooling_heat=referee_system_data[19] |referee_system_data[20]<<8;
                } 
			break;
															
			case 0x0204:
				if(referee_system_num>=10)
                {
                    referee_system_num=0;
                    referee_game_buff.power_rune_buff=referee_system_data[7];
				} 
			break;
										
			case 0x0205:
                if(referee_system_num>=12)
                {
                    referee_system_num=0;
                    referee_aerial_robot_energy.attack_time=referee_system_data[7] |referee_system_data[8]<<8;
                } 
			break;
														
			case 0x0206:
                if(referee_system_num>=9)
                {
                    referee_system_num=0;
                    referee_robot_hurt.armor_id=(referee_system_data[7]<<4)>>4;
                    referee_robot_hurt.hurt_type=referee_system_data[7]>>4;
                } 
			break;
														
			case 0x0207:	
			if(referee_system_num>=14)
            {
                referee_system_num=0;
//				referee_robot_shoot.bullet_type=referee_system_data[7];
//				referee_robot_shoot.shooter_id=referee_system_data[8];
//				referee_robot_shoot.bullet_freq=referee_system_data[9];
                memcpy(&referee_robot_shoot,&referee_system_data[7],7);
            } 
			break;
																												
            case 0x020A:
                if(referee_system_num>=13)
                {
                    referee_system_num=0;
                    referee_dart_client_cmd.dart_launch_opening_status=referee_system_data[7];
                    referee_dart_client_cmd.dart_attack_target=referee_system_data[8];
                    referee_dart_client_cmd.target_change_time=referee_system_data[9] |referee_system_data[10]<<8;
                    referee_dart_client_cmd.operate_launch_cmd_time=referee_system_data[11] |referee_system_data[12]<<8;
                } 
            break;
                                                                                        
            default :
                referee_system_num=0;
            break;		
        }
	}
}



