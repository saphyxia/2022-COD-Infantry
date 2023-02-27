#ifndef CHASSISTASK_H
#define CHASSISTASK_H
/* includes ------------------------------------------------------------------*/
#include "main.h"
#include "rc.h" 
#include "ImuTask.h"
#include "FreeRTOS_Init.h"
#include "motor.h"

/* Private macro -------------------------------------------------------------*/
//底盘信息,全向解算用
#define M_LW 	(32.f)
#define M_L 	(32.f)
#define M_R 	(7.5f)
//键盘斜坡增量
#define KEYSLOPE   0.66f
//底盘控制量增益
#define CHAS_CONTROL_GAIN    50
//自旋速度
#define   SPIN_SPEED_40W    500
#define   SPIN_SPEED_50W    530
#define   SPIN_SPEED_60W    560
#define   SPIN_SPEED_70W    590
#define   SPIN_SPEED_80W    610
#define   SPIN_SPEED_90W    640
#define   SPIN_SPEED_100W   670
#define   SPIN_SPEED_120W   700
#define   SPIN_SPEED_SUPER  800

/* Exported macro -------------------------------------------------------------*/
//自然数e
#define NATURAL_NUMBER  2.718281828f
//交互丢失时间,单位ms
#define INTERAC_LOST_TIME 150

/* Exported typedef -----------------------------------------------------------*/
//底盘状态枚举
typedef enum
{
	ERR,
	INVA,
	FOLO,
	SPIN,
	STATE_CNT,
}CHAS_MODE;
//底盘信息封装
typedef struct
{
	//底盘状态
	DEVICE_STATE state;
    CHAS_MODE mode;
	//交互方式：CAN
	CAN_TypeDef *CANx;
	uint32_t stdid;
	//期望解算
	float target_FR,target_LR,target_GYRO;
	float midangle;
	//控制信息
	int16_t ch2,ch3;
	RC_KEY_T key;
	//底盘电机封装
	DJI_MOTOR *LF,*LR,*RF,*RR,*FOLO;
}CHASSIS_INFO_T;

/* Exported variables --------------------------------------------------------*/
extern CHASSIS_INFO_T chassis;
extern TickType_t CapTick;
extern bool IF_SUPERCAP_ONLINE;
/* Private function prototypes -----------------------------------------------*/
//底盘状态切换
static void CHAS_STATE_Switch(TickType_t *);
//PID装载
static void CHAS_Set_PID(DEVICE_STATE *);
//底盘卸力
static void CHAS_Motor_Stop(void);
//键盘数据拟合
static void Keyboard_Motion_FITTING(float *);
//自旋速度装载
static float Spin_velocity_Load(float *);
//全向移动拟合
static void Omnidirection_FITTING(CHASSIS_INFO_T *);
//底盘姿态控制
static void CHAS_Posture_Ctrl(void);
//加速曲线
static float AcclerateCurve(float,float);
//减速曲线
static float DecclerateCurve(float,float);
//底盘变速曲线
static void Chassis_VariableCurve(float *,float *);
//底盘功率控制
static void CHAS_Power_Control(float *);

/* Exported functions --------------------------------------------------------*/
extern void ChassisTask(void *pvParameters);
extern void get_Chas_INFO(CAN_TypeDef *CANx,CanRxMsg *RxMsg,CHASSIS_INFO_T *std);
#endif


