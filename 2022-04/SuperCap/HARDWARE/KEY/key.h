#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define VAL_LIMIT(val,min,max){	if(val <= min)	val=min;		\
										else if(val >=max)	val=max;	\
									}
#define ABS(x)	( (x>0) ? (x) : (-x) )

typedef struct 
{
	float err;
	float err_last;
	float err_last_last;
	float Kp,Ki,Kd;
	float result;
	float result_min;
	float result_max;
}PID_Struct;


//#define KEY0 PEin(4)  //PE4
//#define KEY1 PEin(3)	//PE3 
//#define KEY2 PEin(2)	//PE2
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

 

#define KEY0_PRES 	1	//KEY0按下
#define KEY1_PRES	2	//KEY1按下
#define KEY2_PRES	3	//KEY2按下
#define WKUP_PRES   4	//KEY_UP按下(即WK_UP/KEY_UP)

extern PID_Struct Current_PID,Power_PID;;

void KEY_Init(void);//IO初始化
u8 KEY_Scan(u8);  	//按键扫描函数		
float Pid(float err,PID_Struct *p);
void PID_Init(void);
#endif
