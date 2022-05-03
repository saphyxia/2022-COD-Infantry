#ifndef _CHASSIS_H
#define _CHASSIS_H
#include "stm32f4xx.h"
#include "motor.h"
#include "rc.h"
#include "filter.h"
#include "State_Task.h"


#define NATURAL_NUMBER  2.718281828f    //???e,??????????  

#define M_LW 	(32.f)
#define M_L 	(32.f) 
#define M_R 	(7.5f)	

/*?????????*/
#define RC_SEPPE_X_K    60
#define RC_SEPPE_Y_K    60
/*????????*/
#define KEY_SEPPE_X_K   40
#define KEY_SEPPE_Y_K   40
/*?????*/
#define   SPIN_SPEED_40W    300
#define   SPIN_SPEED_50W    330
#define   SPIN_SPEED_60W    360
#define   SPIN_SPEED_70W    390
#define   SPIN_SPEED_80W    420
#define   SPIN_SPEED_90W    450
#define   SPIN_SPEED_100W   480
#define   SPIN_SPEED_120W   510
#define   SPIN_SPEED_SUPER  600
//???????
#define    KeySlope_Y_60W    0.33f
#define    KeySlope_Y_80W    0.43f
#define    KeySlope_Y_100W   0.48f
#define    KeySlope_Y_120W   0.53f
#define    KeySlope_Y_40W    0.28f

#define    KeySlope_X_60W    0.33f
#define    KeySlope_X_80W    0.43f
#define    KeySlope_X_100W   0.48f
#define    KeySlope_X_120W   0.53f
#define    KeySlope_X_40W    0.28f

#define   CHAS_W_SPEED_MAX  2000

typedef enum
{
    _X,
    _Y,
    _W,
    SPEED_TYPE,
}CHASSIS_SPEED_TYPE;

typedef struct
{
    CHASSIS_CTRL_MODE ctrl;
    CHASSIS_ACT_MODE  act;

    int16_t CH2,CH3;//遥控器摇杆值
		//键盘数据解算
    union
    {
        uint16_t V;
        struct
        {
           	uint16_t W:1;
						uint16_t S:1;
						uint16_t A:1;
						uint16_t D:1;
						uint16_t SHIFT:1;
						uint16_t CTRL:1;
						uint16_t Q:1;
						uint16_t E:1;
						uint16_t R:1;
						uint16_t F:1;
						uint16_t G:1;
						uint16_t Z:1;
						uint16_t X:1;
						uint16_t C:1;
						uint16_t V:1;
						uint16_t B:1;
        } set;
    } key;

    float speed[SPEED_TYPE];//全向移动速度

    extKalman_t KF_FOLO_Angle;
    float midangle;//底盘跟随中值角
		
}CHASSIS_INFO_T;

extern CHASSIS_INFO_T Chassis;

//?????1
#define    Key_W       Chassis.key.set.W
#define    Key_S       Chassis.key.set.S
#define    Key_A       Chassis.key.set.A
#define    Key_D       Chassis.key.set.D
#define    Key_Q       Chassis.key.set.Q
#define    Key_E       Chassis.key.set.E
#define    Key_G       Chassis.key.set.G
#define    Key_X       Chassis.key.set.X
#define    Key_Z       Chassis.key.set.Z
#define    Key_C       Chassis.key.set.C
#define    Key_B       Chassis.key.set.B
#define    Key_V       Chassis.key.set.V
#define    Key_F       Chassis.key.set.F
#define    Key_R       Chassis.key.set.R
#define    Key_CTRL    Chassis.key.set.CTRL
#define    Key_SHIFT   Chassis.key.set.SHIFT

extern int16_t CAN1_SendData[4];
extern float Expect[4],output[4];


void CHASSIS_Set_PID(CHASSIS_CTRL_MODE ctrl_mode);
void CHASSIS_Stop(void);
float KEY_X_Ctrl(void);
float KEY_Y_Ctrl(void);
float Speed_X_deliver(float speed_X , float speed_Y);
float speed_Y_deliver(float speed_X , float speed_Y);
float Speed_W_deliver(CHASSIS_ACT_MODE mode);
void CHASSIS_Handler(float speed[SPEED_TYPE]);
float RAMP_float( float final, float now, float ramp );
#endif


