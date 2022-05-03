#include "rc.h"

KEYBOARD_INFO_T Key_Info;

void RC_Init(uint8_t *rx1_buf, uint8_t *rx2_buf, uint16_t dma_buf_num)
{
	/* -------------- Enable Module Clock Source ----------------------------*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1); //PB7  usart1 rx

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_DeInit(USART1);

	USART_InitStructure.USART_BaudRate = 100000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);

	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_IDLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	USART_Cmd(USART1, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_InitTypeDef DMA_InitStructure;
	DMA_DeInit(DMA2_Stream2);

	DMA_InitStructure.DMA_Channel = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (USART1->DR);
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
	DMA_Init(DMA2_Stream2, &DMA_InitStructure);
	DMA_DoubleBufferModeConfig(DMA2_Stream2, (uint32_t)rx2_buf, DMA_Memory_0);
	DMA_DoubleBufferModeCmd(DMA2_Stream2, ENABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE); //Add a disable
	DMA_Cmd(DMA2_Stream2, ENABLE);
}

void RC_unable(void)
{
	USART_Cmd(USART1, DISABLE);
}
void RC_restart(uint16_t dma_buf_num)
{
	USART_Cmd(USART1, DISABLE);
	DMA_Cmd(DMA2_Stream2, DISABLE);
	DMA_SetCurrDataCounter(DMA2_Stream2, dma_buf_num);

	USART_ClearFlag(USART1, USART_FLAG_IDLE);

	DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2);
	DMA_ClearITPendingBit(DMA2_Stream2, DMA_IT_TCIF2);
	DMA_Cmd(DMA2_Stream2, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


//遥控器控制变量 
RC_ctrl_t rc_ctrl;
//接收原始数据，为18个字节，给了36个字节长度，防止DMA传输越界
uint8_t SBUS_rx_buf[2][SBUS_RX_BUF_NUM];

//初始化DMA，串口1
void remote_control_init(void)
{
    RC_Init(SBUS_rx_buf[0], SBUS_rx_buf[1], SBUS_RX_BUF_NUM);
}


//串口1中断
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ReceiveData(USART1);
    }
    else if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
        static uint16_t this_time_rx_len = 0;
        USART_ReceiveData(USART1);

        if(DMA_GetCurrentMemoryTarget(DMA2_Stream2) == 0)
        {
			//重新设置DMA
			DMA_Cmd(DMA2_Stream2, DISABLE);
			this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
			DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
			DMA2_Stream2->CR |= DMA_SxCR_CT;
			//清DMA中断标志
			DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
			DMA_Cmd(DMA2_Stream2, ENABLE);
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
            DMA_Cmd(DMA2_Stream2, DISABLE);
            this_time_rx_len = SBUS_RX_BUF_NUM - DMA_GetCurrDataCounter(DMA2_Stream2);
            DMA_SetCurrDataCounter(DMA2_Stream2, SBUS_RX_BUF_NUM);
            DMA2_Stream2->CR &= ~(DMA_SxCR_CT);
            //清DMA中断标志
            DMA_ClearFlag(DMA2_Stream2, DMA_FLAG_TCIF2 | DMA_FLAG_HTIF2);
            DMA_Cmd(DMA2_Stream2, ENABLE);
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
		
    /* Channel 0, 1, 2, 3, 4 */
    rc_ctrl->rc.ch[0] = (  sbus_buf[0]       | (sbus_buf[1] << 8 ) ) & 0x07ff;                            //!< Channel 0
    rc_ctrl->rc.ch[1] = ( (sbus_buf[1] >> 3) | (sbus_buf[2] << 5 ) ) & 0x07ff;                            //!< Channel 1
    rc_ctrl->rc.ch[2] = ( (sbus_buf[2] >> 6) | (sbus_buf[3] << 2 ) | (sbus_buf[4] << 10) ) & 0x07ff;      //!< Channel 2
    rc_ctrl->rc.ch[3] = ( (sbus_buf[4] >> 1) | (sbus_buf[5] << 7 ) ) & 0x07ff;                            //!< Channel 3
    rc_ctrl->rc.ch[4] = ( (sbus_buf[16]    ) | (sbus_buf[17]<< 8 ) ) & 0x07ff;                                  //!< Channel 4

    rc_ctrl->rc.ch[0] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[1] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[2] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[3] -= RC_CH_VALUE_OFFSET;
    rc_ctrl->rc.ch[4] -= RC_CH_VALUE_OFFSET;

	/* Switch left, right */	
    rc_ctrl->rc.s[0] = ((sbus_buf[5] >> 4) & 0x0003);                       //!< Switch left
    rc_ctrl->rc.s[1] = ((sbus_buf[5] >> 4) & 0x000C) >> 2;                  //!< Switch right
		
	/* Mouse axis: X, Y, Z */	
    rc_ctrl->mouse.x = sbus_buf[6] | (sbus_buf[7] << 8);                    //!< Mouse X axis
    rc_ctrl->mouse.y = sbus_buf[8] | (sbus_buf[9] << 8);                    //!< Mouse Y axis
    rc_ctrl->mouse.z = sbus_buf[10] | (sbus_buf[11] << 8);                  //!< Mouse Z axis
		
	/* Mouse Left, Right Is Press  */
    rc_ctrl->mouse.press_l = sbus_buf[12];                                  //!< Mouse Left Is Press 
    rc_ctrl->mouse.press_r = sbus_buf[13];                                  //!< Mouse Right Is Press 
		
	/* KeyBoard value */	
    rc_ctrl->key.v = sbus_buf[14] | (sbus_buf[15] << 8);                    //!< KeyBoard value
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
    key_set_Info->prev_KEY_PRESS = KEY_PRESS;
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

void Key_G(void)
{

	KEY_State_Judge(&Key_Info.G ,KEY_PRESSED_G, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_G);

	switch(Key_Info.G.State)
	{
	case UP:
	break;
	case PRESS:
	break;
	case SHORT_DOWN:
	break;
	case DOWN:
	break;
	case RELAX:
	break;
	}
}
void Key_V(void)
{
	KEY_State_Judge(&Key_Info.V ,KEY_PRESSED_V, SHORT_CHANGE_TIM , LONG_CHANGE_TIM_V);

	switch(Key_Info.V.State)
	{
	case UP:
	break;
	case PRESS:
	break;
	case SHORT_DOWN:

	break;
	case DOWN:
	break;
	case RELAX:
	break;
	}
}
