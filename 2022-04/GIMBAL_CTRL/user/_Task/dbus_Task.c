/*接收机机任务

*/

#include "dbus_Task.h"


/*--------------------------   变量声明   ------------------------------*/


/*--------------------------   变量声明   ------------------------------*/

//调度任务
void DBUS_TASK(void *args)
{
    uint32_t currentTime;

	while(1)
	{
		currentTime = xTaskGetTickCount();
        if(currentTime-rc_ctrl.update_Time>50){//连接超时
            gimbal.state_Setup(OFF);
            shoot.state_Setup(OFF);
            cover.state_Setup(OFF);
            chassis.state_Setup(OFF);
			chassis.mode_Setup(INVA);
        }
        else
        {
							switch (rc_ctrl.rc.s[0])//右档选项
							{
	/*------------------------------------------------*/ 
							case 1://右上（云台部分
											switch (rc_ctrl.rc.s[1])//左档选项
											{
											case 1://左上(全拨上，整车电机关闭卸力)
													gimbal.state_Setup(OFF);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(INVA);
													break;
											
											case 3://左中
													gimbal.state_Setup(ON);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(INVA);
													break;
											
											case 2://左下
													gimbal.state_Setup(ON);
													shoot.state_Setup(ON);
													cover.state_Setup(ON);
													chassis.state_Setup(ON);
													chassis.mode_Setup(INVA);
													break;
											
											default:
													break;
											}
									break;
	/*------------------------------------------------*/                    
							case 3://右中
											switch (rc_ctrl.rc.s[1])//左档选项
											{
											case 1://左上
													gimbal.state_Setup(OFF);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(INVA);
													break;
											
											case 3://左中
													gimbal.state_Setup(ON);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(FOLO);
													break;
											
											case 2://左下
													gimbal.state_Setup(ON);
													shoot.state_Setup(ON);
													cover.state_Setup(ON);
													chassis.state_Setup(ON);
													chassis.mode_Setup(FOLO);
													break;
											
											default:
													break;
											}
									break;
	/*------------------------------------------------*/                    
							case 2://右下
											switch (rc_ctrl.rc.s[1])//左档选项
											{
											case 1://左上
													gimbal.state_Setup(OFF);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(FOLO);
													break;
											
											case 3://左中
													gimbal.state_Setup(ON);
													shoot.state_Setup(OFF);
													cover.state_Setup(OFF);
													chassis.state_Setup(OFF);
													chassis.mode_Setup(SPIN);
													break;
											
											case 2://左下
													gimbal.state_Setup(ON);
													shoot.state_Setup(ON);
													cover.state_Setup(ON);
													chassis.state_Setup(ON);
													chassis.mode_Setup(SPIN);
													break;
											
											default:
													break;
											}
									break;

							default:
									break;
							}
					}
        vTaskDelayUntil(&currentTime, DBUS_TASK_TIM);
    }
}

