#include "motor.h"
#include "imu_Task.h"

Critical_t gimbal_yaw_angle;

DJI_MOTOR Gimbal[MOTOR_NUM]={
    [YAW] = {
                .type=_6020,
                .usage=YAW,
                .CANx=CAN2,
                .Data.StdId=0x205,
                .txMsg=&CAN_TxMsg[_CAN2][_0x1ff],
            },

    [PITCH]={
                .type=_6020,
                .usage=PITCH,
                .CANx=CAN1,
                .Data.StdId=0x206,
                .txMsg=&CAN_TxMsg[_CAN1][_0x1ff],
            },

    [SHOOTL]={
                .type=_3508,
                .usage=SHOOTL,
                .CANx=CAN1,
                .Data.StdId=0x202,
                .txMsg=&CAN_TxMsg[_CAN1][_0x200],
            },

    [SHOOTR]={
                .type=_3508,
                .usage=SHOOTR,
                .CANx=CAN1,
                .Data.StdId=0x201,
                .txMsg=&CAN_TxMsg[_CAN1][_0x200],
            },
            
    [TRIGGER]={
                .type=_2006,
                .usage=TRIGGER,
                .CANx=CAN1,
                .Data.StdId=0x203,
                .txMsg=&CAN_TxMsg[_CAN1][_0x200],
            },
};



void get_Motor_Data(CAN_TypeDef* CANx,CanRxMsg* RxMsg,DJI_MOTOR* DJI_Motor)
{
//检查CAN端口是否正确
    if(CANx!=DJI_Motor->CANx)return ;

    if(RxMsg->StdId==DJI_Motor->Data.StdId){
    //基础信息    
        DJI_Motor->Data.temperature = RxMsg->Data[6];
        DJI_Motor->Data.encoder  = ((int16_t)RxMsg->Data[0]<<8 | (int16_t)RxMsg->Data[1]); 
        DJI_Motor->Data.velocity = ((int16_t)RxMsg->Data[2]<<8 | (int16_t)RxMsg->Data[3]);
        DJI_Motor->Data.current  = ((int16_t)RxMsg->Data[4]<<8 | (int16_t)RxMsg->Data[5]);
        
    //按电机类型做特殊处理
        switch (DJI_Motor->type)
        {
        case _6020:
            DJI_Motor->Data.angle = (DJI_Motor->Data.encoder/8192.f*360.f);
						if(DJI_Motor->usage==YAW)DJI_Motor->Data.angle = Gimbal_Yaw_Gryo_AngleSum(&gimbal_yaw_angle,(DJI_Motor->Data.encoder/8192.f*360.f));
            break;

        case _2006:
            DJI_Motor->Data.angle = encoder2Angle(DJI_Motor->usage,DJI_Motor->Data.encoder,36.0f);
            break;

        case _3508:
            DJI_Motor->Data.angle = encoder2Angle(DJI_Motor->usage,DJI_Motor->Data.encoder,3591.f/187.f);
            break;

        default:
            break;
        }
    }
}


static double encoder2Angle(MOTOR_USAGE motor,int16_t act_Encoder,float torque_Ratio)
{
    static int32_t angle_Last[MOTOR_NUM];
    static int32_t result_Encoder[MOTOR_NUM];
    int res1,res2;
    if(act_Encoder<angle_Last[motor]){
        res1=act_Encoder-angle_Last[motor]+8192;
        res2=act_Encoder-angle_Last[motor];
    }else{
        res1=act_Encoder-angle_Last[motor]-8192;
        res2=act_Encoder-angle_Last[motor];
    }
    angle_Last[motor]=act_Encoder;
    ABS(res1)<ABS(res2) ? (result_Encoder[motor]+=res1) : (result_Encoder[motor]+=res2);
	if(motor == _3508){
		ABS(result_Encoder[motor]) > 8192*torque_Ratio ? (result_Encoder[motor]-result_Encoder[motor]/ABS(result_Encoder[motor])*8192*torque_Ratio): result_Encoder[motor];
	}
	return result_Encoder[motor]/(8192*torque_Ratio)*360.0f;
}


