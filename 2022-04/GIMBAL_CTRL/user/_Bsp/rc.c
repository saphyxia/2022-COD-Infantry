/* COD 开发板C型 串口3 初始化程序
  
 使用说明：
	 1.初始化函数为串口3，对应C板上DBUS
   2.开启DMA功能，用于遥控器接受数据
   3.自行编写数据处理部分
*/    
#include "rc.h"
#include "stm32f4xx.h"
#include "gimbal_Task.h"

KEYBOARD_INFO_T Key_Info;

void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		DMA_InitTypeDef DMA_InitStructure;

	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_DMA1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3); 

    USART_InitTypeDef USART_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    USART_DeInit(USART3);

    USART_InitStructure.USART_BaudRate = 100000;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_Even;
    USART_InitStructure.USART_Mode = USART_Mode_Rx;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3, &USART_InitStructure);

    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);

    USART_ClearFlag(USART3, USART_FLAG_IDLE);
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

    USART_Cmd(USART3, ENABLE);
		DMA_DeInit(DMA1_Stream1);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART3->DR);
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)rx1_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = dma_buf_num;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream1, &DMA_InitStructure);
    DMA_DoubleBufferModeConfig(DMA1_Stream1, (uint32_t)rx2_buf, DMA_Memory_0);
    DMA_DoubleBufferModeCmd(DMA1_Stream1, ENABLE);
    DMA_Cmd(DMA1_Stream1, DISABLE); 
    DMA_Cmd(DMA1_Stream1, ENABLE);
    
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);   
}


void RC_unable(void)
{
    USART_Cmd(USART3, DISABLE);
}
void RC_restart(uint16_t dma_buf_num)
{
    USART_Cmd(USART3, DISABLE);
    DMA_Cmd(DMA1_Stream1, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream1, dma_buf_num);

    USART_ClearFlag(USART3, USART_FLAG_IDLE);

    DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1);
    DMA_ClearITPendingBit(DMA1_Stream1, DMA_IT_TCIF1);
    DMA_Cmd(DMA1_Stream1, ENABLE);
    USART_Cmd(USART3, ENABLE);
}



//遥控器控制变量
//static 
RC_ctrl_t rc_ctrl;
//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

//初始化DMA，串口1

void remote_control_init(void)
{
    RC_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}

////串口中断
void USART3_IRQHandler(void)
{
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART3);
    }
    else if (USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
			static uint16_t this_time_rx_len = 0;

        USART_ReceiveData(USART3);

        if(DMA_GetCurrentMemoryTarget(DMA1_Stream1) == 0)
        {
            //重新设置DMA
            DMA_Cmd(DMA1_Stream1, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
            DMA_SetCurrDataCounter(DMA1_Stream1, SBUS_RX_BUF_NUM);
            DMA1_Stream1->CR |= DMA_SxCR_CT;
            //清DMA中断标志
            DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA1_Stream1, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[0], &rc_ctrl);
                //记录数据接收时间
            }
        }
        else
        {
            //重新设置DMA
            DMA_Cmd(DMA1_Stream1, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA1_Stream1);
            DMA_SetCurrDataCounter(DMA1_Stream1, SBUS_RX_BUF_NUM);
            DMA1_Stream1->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA1_Stream1, DMA_FLAG_TCIF1 | DMA_FLAG_HTIF1);
            DMA_Cmd(DMA1_Stream1, ENABLE);
            if(this_time_rx_len == RC_FRAME_LENGTH)
            {
                //处理遥控器数据
                SBUS_TO_RC(SBUS_rx_buf[1], &rc_ctrl);
                //记录数据接收时间
            }
        }
			
    }
}


void SBUS_TO_RC(volatile const uint8_t *sbus_buf, RC_ctrl_t *rc_ctrl)
{
    if (sbus_buf == NULL || rc_ctrl == NULL)
			{
					return;
			}
		rc_ctrl->update_Time=xTaskGetTickCount();

		/* Channel 0, 1, 2, 3 */
		rc_ctrl->rc.ch[0] = (  sbus_buf[0]       | (sbus_buf[1] << 8 ) ) & 0x07ff;                            //!< Channel 0
		rc_ctrl->rc.ch[1] = ( (sbus_buf[1] >> 3) | (sbus_buf[2] << 5 ) ) & 0x07ff;                            //!< Channel 1
		rc_ctrl->rc.ch[2] = ( (sbus_buf[2] >> 6) | (sbus_buf[3] << 2 ) | (sbus_buf[4] << 10) ) & 0x07ff;      //!< Channel 2
		rc_ctrl->rc.ch[3] = ( (sbus_buf[4] >> 1) | (sbus_buf[5] << 7 ) ) & 0x07ff;                            //!< Channel 3
		rc_ctrl->rc.ch[4] = (  sbus_buf[16] 		 | (sbus_buf[17] << 8) ) & 0x07ff;                 						//!< Channel 4	
	  /* Switch left, right */	
    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                  //!< Switch left
    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;             //!< Switch right
		
		/* Mouse axis: X, Y, Z */	
    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
		
		/* Mouse Left, Right Is Press  */
    rc_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press 
    rc_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press 
		
		/* KeyBoard value */	
    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
		
    rc_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;
		
}


/**
 * @brief 滑动滤波
 * @note  
 * @param 
 */
static float Sliding_Filter(float t,float *slideFilter)
{
    float res = 0.f;

    for(int i = SF_LENGTH-1; i > 0; i-- )
    {
        slideFilter[i] = slideFilter[i-1];
    }
    slideFilter[0] = t;
    for(int i = 0; i < SF_LENGTH-1; i++)
    {
        res +=slideFilter[i];
    }
    return (res/SF_LENGTH);
}

extKalman_t KF_Mouse_X_Speed,KF_Mouse_Y_Speed;
static float sliding_mouse_x[SF_LENGTH]={0.f,},sliding_mouse_y[SF_LENGTH]={0.f};

float Mouse_X_Speed(void)
{
    float res=0.f;

    if(abs(MOUSE_X_MOVE_SPEED) > Xmax){
        res = 0;
    }else{
        res = Sliding_Filter(KalmanFilter(&KF_Mouse_X_Speed,(float)MOUSE_X_MOVE_SPEED),sliding_mouse_x);
    } 
    return (float)res;
}
float Mouse_Y_Speed(void)
{
    float res=0.f;

    if(abs(MOUSE_Y_MOVE_SPEED) > Ymax){
        res = 0;
    }else{
        res = Sliding_Filter(KalmanFilter(&KF_Mouse_Y_Speed,(float)MOUSE_Y_MOVE_SPEED),sliding_mouse_y);
    }
    return (float)res;
}

/**
 * @brief 判断按键是否按下
 * @note 
 * @param 
 */
void FirstGetInto_KEY_PRESS(KEY_SET_INFO_T *key_set_Info)
{
  if(key_set_Info->prev_KEY_PRESS != key_set_Info->KEY_PRESS)
  {
    key_set_Info->state_cnt = 0;
    key_set_Info->prev_KEY_PRESS = key_set_Info->KEY_PRESS;
  }
}
void KEY_State_Judge(KEY_SET_INFO_T *key_set_Info , uint8_t KEY_PRESS , int change_tim ,int long_change_tim)
{
    key_set_Info->KEY_PRESS = KEY_PRESS;
    FirstGetInto_KEY_PRESS(key_set_Info);
    if(KEY_PRESS == KEY_UP)
    {
        if(key_set_Info->prev_State != UP)
        {
            key_set_Info->state_cnt++;
            if(key_set_Info->state_cnt >= change_tim + 1)
            {
                key_set_Info->State = UP;
                key_set_Info->prev_State = UP;
            }
            else if(key_set_Info->state_cnt >= change_tim)
            {
                key_set_Info->State = RELAX;
                key_set_Info->prev_State = RELAX;
            }
        }else
        {
            key_set_Info->state_cnt = 0;
        }
    }
    else if(KEY_PRESS == KEY_DOWN)
    {
        if(key_set_Info->prev_State != DOWN) 
        {
            key_set_Info->state_cnt++;
            if(key_set_Info->state_cnt >= long_change_tim)  
            {
                key_set_Info->State = DOWN;
                key_set_Info->prev_State = DOWN;
            }
            else if(key_set_Info->state_cnt >= change_tim + 1)
            {
                key_set_Info->State = SHORT_DOWN;
                key_set_Info->prev_State = SHORT_DOWN;
            }
            else if(key_set_Info->state_cnt >= change_tim)  
            {
            key_set_Info->State = PRESS;
            key_set_Info->prev_State = PRESS;
            }
        }else
        {
            key_set_Info->state_cnt = 0;
        }
    }
}

bool Key_SHIFT(void)
{
	static bool res = false;

	KEY_State_Judge(&Key_Info.SHIFT ,KEY_PRESSED_SHIFT, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_SHIFT);

	switch(Key_Info.SHIFT.State)
	{
	case UP:
	break;
	case PRESS:
	break;
	case SHORT_DOWN:
		res = true;
	break;
	case DOWN:
	break;
	case RELAX:
		res = false;
	break;
	}
	return res;
}
bool Key_CTRL(void)
{
    static bool res = false;

    KEY_State_Judge(&Key_Info.CTRL ,KEY_PRESSED_CTRL, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_CTRL);

    switch(Key_Info.CTRL.State)
    {
        case UP:
        break;
        case PRESS:
        break;
        case SHORT_DOWN:
            res = true;
        break;
        case DOWN:
        break;
        case RELAX:
            res = false;
        break;
    }
    return res;
}

bool Key_MOUSE_L(void)
{
	static bool res = false;

	KEY_State_Judge(&Key_Info.MOUSE_L ,MOUSE_PRESSED_LEFT, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_MOUSE_L);

	switch(Key_Info.MOUSE_L.State)
	{
	case UP:
	break;
	case PRESS:
	break;
	case SHORT_DOWN:
		res = true;
	break;
	case DOWN:
	break;
	case RELAX:
		res = false;
	break;
	}
	return res;
}
bool Key_MOUSE_R(void)
{
    bool res = false;

    KEY_State_Judge(&Key_Info.MOUSE_R ,MOUSE_PRESSED_RIGH, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_MOUSE_R);

    switch(Key_Info.MOUSE_R.State)
    {
        case UP:
        break;
        case PRESS:
        break;
        case SHORT_DOWN:
            res = true;
        break;
        case DOWN:
        break;
        case RELAX:
            res = false;
        break;
    }
		return res;
}
bool Key_R(void)
{
	static bool res = false;
	static bool single = true;
	  KEY_State_Judge(&Key_Info.R ,KEY_PRESSED_R, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_R);
	  switch(Key_Info.R.State)
	  {
		case UP:
		break;
		case PRESS:
		break;
		case SHORT_DOWN:
				if(single == true)
				{
					if(res == false)
					{
						res = true;
					}else if(res == true)
					{
						res = false;
					}
					single = false;
				}
		break;
		case DOWN:
		break;
		case RELAX:
				single = true;
		break;
	  }
	return res;
}

void Key_Q(void)
{
	static bool res = true;
	KEY_State_Judge(&Key_Info.Q ,KEY_PRESSED_Q, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_Q);
	switch(Key_Info.Q.State)
	{
	case UP:
	break;
	case PRESS:
        res = true;
	break;
	case SHORT_DOWN:
        if(res == true)
        {
						gimbal.target.yaw_Angle += 90.f;
            res = false;
        }
	break;
	case DOWN:
	break;
	case RELAX:
	break;
	}
}
void Key_E(void)
{
    static bool res = true;

    KEY_State_Judge(&Key_Info.E ,KEY_PRESSED_E, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_E);

    switch(Key_Info.E.State)
    {
    case UP:
    break;
    case PRESS:
        res = true;
    break;
    case SHORT_DOWN:
        if(res == true)
        {
            gimbal.target.yaw_Angle -= 90.f;
            res = false;
        }
    break;
    case DOWN:
    break;
    case RELAX:
    break;
    }
}
bool Key_B(void)
{
	static bool res = false;
	static bool single = true;
  KEY_State_Judge(&Key_Info.B ,KEY_PRESSED_B, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_B);
  switch(Key_Info.B.State)
  {
    case UP:
    break;
    case PRESS:
    break;
    case SHORT_DOWN:
			if(single == true)
			{
				if(res == false)
				{
					res = true;
				}else if(res == true)
				{
					res = false;
				}
				single = false;
			}
    break;
    case DOWN:
    break;
    case RELAX:
			single = true;
    break;
  }
	return res;
}
bool Key_C(void)
{
	static bool res = false;
	static bool single = true;
  KEY_State_Judge(&Key_Info.C ,KEY_PRESSED_C, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_C);
  switch(Key_Info.C.State)
  {
    case UP:
    break;
    case PRESS:
    break;
    case SHORT_DOWN:
			if(single == true)
			{
				if(res == false)
				{
					res = true;
				}else if(res == true)
				{
					res = false;
				}
				single = false;
			}
    break;
    case DOWN:
    break;
    case RELAX:
			single = true;
    break;
  }
	return res;
}

