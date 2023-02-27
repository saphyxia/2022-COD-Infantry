#ifndef RC_H
#define RC_H
#include "main.h"
#include "filter.h"

extern void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num);
extern void RC_unable(void);
extern void RC_restart(uint16_t dma_buf_num);

/*鼠标速度最大值限制*/
#define Xmax    300
#define Ymax    300
/*鼠标滑动滤波长度*/
#define SF_LENGTH 30  

#define SBUS_RX_BUF_NUM 36u

#define RC_FRAME_LENGTH 18u

//遥控器出错数据上限
#define RC_CHANNAL_ERROR_VALUE 700


#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)

/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)
#define switch_is_down(s) (s == RC_SW_DOWN)
#define switch_is_mid(s) (s == RC_SW_MID)
#define switch_is_up(s) (s == RC_SW_UP)

/*按键时间 长短按的判断*/
#define SHORT_CHANGE_TIM            5   	//ms
#define LONG_CHANGE_TIM_W      		300		//ms
#define LONG_CHANGE_TIM_S      		300		//ms
#define LONG_CHANGE_TIM_A      		300		//ms
#define LONG_CHANGE_TIM_D      		300		//ms
#define LONG_CHANGE_TIM_Q      		300 	//ms
#define LONG_CHANGE_TIM_E      		300 	//ms
#define LONG_CHANGE_TIM_R      		300 	//ms
#define LONG_CHANGE_TIM_F      		500 	//ms
#define LONG_CHANGE_TIM_G      		1500 	//ms
#define LONG_CHANGE_TIM_Z      		150 	//ms
#define LONG_CHANGE_TIM_X      		150 	//ms
#define LONG_CHANGE_TIM_C      		300 	//ms
#define LONG_CHANGE_TIM_V      		300 	//ms
#define LONG_CHANGE_TIM_B      		300 	//ms
#define LONG_CHANGE_TIM_CTRL   		300	    //ms
#define LONG_CHANGE_TIM_SHIFT  		300 	//ms
#define LONG_CHANGE_TIM_MOUSE_L		300 	//ms
#define LONG_CHANGE_TIM_MOUSE_R		300 	//ms

/* 检测键盘按键状态 */
#define KEY_UP                    0x00
#define KEY_DOWN                  0x01

/*按键状态枚举*/
typedef enum
{
	UP,			//抬起
	SHORT_DOWN,	//短按
	DOWN,		//长按
	PRESS,		//0->1
	RELAX,		//1->0
	KEY_STATE_CNT,
}KEY_SET_STATE;

/*单独按键信息*/
typedef struct 
{  
  uint16_t state_cnt;
  uint8_t State;
  KEY_SET_STATE prev_State;
  bool prev_KEY_PRESS;
  bool KEY_PRESS;
}KEY_SET_INFO_T;

/*总体键盘按键信息*/
typedef struct
{
	KEY_SET_INFO_T W;
	KEY_SET_INFO_T S;
	KEY_SET_INFO_T A;
	KEY_SET_INFO_T D;
	KEY_SET_INFO_T SHIFT;
	KEY_SET_INFO_T CTRL;
	KEY_SET_INFO_T Q;
	KEY_SET_INFO_T E;
	KEY_SET_INFO_T R;
	KEY_SET_INFO_T F;
	KEY_SET_INFO_T G;
	KEY_SET_INFO_T Z;
	KEY_SET_INFO_T X;
	KEY_SET_INFO_T C;
	KEY_SET_INFO_T V;
	KEY_SET_INFO_T B;
	KEY_SET_INFO_T MOUSE_L;
	KEY_SET_INFO_T MOUSE_R;

}KEYBOARD_INFO_T;

extern KEYBOARD_INFO_T Key_Info;

extern extKalman_t KF_Mouse_X_Speed,KF_Mouse_Y_Speed;
/* ----------------------- Data Struct ------------------------------------- */
typedef  struct
{
	struct
	{
		int16_t ch[5];
		char s[2];
	} rc;
	struct
	{
		int16_t x;
		int16_t y;
		int16_t z;
		uint8_t press_l;
		uint8_t press_r;
	} mouse;
	union
	{
		uint16_t v;
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
	uint32_t update_Time;
} RC_ctrl_t;
extern RC_ctrl_t rc_ctrl;

//鼠标位移速度
#define    MOUSE_X_MOVE_SPEED    (rc_ctrl.mouse.x )
#define    MOUSE_Y_MOVE_SPEED    (rc_ctrl.mouse.y )
#define    MOUSE_Z_MOVE_SPEED    (rc_ctrl.mouse.z )

//鼠标按下为1
#define    MOUSE_PRESSED_LEFT    (rc_ctrl.mouse.press_l)
#define    MOUSE_PRESSED_RIGH    (rc_ctrl.mouse.press_r)

//按键按下为1
#define    KEY_PRESSED_W       rc_ctrl.key.set.W
#define    KEY_PRESSED_S       rc_ctrl.key.set.S
#define    KEY_PRESSED_A       rc_ctrl.key.set.A
#define    KEY_PRESSED_D       rc_ctrl.key.set.D
#define    KEY_PRESSED_Q       rc_ctrl.key.set.Q
#define    KEY_PRESSED_E       rc_ctrl.key.set.E
#define    KEY_PRESSED_G       rc_ctrl.key.set.G
#define    KEY_PRESSED_X       rc_ctrl.key.set.X
#define    KEY_PRESSED_Z       rc_ctrl.key.set.Z
#define    KEY_PRESSED_C       rc_ctrl.key.set.C
#define    KEY_PRESSED_B       rc_ctrl.key.set.B
#define    KEY_PRESSED_V       rc_ctrl.key.set.V
#define    KEY_PRESSED_F       rc_ctrl.key.set.F
#define    KEY_PRESSED_R       rc_ctrl.key.set.R
#define    KEY_PRESSED_CTRL    rc_ctrl.key.set.CTRL
#define    KEY_PRESSED_SHIFT   rc_ctrl.key.set.SHIFT

extern void remote_control_init(void);

extern  void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl);
float Mouse_X_Speed(void);
float Mouse_Y_Speed(void);
bool Key_SHIFT(void);
bool Key_CTRL(void);
bool Key_MOUSE_L(void);
bool Key_MOUSE_R(void);
bool Key_R(void);
void Key_Q(void);
void Key_E(void);
bool Key_B(void);
bool Key_C(void);
bool Key_X(void);
#endif
