/*摩擦轮任务

*/

#include "shoot_Task.h"

#include "vision_Task.h"


/*--------------------------   变量声明   ------------------------------*/
int8_t stuck_flag = 1;//卡弹时变号反转
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
                [LV_0]=SHOOT_SPEED_15M_S,
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_15M_S,
                [LV_3]=SHOOT_SPEED_15M_S,
            },
            [BURST]={//爆发优先
                [LV_0]=SHOOT_SPEED_15M_S,
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_15M_S,
                [LV_3]=SHOOT_SPEED_15M_S,
            },
            [COOLING]={//冷却优先
                [LV_0]=SHOOT_SPEED_15M_S,
                [LV_1]=SHOOT_SPEED_15M_S,
                [LV_2]=SHOOT_SPEED_18M_S,
                [LV_3]=SHOOT_SPEED_18M_S,
            },
            [RATE]={//射速优先
                [LV_0]=SHOOT_SPEED_15M_S,
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

//		if(Key_B() == false)
//		{
//			shoot.mode = SINGLE;
//		}else if(Key_B() == true)
//		{
			shoot.mode = AUTO;
//		}

		wheel_Ctrl();
		trigger_Ctrl();
		trigger_Stall_Handle();
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


//摩擦轮速度控制（全模式调用）（未接入裁判系统，无射速限制）
static void wheel_Ctrl(void)
{
    int16_t send_Value[2];
    float speed_Err[2];
	float res = shoot.wheel_Speed[shoot.state][robot.mode][robot.level];
	float real_speed=robot.bullet_speed;
	
	//温度控制,射速控制
	if(res==SHOOT_SPEED_15M_S)
	{
		res += Temp_Fix_15S();
		res += SpeedAdapt(real_speed, 12.5 , 13.5 , 15 , 35);
	}else if(res==SHOOT_SPEED_18M_S)
	{
		res += Temp_Fix_18S();
		res += SpeedAdapt(real_speed , 15.8 , 16.8 , 15 , 35);
	}else if(res==SHOOT_SPEED_30M_S)
	{
		res += Temp_Fix_30S();
		res += SpeedAdapt(real_speed , 27.5 , 28.5  , 25 , 50);
	}
	//速度误差计算(符号相反)
    speed_Err[0] = (  res - shoot.wheel_L->Data.velocity);
    speed_Err[1] = ( -res - shoot.wheel_R->Data.velocity);

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
    
		if(rc_ctrl.rc.ch[1] == 0 && Key_MOUSE_L()==false)
		{
			IF_TRIGGER_READY = false;
		}
//点射模式
    else if(shoot.mode==SINGLE&&IF_TRIGGER_READY == false)
		{
//期望角度计算
			if(robot.shooter_id1_17mm.cooling_limit - robot.cooling_heat >= 20)
			{
				shoot.trigger_Angle = Gimbal[TRIGGER].Data.angle	- stuck_flag   \
															*(-rc_ctrl.rc.ch[1]/ABS(rc_ctrl.rc.ch[1])-Key_MOUSE_L())    \
															*360.0f/7.f;
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
		speed_Err = (-Trigger_Speed_deliver(robot.shooter_id1_17mm.cooling_rate)*Key_MOUSE_L()  \
								 -8*rc_ctrl.rc.ch[1])*stuck_flag - Gimbal[TRIGGER].Data.velocity;
	}
	if((key_KF&&vision.tx2->isFind) || Key_C()==true)//开启预测
	{
		speed_Err = -Trigger_Speed_deliver(25)*(IF_Fire_Ready||Key_MOUSE_L())*stuck_flag \
					- Gimbal[TRIGGER].Data.velocity;//自动开火判断
	}
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
static float Temp_Fix_30S(void)
{
  float temp_scope = 35;//假设变化范围为35摄氏度
  float temp_low = 35;//初始温度设定为35摄氏度
  float res = 0;
  float temp_real;
  
  temp_real = ((float)shoot.wheel_L->Data.temperature + 
               (float)shoot.wheel_R->Data.temperature)/2;
  
  if(temp_real >= temp_low)
    res = (temp_real - temp_low)/temp_scope * (-168);
  if(temp_real < temp_low)
    res = 0;
  if(temp_real > temp_low + temp_scope)
    res = -168;
  
  return res;
}
static float Temp_Fix_18S(void)
{
  float temp_scope = 35;//假设变化范围为25摄氏度
  float temp_low = 35;//初始温度设定为35摄氏度
  float res = 0;
  float temp_real;
  
  temp_real = ((float)shoot.wheel_L->Data.temperature + 
               (float)shoot.wheel_R->Data.temperature)/2;
  
  if(temp_real >= temp_low)
    res = (temp_real - temp_low)/temp_scope * (-70);
  if(temp_real < temp_low)
    res = 0;
  if(temp_real > temp_low + temp_scope)
    res = -70;
  
	return res;
}
static float Temp_Fix_15S(void)
{
  float temp_scope = 35;//假设变化范围为50摄氏度
  float temp_low = 35;//初始温度设定为35摄氏度
  float res = 0;
  float temp_real;
  
  temp_real = ((float)shoot.wheel_L->Data.temperature + 
               (float)shoot.wheel_R->Data.temperature)/2;
  
  if(temp_real >= temp_low)
    res = (temp_real - temp_low)/temp_scope * (-50);
  if(temp_real < temp_low)
    res = 0;
  if(temp_real > temp_low + temp_scope)
    res = -50;

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
		if(cnt > 100)
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
				res = TRIGGER_FREQ_2_HZ;
		break;
		case 35:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)
				res = TRIGGER_FREQ_6_HZ;
			else 
				res = TRIGGER_FREQ_3_HZ;
		break;
		case 40:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)
				res = TRIGGER_FREQ_5_HZ;
			else 
				res = TRIGGER_FREQ_4_HZ;
		break;
		case 60:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)
				res = TRIGGER_FREQ_8_HZ;
			else
				res = TRIGGER_FREQ_6_HZ;
		break;
		case 80:
			if((robot.shooter_id1_17mm.cooling_limit-robot.cooling_heat) >= 20)
				res = TRIGGER_FREQ_10_HZ;
			else 
				res = TRIGGER_FREQ_8_HZ;							
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
