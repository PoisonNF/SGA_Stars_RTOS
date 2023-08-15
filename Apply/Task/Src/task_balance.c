#include "task_conf.h"
#include "usercode.h"
#include "config.h"

extern PWMInfo_T PWMInfo;
/* ʹ�����һ�����ƽ�� */
void Task_Balance_Process(void)
{
    float CurrTiltAngle = JY901S.stcAngle.ConPitch;
    float ExpTiltAngle = 0.0f;
    float PIDout;

    PIDout = Algo_PID_Calculate(&BalancePID,CurrTiltAngle,ExpTiltAngle);
    //printf("%f\r\n",PIDout);

    //�����³�ʱ��CurrTiltAngle����0���Ҳ��෴����ֱ�ƽ�������1500ʱ���ϸ�
    PWMInfo.PWMout[LeftVThruster] = -PIDout + STOP_PWM_VALUE;
    PWMInfo.PWMout[RightVThruster] = PIDout + STOP_PWM_VALUE;

    //PWM�޷���0.6A
    if(PWMInfo.PWMout[LeftVThruster] < 1350)  PWMInfo.PWMout[LeftVThruster] = 1350;
    if(PWMInfo.PWMout[LeftVThruster] > 1650)  PWMInfo.PWMout[LeftVThruster] = 1650;

    if(PWMInfo.PWMout[RightVThruster] < 1350)  PWMInfo.PWMout[RightVThruster] = 1350;
    if(PWMInfo.PWMout[RightVThruster] > 1650)  PWMInfo.PWMout[RightVThruster] = 1650;

    Task_Thruster_SpeedSet(LeftVThruster,PWMInfo.PWMout[LeftVThruster]);
    Task_Thruster_SpeedSet(RightVThruster,PWMInfo.PWMout[RightVThruster]);

    //printf("%d %d\r\n",PWMInfo.PWMout[LeftVThruster],PWMInfo.PWMout[RightVThruster]);
}

