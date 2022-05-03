#include "UI.h"
#include "usart.h"
#include "referee_info.h"
#include "usart.h"
#include "chassis.h"

#define Robot_receiver_ID 0x168

//		Blue_receiver_ID_client_hero 		 	= 0x0165;
//		Blue_receiver_ID_client_engineer  = 0x0166;
//		Blue_receiver_ID_client_infantry3 = 0x0167;
//		Blue_receiver_ID_client_infantry4 = 0x0168;
//		Blue_receiver_ID_client_infantry5 = 0x0169;
//		Blue_receiver_ID_client_plane			= 0x016A;

//		Red_receiver_ID_client_hero   = 0x0101;
//		Red_receiver_client_engineer  = 0x0102;
//		Red_receiver_client_infantry3 = 0x0103;
//		Red_receiver_client_infantry4 = 0x0104;
//		Red_receiver_client_infantry5 = 0x0105;
//		Red_receiver_client_plane			= 0x0106;

uint32_t division_value = 10;

UI_Info_t UI;
int update_figure_flag,update_aim_flag,update_float_flag,update_supercapacitor_flag,update_int_flag;
bool global_fri,global_spin,global_auto_aim,global_err;//圈圈

float global_supercapacitor_remain = 77.3;//[0,100]

uint32_t global_sight_bead_x = 960,global_sight_bead_y = 720,global_supercapacitor_point=20;//[0,100]

/**
  * @brief  串口一次发送一个字节数据
  * @param  自己打包好的要发给裁判的数据
  * @retval void
  * @attention  串口移位发送
  */
void UART8_SendChar(uint8_t cData)
{
	while (USART_GetFlagStatus( UART8, USART_FLAG_TC ) == RESET);
	
	USART_SendData( UART8, cData );   
}

ext_charstring_data_t tx_client_char;
uint8_t CliendTxBuffer[200];
uint8_t state_first_graphic = 0 ;//0~7循环
char first_line[30]  = {" FRI:"};//是否可以射击,最多放30个字符串，bool
char second_line[30] = {"SPIN:"};//小陀螺
char third_line[30]  = {" ERR:"};//自瞄
char fourth_line[30] = {"WARNING!"};//异常
char fifth_line[30] = {"STANDARD-4"};//启动提示
char empty_line[30] = {"                              "};

void Char_Graphic(ext_client_string_t* graphic,//最终要发出去的数组中的数据段内容
									const char* name,
									uint32_t operate_tpye,
									
									uint32_t layer,
									uint32_t color,
									uint32_t size,
									uint32_t length,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,

									
									
									const char *character)//外部放入的数组
{
	graphic_data_struct_t *data_struct = &graphic->grapic_data_struct;
	for(char i=0;i<3;i++)
		data_struct->graphic_name[i] = name[i];	//字符索引
	data_struct->operate_tpye = operate_tpye; //图层操作
	data_struct->graphic_tpye = CHAR;         //Char型
	data_struct->layer = layer;//都在第零层
	data_struct->color = color;//都是白色
	data_struct->start_angle = size;
	data_struct->end_angle = length;	
	data_struct->width = width;
	data_struct->start_x = start_x;
	data_struct->start_y = start_y;	
	
	data_struct->radius = 0;
	data_struct->end_x = 0;
	data_struct->end_y = 0;
	memcpy(graphic->data,empty_line,19);
  memcpy(graphic->data,character,length);
}

static void start_UI_char(void)
{
	tx_client_char.txFrameHeader.SOF = 0xA5;
	tx_client_char.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_string_t);
	tx_client_char.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer,&tx_client_char.txFrameHeader,sizeof(xFrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

	//命令码
	tx_client_char.CmdID = 0x0301;
	
	//数据段头结构
	tx_client_char.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_char_graphic;
	tx_client_char.dataFrameHeader.send_ID     = game_robot_state.robot_id;
	tx_client_char.dataFrameHeader.receiver_ID = Robot_receiver_ID;

	Char_Graphic(&tx_client_char.clientData,"CL5",ADD,0,WHITE,20,strlen(fifth_line),4,(960-100),400,fifth_line);

	memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&tx_client_char.CmdID, LEN_CMD_ID+tx_client_char.txFrameHeader.DataLength);//加上命令码长度2

	//帧尾
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(tx_client_char));

	for(int i = 0; i < sizeof(tx_client_char); i++) {
	UART8_SendChar(CliendTxBuffer[i]);
	}
}

static void Draw_char()
{
	if(state_first_graphic == 0)//不知道什么时候进入客户端所以要不断更新
	{
		Char_Graphic(&tx_client_char.clientData,"CL1",ADD,0,RED_BLUE,20,strlen(first_line),2,(50),(1080*9/12),first_line);//x1920/18
		state_first_graphic = 1;
	}
	else	if(state_first_graphic == 1)//不知道什么时候进入客户端所以要不断更新
	{
		Char_Graphic(&tx_client_char.clientData,"CL2",ADD,0,RED_BLUE,20,strlen(second_line),2,(50),(1080*8/12),second_line);//x1920/18
		state_first_graphic = 2;
	}
	else	if(state_first_graphic == 2)//不知道什么时候进入客户端所以要不断更新
	{
		Char_Graphic(&tx_client_char.clientData,"CL3",ADD,0,RED_BLUE,20,strlen(third_line),2,(50),(1080*7/12),third_line);//x1920/18
		state_first_graphic = 3;
	}
	else	if(state_first_graphic == 3)//不知道什么时候进入客户端所以要不断更新
	{
		Char_Graphic(&tx_client_char.clientData,"CL4",ADD,0,RED_BLUE,40,strlen(fourth_line),4,(1920-500),(1080*9/12),fourth_line);//x1920/18
		state_first_graphic = 4;
	}
}
void Client_graphic_Init(void)
{
	if(state_first_graphic>=4)
	{
		state_first_graphic = 0;
	}
		//帧头
		tx_client_char.txFrameHeader.SOF = 0xA5;
		tx_client_char.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(ext_client_string_t);
		tx_client_char.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&tx_client_char.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验
	
		//命令码
		tx_client_char.CmdID = 0x0301;
		
		//数据段头结构
		tx_client_char.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_char_graphic;
		tx_client_char.dataFrameHeader.send_ID     = game_robot_state.robot_id;
		tx_client_char.dataFrameHeader.receiver_ID = Robot_receiver_ID;

		
		//数据段
		Draw_char();
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&tx_client_char.CmdID, LEN_CMD_ID+tx_client_char.txFrameHeader.DataLength);//加上命令码长度2
		
		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(tx_client_char));
		
    for(int i = 0; i < sizeof(tx_client_char); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
void UI_Init(void)
{
	static bool IF_Init_Over = false;
	
  if(!IF_Init_Over)
  {
    
    update_figure_flag = ADD;
    update_aim_flag = ADD;
    update_float_flag = ADD;
    update_supercapacitor_flag = ADD;
    
    update_int_flag = ADD;
    
    IF_Init_Over = true;

  }
}
void UI_DataUpdate(void)
{
		UI.User.SPIN =             (uint8_t)(Chassis.act==SPIN);

		UI.User.Vcap_show =        Cap_V;
}
void Figure_Graphic(graphic_data_struct_t* graphic,//最终要发出去的数组的数据段内容
									const char* name,
									uint32_t operate_tpye,
									uint32_t graphic_tpye,//绘制什么图像
									uint32_t layer,
									uint32_t color,
									uint32_t start_angle,
									uint32_t end_angle,
									uint32_t width,
									uint32_t start_x,
									uint32_t start_y,
									uint32_t radius,
									uint32_t end_x,
									uint32_t end_y)							
{
	for(char i=0;i<3;i++)
		graphic->graphic_name[i] = name[i];	//字符索引
	graphic->operate_tpye = operate_tpye; //图层操作
	graphic->graphic_tpye = graphic_tpye;         //Char型
	graphic->layer        = layer;//都在第一层
	graphic->color        = color;//变色
	graphic->start_angle  = start_angle;
	graphic->end_angle    = end_angle;	
	graphic->width        = width;
	graphic->start_x      = start_x;
	graphic->start_y      = start_y;	
	graphic->radius = radius;
	graphic->end_x  = end_x;
	graphic->end_y  = end_y;
}
//************************************绘制象形*******************************/
ext_graphic_seven_data_t tx_client_graphic_figure;
static void fri_figure(bool clip)//摩擦轮打开为true
{
	if( clip == true)//可准备射击为绿色
		Figure_Graphic(&tx_client_graphic_figure.clientData[0],"GL1",update_figure_flag,CIRCLE,1,GREEN,0,0,5, 180,1080*9/12-10, 20,0,0);
	else if( clip == false)//摩擦轮关闭
		Figure_Graphic(&tx_client_graphic_figure.clientData[0],"GL1",update_figure_flag,CIRCLE,1,FUCHSIA,0,0,5, 180,1080*9/12-10, 20,0,0);
}
static void spin_second_figure(bool spin)//小陀螺打开为true
{
	if(spin == true)//打开小陀螺为绿色
		Figure_Graphic(&tx_client_graphic_figure.clientData[1],"GL2",update_figure_flag,CIRCLE,1,GREEN,0,0,5,  180,1080*8/12-10, 20,0,0);
	else if(spin == false)//没开小陀螺为紫红色
		Figure_Graphic(&tx_client_graphic_figure.clientData[1],"GL2",update_figure_flag,CIRCLE,1,FUCHSIA,0,0,5, 180,1080*8/12-10, 20,0,0);
}
static void err_figure(bool err)//异常
{
	if(err == true)//无异常为绿色
		Figure_Graphic(&tx_client_graphic_figure.clientData[2],"GL3",update_figure_flag,CIRCLE,1,GREEN,0,0,5,  180,1080*7/12-10, 20,0,0);
	else if(err == false)//异常为紫红色
		Figure_Graphic(&tx_client_graphic_figure.clientData[2],"GL3",update_figure_flag,CIRCLE,1,FUCHSIA,0,0,5, 180,1080*7/12-10, 20,0,0);
}
static void auto_aim_third_figure(bool auto_aim)//自瞄打开为true
{
	if(auto_aim == true)
		Figure_Graphic(&tx_client_graphic_figure.clientData[3],"GL4",update_figure_flag,RECTANGLE,1,GREEN,0,0,3,       640,400, 0,1280,810);
	else if(auto_aim == false)
		Figure_Graphic(&tx_client_graphic_figure.clientData[3],"GL4",DELETE,RECTANGLE,1,FUCHSIA,0,0,3,  (1920-580),610, 0,1920-450,540-70);
}
int start_x,start_y,end_x,end_y;
static void driveway_figure(void)//车道
{
		Figure_Graphic(&tx_client_graphic_figure.clientData[4],"GL5",ADD,LINE,1,GREEN,0,0,3,       540,0, 0,640,100);
		Figure_Graphic(&tx_client_graphic_figure.clientData[5],"GL6",ADD,LINE,1,GREEN,0,0,3,       1380,0, 0,1280,100);
}
static void EMPTY(bool a)
{
  Figure_Graphic(&tx_client_graphic_figure.clientData[6],"GL7",DELETE,FUCHSIA,1,GREEN,0,0,5,  (1920-350),1080*9/12, 20,0,0);
}
static void Draw_Figure_bool()
{
	fri_figure(global_fri);
	spin_second_figure(global_spin);
	err_figure(global_err);
	auto_aim_third_figure (global_auto_aim);
	driveway_figure();
  EMPTY(0);
}
void Client_graphic_Info_update()//七个图像一起更新
{
		//帧头
		tx_client_graphic_figure.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		tx_client_graphic_figure.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
		tx_client_graphic_figure.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&tx_client_graphic_figure.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		tx_client_graphic_figure.CmdID = 0x0301;

		//数据段头结构
		tx_client_graphic_figure.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
		tx_client_graphic_figure.dataFrameHeader.send_ID     = game_robot_state.robot_id;
		tx_client_graphic_figure.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		Draw_Figure_bool();
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&tx_client_graphic_figure.CmdID, LEN_CMD_ID+tx_client_graphic_figure.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(tx_client_graphic_figure));
		
    for(int i = 0; i < sizeof(tx_client_graphic_figure); i++) 
		{
      UART8_SendChar(CliendTxBuffer[i]);
    }

}
ext_graphic_seven_data_t high_aim_figure;//操作手准心之上,不更新
ext_graphic_seven_data_t low_aim_shortfigure_1;//准心下的第一个短线
ext_graphic_seven_data_t low_aim_shortfigure_2;
ext_graphic_seven_data_t  low_aim_shortfigure_3;//五个短线两个纵线
ext_graphic_seven_data_t  start_cod_lineconfig_1;//启动Ui 13根线
ext_graphic_seven_data_t  start_cod_lineconfig_2;
ext_graphic_seven_data_t  start_cod_lineconfig_3;
ext_graphic_five_data_t  low_aim_longfigure;//准心下的长横线

static void start_UI_line_1(void)
{
	//帧头
	start_cod_lineconfig_1.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
	start_cod_lineconfig_1.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
	start_cod_lineconfig_1.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer,&start_cod_lineconfig_1.txFrameHeader,sizeof(xFrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

	//命令码
	start_cod_lineconfig_1.CmdID = 0x0301;

	//数据段头结构
	start_cod_lineconfig_1.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
	start_cod_lineconfig_1.dataFrameHeader.send_ID     = game_robot_state.robot_id;
	start_cod_lineconfig_1.dataFrameHeader.receiver_ID = Robot_receiver_ID;

	Figure_Graphic(&start_cod_lineconfig_1.clientData[0],"AO1",ADD,LINE,4,YELLOW,0,0,4,   904,478, 0, 875,495);//graphic_Remove
	Figure_Graphic(&start_cod_lineconfig_1.clientData[1],"AO2",ADD,LINE,4,YELLOW,0,0,4,   875,495, 0, 875,585);
	Figure_Graphic(&start_cod_lineconfig_1.clientData[2],"AO3",ADD,LINE,4,YELLOW,0,0,4,   875,585, 0, 904,602);
	Figure_Graphic(&start_cod_lineconfig_1.clientData[3],"AO4",ADD,LINE,4,YELLOW,0,0,4,   960,445, 0, 933,462);
	Figure_Graphic(&start_cod_lineconfig_1.clientData[4],"AO5",ADD,LINE,4,YELLOW,0,0,4,   933,462, 0, 933,618);
	Figure_Graphic(&start_cod_lineconfig_1.clientData[5],"AO6",ADD,LINE,4,YELLOW,0,0,4,   933,618, 0, 960, 635);
	Figure_Graphic(&start_cod_lineconfig_1.clientData[6],"AO7",ADD,LINE,4,YELLOW,0,0,4,   960,635, 0, 987, 618);

	memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&start_cod_lineconfig_1.CmdID, LEN_CMD_ID+start_cod_lineconfig_1.txFrameHeader.DataLength);//加上命令码长度2

	//帧尾
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(start_cod_lineconfig_1));

	for(int i = 0; i < sizeof(start_cod_lineconfig_1); i++) {
	UART8_SendChar(CliendTxBuffer[i]);
	}
}
static void start_UI_line_2(void)
{
	//帧头
	start_cod_lineconfig_2.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
	start_cod_lineconfig_2.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
	start_cod_lineconfig_2.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer,&start_cod_lineconfig_2.txFrameHeader,sizeof(xFrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

	//命令码
	start_cod_lineconfig_2.CmdID = 0x0301;

	//数据段头结构
	start_cod_lineconfig_2.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
	start_cod_lineconfig_2.dataFrameHeader.send_ID     = game_robot_state.robot_id;
	start_cod_lineconfig_2.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
	Figure_Graphic(&start_cod_lineconfig_2.clientData[0],"AN1",ADD,LINE,4,YELLOW,0,0,4,   987,618, 0, 987,462);//graphic_Remove
	Figure_Graphic(&start_cod_lineconfig_2.clientData[1],"AN2",ADD,LINE,4,YELLOW,0,0,4,   987,462, 0, 960,445);
	Figure_Graphic(&start_cod_lineconfig_2.clientData[2],"AN3",ADD,LINE,4,YELLOW,0,0,4,   1016,478, 0, 1016,602);
	Figure_Graphic(&start_cod_lineconfig_2.clientData[3],"AN4",ADD,LINE,4,YELLOW,0,0,4,   1016,602, 0, 1045,585);
	Figure_Graphic(&start_cod_lineconfig_2.clientData[4],"AN5",ADD,LINE,4,YELLOW,0,0,4,   1045,585, 0, 1045,495);
	Figure_Graphic(&start_cod_lineconfig_2.clientData[5],"AN6",ADD,LINE,4,YELLOW,0,0,4,   1045,495, 0, 1016,478);

	memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&start_cod_lineconfig_2.CmdID, LEN_CMD_ID+start_cod_lineconfig_2.txFrameHeader.DataLength);//加上命令码长度2

	//帧尾
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(start_cod_lineconfig_2));

	for(int i = 0; i < sizeof(start_cod_lineconfig_2); i++) {
	UART8_SendChar(CliendTxBuffer[i]);
	}
}
static void start_UI_line_3(void)
{
	//帧头
	start_cod_lineconfig_3.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
	start_cod_lineconfig_3.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
	start_cod_lineconfig_3.txFrameHeader.Seq = 0;//包序号
	memcpy(CliendTxBuffer,&start_cod_lineconfig_3.txFrameHeader,sizeof(xFrameHeader));
	Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

	//命令码
	start_cod_lineconfig_3.CmdID = 0x0301;

	//数据段头结构
	start_cod_lineconfig_3.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
	start_cod_lineconfig_3.dataFrameHeader.send_ID     = game_robot_state.robot_id;
	start_cod_lineconfig_3.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
	Figure_Graphic(&start_cod_lineconfig_3.clientData[0],"AP1",ADD,LINE,4,BLACK,0,0,4,   960,435, 0, 868,490);//graphic_Remove
	Figure_Graphic(&start_cod_lineconfig_3.clientData[1],"AP2",ADD,LINE,4,BLACK,0,0,4,   868,490, 0, 868,590);
	Figure_Graphic(&start_cod_lineconfig_3.clientData[2],"AP3",ADD,LINE,4,BLACK,0,0,4,   868,590, 0, 960,645);
	Figure_Graphic(&start_cod_lineconfig_3.clientData[3],"AP4",ADD,LINE,4,BLACK,0,0,4,   960,645, 0, 1052,590);
	Figure_Graphic(&start_cod_lineconfig_3.clientData[4],"AP5",ADD,LINE,4,BLACK,0,0,4,   1052,590, 0, 1052,490);
	Figure_Graphic(&start_cod_lineconfig_3.clientData[5],"AP6",ADD,LINE,4,BLACK,0,0,4,   1052,490, 0, 960,435);

	memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&start_cod_lineconfig_3.CmdID, LEN_CMD_ID+start_cod_lineconfig_3.txFrameHeader.DataLength);//加上命令码长度2

	//帧尾
	Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(start_cod_lineconfig_3));

	for(int i = 0; i < sizeof(start_cod_lineconfig_3); i++) {
	UART8_SendChar(CliendTxBuffer[i]);
	}
}
//stem--茎
static void aim_lowshort_stem(uint32_t division_value,uint32_t line_length)//准心上半部分的宽度"AM"--aim_low_bottom,"AS"--aim_stem
{ 
	Figure_Graphic(&low_aim_shortfigure_3.clientData[0],"AB1",ADD,LINE,3,YELLOW,0,0,1,   960-line_length,540-30-division_value*18,0,   960+line_length,540-30-division_value*18);//graphic_Remove
	Figure_Graphic(&low_aim_shortfigure_3.clientData[1],"AB2",ADD,LINE,3,YELLOW,0,0,1,   960-line_length,540-30-division_value*20,0,   960+line_length,540-30-division_value*20);
	Figure_Graphic(&low_aim_shortfigure_3.clientData[2],"AB3",ADD,LINE,3,YELLOW,0,0,1,   960-line_length,540-30-division_value*21,0,   960+line_length,540-30-division_value*21);
	Figure_Graphic(&low_aim_shortfigure_3.clientData[3],"AB4",ADD,LINE,3,YELLOW,0,0,1,   960-line_length,540-30-division_value*22,0,   960+line_length,540-30-division_value*22);
	Figure_Graphic(&low_aim_shortfigure_3.clientData[4],"AB5",ADD,LINE,3,YELLOW,0,0,1,960-line_length-10,540-30-division_value*23,0,960+line_length+10,540-30-division_value*23);
	
	Figure_Graphic(&low_aim_shortfigure_3.clientData[5],"AS1",ADD,LINE,3,YELLOW,0,0,1,   960,            540+30+division_value*6 ,0,   960,            540+30);
	Figure_Graphic(&low_aim_shortfigure_3.clientData[6],"AS2",ADD,LINE,3,YELLOW,0,0,1,   960,            540-30-division_value*23,0,   960,            540-30);

}
void _lowshortstem_aim_4()
{
		//帧头
		low_aim_shortfigure_3.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		low_aim_shortfigure_3.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
		low_aim_shortfigure_3.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&low_aim_shortfigure_3.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		low_aim_shortfigure_3.CmdID = 0x0301;

		//数据段头结构
		low_aim_shortfigure_3.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
		low_aim_shortfigure_3.dataFrameHeader.send_ID     = game_robot_state.robot_id;
		low_aim_shortfigure_3.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		aim_lowshort_stem(division_value,10);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&low_aim_shortfigure_3.CmdID, LEN_CMD_ID+low_aim_shortfigure_3.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(low_aim_shortfigure_3));
		
    for(int i = 0; i < sizeof(low_aim_shortfigure_3); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
//图层四
static void aim_lowlong(uint32_t division_value,uint32_t line_length)//准心上半部分的宽度"AM"--aim_low_Long,"AS"--aim_stem
{ 
	Figure_Graphic(&low_aim_longfigure.clientData[0],"AL1",ADD,LINE,4,YELLOW,0,0,1,960-line_length-30,540-30-division_value*19,0,960+line_length+30,540-30-division_value*19);//graphic_Remove
	Figure_Graphic(&low_aim_longfigure.clientData[1],"AL2",ADD,LINE,4,YELLOW,0,0,1,960-line_length-40,540-30-division_value*15,0,960+line_length+40,540-30-division_value*15);
	Figure_Graphic(&low_aim_longfigure.clientData[2],"AL3",ADD,LINE,4,YELLOW,0,0,1,960-line_length-50,540-30-division_value*11,0,960+line_length+50,540-30-division_value*11);
	Figure_Graphic(&low_aim_longfigure.clientData[3],"AL4",ADD,LINE,4,YELLOW,0,0,1,960-line_length-60,540-30-division_value*7 ,0,960+line_length+60,540-30-division_value*7 );
	Figure_Graphic(&low_aim_longfigure.clientData[4],"AL5",ADD,LINE,4,YELLOW,0,0,1,960-line_length-70,540-30-division_value*3 ,0,960+line_length+70,540-30-division_value*3 );
	
}
void _lowlong_aim_()
{
		//帧头
		low_aim_longfigure.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		low_aim_longfigure.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*5;
		low_aim_longfigure.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&low_aim_longfigure.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		low_aim_longfigure.CmdID = 0x301;

		//数据段头结构
		low_aim_longfigure.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_five_graphic;
		low_aim_longfigure.dataFrameHeader.send_ID     = game_robot_state.robot_id;
		low_aim_longfigure.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		aim_lowlong(division_value,10);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&low_aim_longfigure.CmdID, LEN_CMD_ID+low_aim_longfigure.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(low_aim_longfigure));
		
    for(int i = 0; i < sizeof(low_aim_longfigure); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
//剩余电容只有一个图层
ext_graphic_one_data_t tx_supercapacitor_figure;
int update_supercapacitor_flag;
static void supercapacitor_figure(float remain_energy,uint32_t turning_point)//剩余超级电容（单位百分比），低于某百分比变红色
{
	uint32_t remaining = (uint32_t)remain_energy;//强制转换
	if(remaining >= turning_point)//直线长度为3
		Figure_Graphic(&tx_supercapacitor_figure.clientData,"SR1",update_supercapacitor_flag,LINE,2,GREEN,0,0,20,(1920-100),(1080-700)  ,0, (1920-100),(1080-700)+remaining*3);
	else if(remaining < turning_point)
		Figure_Graphic(&tx_supercapacitor_figure.clientData,"SR1",update_supercapacitor_flag,LINE,2,FUCHSIA,0,0,20,(1920-100),(1080-700)  ,0,  (1920-100),(1080-700)+remaining*3);		
}
void Client_supercapacitor_update()//一个图像更新
{
		//帧头
		tx_supercapacitor_figure.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		tx_supercapacitor_figure.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t);
		tx_supercapacitor_figure.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&tx_supercapacitor_figure.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		tx_supercapacitor_figure.CmdID = 0x301;

		//数据段头结构
		tx_supercapacitor_figure.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_one_graphic;
		tx_supercapacitor_figure.dataFrameHeader.send_ID     = game_robot_state.robot_id;;
		tx_supercapacitor_figure.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		supercapacitor_figure(global_supercapacitor_remain,global_supercapacitor_point);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&tx_supercapacitor_figure.CmdID, LEN_CMD_ID+tx_supercapacitor_figure.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(tx_supercapacitor_figure));
		
    for(int i = 0; i < sizeof(tx_supercapacitor_figure); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
//division_value分度值,line_length短线长度的一半
static void aim_1(uint32_t division_value,uint32_t line_length)//准心上半部分的宽度"AH"--aim_high
{
	Figure_Graphic(&high_aim_figure.clientData[0],"AH1",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30                 ,0,  960+line_length,540+30);//graphic_Remove
	Figure_Graphic(&high_aim_figure.clientData[1],"AH2",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value  ,0,  960+line_length,540+30+division_value  );
	Figure_Graphic(&high_aim_figure.clientData[2],"AH3",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value*2,0,  960+line_length,540+30+division_value*2);
	Figure_Graphic(&high_aim_figure.clientData[3],"AH4",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value*3,0,  960+line_length,540+30+division_value*3);
	Figure_Graphic(&high_aim_figure.clientData[4],"AH5",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value*4,0,  960+line_length,540+30+division_value*4);
	Figure_Graphic(&high_aim_figure.clientData[5],"AH6",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value*5,0,  960+line_length,540+30+division_value*5);
	Figure_Graphic(&high_aim_figure.clientData[6],"AH7",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540+30+division_value*6,0,  960+line_length,540+30+division_value*6);
}
void _high_aim_()
{
		//帧头
		high_aim_figure.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		high_aim_figure.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
		high_aim_figure.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&high_aim_figure.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		high_aim_figure.CmdID = 0x301;

		//数据段头结构
		high_aim_figure.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
		high_aim_figure.dataFrameHeader.send_ID     = game_robot_state.robot_id;;
		high_aim_figure.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		aim_1(division_value,10);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&high_aim_figure.CmdID, LEN_CMD_ID+high_aim_figure.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(high_aim_figure));
		
    for(int i = 0; i < sizeof(high_aim_figure); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
static void aim_lowshort_2(uint32_t division_value,uint32_t line_length)//准心上半部分的宽度"AL"--aim_low
{
	Figure_Graphic(&low_aim_shortfigure_1.clientData[0],"AL1",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30                 ,0,  960+line_length,540-30);//graphic_Remove
	Figure_Graphic(&low_aim_shortfigure_1.clientData[1],"AL2",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value  ,0,  960+line_length,540-30-division_value  );
	Figure_Graphic(&low_aim_shortfigure_1.clientData[2],"AL3",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*2,0,  960+line_length,540-30-division_value*2);
	Figure_Graphic(&low_aim_shortfigure_1.clientData[3],"AL4",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*4,0,  960+line_length,540-30-division_value*4);
	Figure_Graphic(&low_aim_shortfigure_1.clientData[4],"AL5",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*5,0,  960+line_length,540-30-division_value*5);
	Figure_Graphic(&low_aim_shortfigure_1.clientData[5],"AL6",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*6,0,  960+line_length,540-30-division_value*6);
	Figure_Graphic(&low_aim_shortfigure_1.clientData[6],"AL7",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*8,0,  960+line_length,540-30-division_value*8);
}
void _lowshort_aim_2()
{
		//帧头
		low_aim_shortfigure_1.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		low_aim_shortfigure_1.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
		low_aim_shortfigure_1.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&low_aim_shortfigure_1.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		low_aim_shortfigure_1.CmdID = 0x301;

		//数据段头结构
		low_aim_shortfigure_1.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
		low_aim_shortfigure_1.dataFrameHeader.send_ID     =	game_robot_state.robot_id;
		low_aim_shortfigure_1.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		aim_lowshort_2(division_value,10);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&low_aim_shortfigure_1.CmdID, LEN_CMD_ID+low_aim_shortfigure_1.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(low_aim_shortfigure_1));
		
    for(int i = 0; i < sizeof(low_aim_shortfigure_1); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
static void aim_lowshort_3(uint32_t division_value,uint32_t line_length)//准心上半部分的宽度"AM"--aim_low_middle
{
	Figure_Graphic(&low_aim_shortfigure_2.clientData[0],"AM1",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*9 ,0,  960+line_length,540-30-division_value*9 );//graphic_Remove
	Figure_Graphic(&low_aim_shortfigure_2.clientData[1],"AM2",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*10,0,  960+line_length,540-30-division_value*10);
	Figure_Graphic(&low_aim_shortfigure_2.clientData[2],"AM3",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*12,0,  960+line_length,540-30-division_value*12);
	Figure_Graphic(&low_aim_shortfigure_2.clientData[3],"AM4",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*13,0,  960+line_length,540-30-division_value*13);
	Figure_Graphic(&low_aim_shortfigure_2.clientData[4],"AM5",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*14,0,  960+line_length,540-30-division_value*14);
	Figure_Graphic(&low_aim_shortfigure_2.clientData[5],"AM6",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*16,0,  960+line_length,540-30-division_value*16);
	Figure_Graphic(&low_aim_shortfigure_2.clientData[6],"AM7",ADD,LINE,3,YELLOW,0,0,1,  960-line_length,540-30-division_value*17,0,  960+line_length,540-30-division_value*17);
}
void _lowshort_aim_3()
{
		//帧头
		low_aim_shortfigure_2.txFrameHeader.SOF = JUDGE_FRAME_HEADER;
		low_aim_shortfigure_2.txFrameHeader.DataLength = sizeof(ext_student_interactive_header_data_t) + sizeof(graphic_data_struct_t)*7;
		low_aim_shortfigure_2.txFrameHeader.Seq = 0;//包序号
		memcpy(CliendTxBuffer,&low_aim_shortfigure_2.txFrameHeader,sizeof(xFrameHeader));
		Append_CRC8_Check_Sum(CliendTxBuffer, sizeof(xFrameHeader));//头校验

		//命令码
		low_aim_shortfigure_2.CmdID = 0x301;

		//数据段头结构
		low_aim_shortfigure_2.dataFrameHeader.data_cmd_id = INTERACT_ID_draw_seven_graphic;
		low_aim_shortfigure_2.dataFrameHeader.send_ID     = game_robot_state.robot_id;
		low_aim_shortfigure_2.dataFrameHeader.receiver_ID = Robot_receiver_ID;
	
		//数据段
		aim_lowshort_3(division_value,10);
		memcpy(CliendTxBuffer+LEN_FRAME_HEAD, (uint8_t*)&low_aim_shortfigure_2.CmdID, LEN_CMD_ID+low_aim_shortfigure_2.txFrameHeader.DataLength);//加上命令码长度2

		//帧尾
		Append_CRC16_Check_Sum(CliendTxBuffer,sizeof(low_aim_shortfigure_2));
		
    for(int i = 0; i < sizeof(low_aim_shortfigure_2); i++) {
      UART8_SendChar(CliendTxBuffer[i]);
    }
}
bool IF_start_ui=1;
void Startjudge_task(void)
{	
  static int i = -1,J = 0;
  i++;
  J++;
  UI_Init();
  UI_DataUpdate();
  
	
if(IF_start_ui)
{
  switch(i)
  {
    case 0:
		  start_UI_line_1();
        break;
    case 1:
		  start_UI_char();
        break;
    case 2:
		  start_UI_line_2();
        break;
    case 3:
		  start_UI_line_3();
        break;
    default:
        i = -1;
        break;
  }
}else{
  switch(i)
  {
    case 0:
        Client_graphic_Init();   //文字
        break;
    
    case 1:
        global_fri     = UI.User.FRI;
        global_spin     = UI.User.SPIN;
        global_auto_aim = UI.User.AUTO;
        Client_graphic_Info_update();//圆圈
        update_figure_flag = MODIFY;
        break;
    
    case 2:
			if(!global_auto_aim)
			{
				_lowshortstem_aim_4();
			}
      break;
    case 3:
			if(!global_auto_aim)
			{
       _lowlong_aim_();
			}
      break;
    
    case 4:
        global_supercapacitor_remain = (UI.User.Vcap_show - 13)/(23-13) * 100;
        Client_supercapacitor_update();
        update_supercapacitor_flag = MODIFY;
        break;
    
    case 5:
			if(!global_auto_aim)
			{
        _high_aim_();
			}
        break;
        
    case 6:
			if(!global_auto_aim)
			{
        _lowshort_aim_2();
			}
        break;
    
    case 7:
			if(!global_auto_aim)
			{
        _lowshort_aim_3();
			}
        break;
    default:
        i = -1;
        break;
  }
}
  
  if(J == 100)
  {
    J = 0;
    UI.IF_Init_Over = false;
  }
  
}


