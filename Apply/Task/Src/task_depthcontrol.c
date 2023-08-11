#include "task_conf.h"
#include "usercode.h"
#include "config.h"

extern PWMInfo_T PWMInfo;

/* ��ȿ��ƺ�������Ҫ��ǰ��Ⱥ�������� */
void task_DepthControl_Process(float Curr,float Exp)
{
    float PIDOut = 0.0;         //PID�����Ľ��

    PIDOut = Algo_PID_Calculate(&DepthPID,Curr,Exp);

    //printf("%f",PIDOut);

    //ʵ�����1500msΪ�ϸ�
    PWMInfo.PWMout[LeftVThruster] = -PIDOut + STOP_PWM_VALUE;

    //�޷�
    if(PWMInfo.PWMout[LeftVThruster] < 1350)  PWMInfo.PWMout[LeftVThruster] = 1350;
    if(PWMInfo.PWMout[LeftVThruster] > 1650)  PWMInfo.PWMout[LeftVThruster] = 1650;

    PWMInfo.PWMout[RightVThruster] = PWMInfo.PWMout[LeftVThruster];

    Task_Thruster_SpeedSet(LeftVThruster,PWMInfo.PWMout[LeftVThruster]);
    Task_Thruster_SpeedSet(RightVThruster,PWMInfo.PWMout[RightVThruster]);

}
