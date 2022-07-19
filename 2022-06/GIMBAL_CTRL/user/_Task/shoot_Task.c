/*摩擦轮任务

*/

#include "shoot_Task.h"

#include "vision_Task.h"


/*--------------------------   变量声明   ------------------------------*/
int8_t stuck_flag = 1;//卡弹时变号反转
bool IF_SHOOT_ENABLE = false;
SHOOT shoot=
{
    .state=OFF,
    .mode=SINGLE,
    .trigger_Buf=7,//拨盘叶数
    .dr16=&rc_ctrl,
    .wheel_L=&Gimbal[SHOOTL],
    .wheel_R=&Gimbal[SHOOTR],
    .trigger=&Gimbal[TRIGGER],

    .wheel_Speed={
        [ON]={
            [INITIAL]={//初始
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_15M_S,
                [LV_3]=SHOOT_SPEED_15M_S,
            },
            [BURST]={//爆发优先
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_15M_S,
                [LV_3]=SHOOT_SPEED_15M_S,
            },
            [COOLING]={//冷却优先
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_18M_S,
                [LV_3]=SHOOT_SPEED_18M_S,
            },
            [RATE]={//射速优先
                [LV_1]=SHOOT_SPEED_30M_S,
                [LV_2]=SHOOT_SPEED_30M_S,
                [LV_3]=SHOOT_SPEED_30M_S,
            },
        },
    },
    .state_Setup=shoot_State_Handoff,
};

float wheel_PID_Param[2][PID_Param_NUM]={
    [ON]={
        13,//P
        0.1f,//I
        0,//D
        1500,//Err_Max
        10000,//Integral_MAX
        15000,//P_OUT_MAX
        3000,//I_OUT_MAX
        0,//D_OUT_MAX
        15000 //T_OUT_MAX
    },
};
float trigger_PID_Param[2][2][PID_Param_NUM]={
    [ON]={
        [0]={
            16,//P
            0.24f,//I
            0,//D
            1000,//Err_Max
            10000,//Integral_MAX
            15000,//P_OUT_MAX
            1500,//I_OUT_MAX
            0,//D_OUT_MAX
            10000 //T_OUT_MAX
        },
        [1]={
            80,//P
            0,//I
            0,//D
            200,//Err_Max 
            0,//Integral_MAX
            10000,//P_OUT_MAX
            0,//I_OUT_MAX
			0,//D_OUT_MAX
            10000 //T_OUT_MAX
        },
    },
};

/*--------------------------   变量声明   ------------------------------*/

//调度任务
void SHOOT_TASK(void *args)
{
    uint32_t currentTime;
	while(1)
	{
		currentTime = xTaskGetTickCount();//当前系统时间

		if(Key_B() == true)
		{
			switch_shoot_mode();
		}

		wheel_Ctrl();
		if((shoot.state == ON) &&((ABS(shoot.wheel_L->Data.velocity) + ABS(shoot.wheel_R->Data.velocity))/2.f > 2000))
		{
			IF_SHOOT_ENABLE = true;
			trigger_Ctrl();
			trigger_Stall_Handle();
		}else{
			IF_SHOOT_ENABLE = false;
		}
		vTaskDelayUntil(&currentTime, SHOOT_TASK_TIM);//绝对延时
	}
}


//发射机构状态切换(在dbus任务中调用)
static void shoot_State_Handoff(DEVICE_STATE state)
{
    if(state!=shoot.state){//防止循环调用出错
    //参数重置（受当前模式影响）
		shoot.state=state;
        PID_Init_ByParamArray(&shoot.wheel_L->pid_Speed,wheel_PID_Param[state]);
        PID_Init_ByParamArray(&shoot.wheel_R->pid_Speed,wheel_PID_Param[state]);
        PID_Init_ByParamArray(&Gimbal[TRIGGER].pid_Speed,trigger_PID_Param[state][0]);
        PID_Init_ByParamArray(&Gimbal[TRIGGER].pid_Angle,trigger_PID_Param[state][1]);
    }
}
void switch_shoot_mode(void)
{
	if(shoot.mode == SINGLE)
	{
		shoot.mode = COILED;
	}else if(shoot.mode == COILED)
	{
		shoot.mode = SINGLE;
	}
}
float speed_gain = -140;
//摩擦轮速度控制（全模式调用）（未接入裁判系统，无射速限制）
static void wheel_Ctrl(void)
{
    int16_t send_Value[2];
    float speed_Err[2];
	float res = shoot.wheel_Speed[shoot.state][robot.mode][robot.level];
	float real_speed=robot.bullet_speed;
	static float speed_last = 0; 
	//射速发生变化
	if(real_speed != speed_last)
	{
		//射速控制
		if(res==SHOOT_SPEED_15M_S)
		{
			speed_gain += SpeedAdapt(real_speed, 13.7f , 14.3f , 15 , 35);
		}else if(res==SHOOT_SPEED_18M_S)
		{
			speed_gain += SpeedAdapt(real_speed , 16.7f , 17.3f , 15 , 40);
		}else if(res==SHOOT_SPEED_30M_S)
		{
			speed_gain += SpeedAdapt(real_speed , 27.7f , 28.3f , 25 , 55);
		}
	}
	res += speed_gain + Temp_Fix_speed(real_speed);
	speed_last = real_speed;
	//速度误差计算(符号相反)
    speed_Err[0] = (   res - shoot.wheel_L->Data.velocity);
    speed_Err[1] = (  -res - shoot.wheel_R->Data.velocity);

//速度控制(PID控制方案)
    send_Value[0] = shoot.wheel_L->pid_Speed.PID_Calc(&shoot.wheel_L->pid_Speed.middle,speed_Err[0]);
    send_Value[1] = shoot.wheel_R->pid_Speed.PID_Calc(&shoot.wheel_R->pid_Speed.middle,speed_Err[1]);
//发送装载
    shoot.wheel_L->txMsg->Data[H[shoot.wheel_L->Data.StdId-0x201]] = (uint8_t)(send_Value[0]>>8);
    shoot.wheel_L->txMsg->Data[L[shoot.wheel_L->Data.StdId-0x201]] = (uint8_t)(send_Value[0]);
    shoot.wheel_R->txMsg->Data[H[shoot.wheel_R->Data.StdId-0x201]] = (uint8_t)(send_Value[1]>>8);
    shoot.wheel_R->txMsg->Data[L[shoot.wheel_R->Data.StdId-0x201]] = (uint8_t)(send_Value[1]);
}
//卡弹判断
bool Judge_IF_SingeStuck(void)
{
	bool res = false;
	
	float angle_err = Gimbal[TRIGGER].pid_Angle.middle.Err;
	
	if(ABS(angle_err) >= 360.f/13.f)
	{
			res = true;
	}
	
	return res;
}
bool Judge_IF_AutoBlock(void)
{
	bool res = false;
	
	float speed_err = Gimbal[TRIGGER].pid_Speed.middle.Err;
	
	if(ABS(speed_err) > 500 && ABS(Gimbal[TRIGGER].Data.velocity) < 300)
	{
		res = true;
	}
	
	return res;
}
    float angle_Err1,speed_Err;

//拨弹盘控制（全模式调用）（未接入裁判系统，无射频限制）
static void trigger_Ctrl(void)
{	
    static bool IF_TRIGGER_READY=false;
    int16_t send_Value;
	float angle_heft;
    
	if(rc_ctrl.rc.ch[1] == 0 && Key_MOUSE_L()==false)
	{
		IF_TRIGGER_READY = false;
	}
//点射模式
    else if(IF_TRIGGER_READY == false)
	{
//期望角度计算
		if(shoot.mode==SINGLE)
		{
			angle_heft = 360.0f/7.f;
			if(robot.shooter_id1_17mm.cooling_limit - robot.cooling_heat >= 20)
			{
				shoot.trigger_Angle = Gimbal[TRIGGER].Data.angle	- stuck_flag   \
									* (rc_ctrl.rc.ch[1]/ABS(rc_ctrl.rc.ch[1])+Key_MOUSE_L())    \
									* angle_heft;
			}
		}else if(shoot.mode==COILED)
		{
			angle_heft = 360.0f/7.f*3.f;
			if(robot.shooter_id1_17mm.cooling_limit - robot.cooling_heat >= 40)
			{
				shoot.trigger_Angle = Gimbal[TRIGGER].Data.angle	- stuck_flag   \
									* (rc_ctrl.rc.ch[1]/ABS(rc_ctrl.rc.ch[1])+Key_MOUSE_L())    \
									* angle_heft;
			}
		}
		IF_TRIGGER_READY = true;
	}
//角度误差计算
			angle_Err1 = shoot.trigger_Angle - Gimbal[TRIGGER].Data.angle;
//速度误差计算
			speed_Err = Gimbal[TRIGGER].pid_Angle.PID_Calc(&Gimbal[TRIGGER].pid_Angle.middle,angle_Err1) - Gimbal[TRIGGER].Data.velocity;
    if(shoot.mode==AUTO)
	{
//连发模式
		shoot.trigger_Angle = Gimbal[TRIGGER].Data.angle;
//速度误差计算
		speed_Err = (-Trigger_Speed_deliver(robot.shooter_id1_17mm.cooling_rate)*(//rc_ctrl.rc.ch[1]/ABS(rc_ctrl.rc.ch[1])+
					 Key_MOUSE_L())-rc_ctrl.rc.ch[1]*8.f)*stuck_flag - Gimbal[TRIGGER].Data.velocity;
	}
//	if((key_KF&&vision.tx2->isFind) || Key_C()==true)//开启预测
//	{
//		speed_Err = -Trigger_Speed_deliver(robot.shooter_id1_17mm.cooling_rate)*(IF_Fire_Ready||Key_MOUSE_L())*stuck_flag \
//					- Gimbal[TRIGGER].Data.velocity;//自动开火判断
//	}
//速度控制(PID控制方案)
	send_Value = Gimbal[TRIGGER].pid_Speed.PID_Calc(&Gimbal[TRIGGER].pid_Speed.middle,speed_Err);
//发送装载
    Gimbal[TRIGGER].txMsg->Data[H[Gimbal[TRIGGER].Data.StdId-0x201]] = (uint8_t)(send_Value>>8);
    Gimbal[TRIGGER].txMsg->Data[L[Gimbal[TRIGGER].Data.StdId-0x201]] = (uint8_t)(send_Value);
}
/**
 * @brief 检查射速
 * @note  摩擦轮变速，动态调节转速
 */
static float Temp_Fix_speed(uint16_t real_speed)
{
  float temp_scope = 35;//假设变化范围为50摄氏度
  float temp_low = 35;//初始温度设定为35摄氏度
  float res = 0;
  float temp_real= ((float)shoot.wheel_L->Data.temperature + 
               (float)shoot.wheel_R->Data.temperature)/2.f;
  float speed_bap;
  
  if(13.5f<real_speed<=15.f)
  {
	speed_bap = -50.f;
  }else if(16.5f<real_speed<=18.f)
  {
	speed_bap = -70.f;
  }else if(27.5f<real_speed<=30.f)
  {
	speed_bap = -170.f;
  }
  
  if(temp_real >= temp_low)
    res = (temp_real - temp_low)/temp_scope * (-speed_bap);
  if(temp_real < temp_low)
    res = 0;
  if(temp_real > temp_low + temp_scope)
    res = -speed_bap;

	return res;
}
uint8_t SpeedErr_cnt=0;
static float SpeedAdapt(float real_S , float min_S, float max_S,float up_num , float down_num)
{
	float res=0;

  if(real_S < min_S && real_S > 8)
    SpeedErr_cnt++;
  else if(real_S >= min_S && real_S <= max_S )SpeedErr_cnt = 0;
  if(SpeedErr_cnt == 1)//射速偏低
  {
    SpeedErr_cnt = 0;
    res += up_num;
  }
  if(real_S > max_S)//射速偏高
    res -= down_num;
  return res;
}
//堵转判断和处理（全模式调用）
static void trigger_Stall_Handle(void)
{
	static uint16_t cnt = 0;
	
	if(Judge_IF_AutoBlock()==true || Judge_IF_SingeStuck()==true)
	{
		cnt++;
		if(cnt > 300)
		{
			stuck_flag = -stuck_flag;
			cnt = 0;
		}
	}else{
			cnt = 0;
	}
}

static uint16_t Trigger_Speed_deliver(uint16_t cooling_rate)
{
	float res = 0;
	
	switch(cooling_rate)//枪口每秒冷却值
	{
		case 15:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)//2发余量
				res = TRIGGER_FREQ_2_HZ;
			else 
				res = 0;
		break;
		case 25:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)
				res = TRIGGER_FREQ_5_HZ;
			else 
				res = 0;
		break;
		case 35:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 30)
				res = TRIGGER_FREQ_6_HZ;
			else 
				res = TRIGGER_FREQ_3_HZ;
		break;
		case 40:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 30)
				res = TRIGGER_FREQ_5_HZ;
			else 
				res = TRIGGER_FREQ_2_HZ;
		break;
		case 60:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 30)
				res = TRIGGER_FREQ_8_HZ;
			else
				res = TRIGGER_FREQ_5_HZ;
		break;
		case 80:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 30)
				res = TRIGGER_FREQ_10_HZ;
			else 
				res = TRIGGER_FREQ_7_HZ;							
		break;
		default:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)//2发余量
				res = TRIGGER_FREQ_2_HZ;
			else 
				res = 0;
		break;
	}
	return res;
}
