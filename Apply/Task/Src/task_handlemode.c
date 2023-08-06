#include "task_conf.h"
#include "usercode.h"
#include "config.h"

extern PWMInfo_T PWMInfo;

void Task_HandleMode_Process(HandleModeInfo HMInfo)
{
    float Angle = 0.0f;         //�洢�˶��Ƕ� [-180,180]
    float Pro = 0.0f;           //�洢�ƽ����� [0,100]
    float CurrYaw = 0.0f;       //��ǰ���������JY901S
    float ExpYaw = 0.0f;        //���������
    float PIDOut = 0.0;         //PID�����Ľ��

    //�洢�ֱ��������˶��ǶȺ��ƽ�����
    Angle = HMInfo.fNum[0];
    Pro = HMInfo.fNum[1];

    //�趨��ǰ�ǶȺ������Ƕ�
    CurrYaw = JY901S.stcAngle.ConYaw;
    ExpYaw = CurrYaw - Angle;
    //printf("ExpYaw %f\r\n",ExpYaw);

    //����PID����
    PIDOut = Algo_PID_Calculate(&YawPID,CurrYaw,ExpYaw);
    //printf("PIDOut %f\r\n",PIDOut);

    if(Angle >= -90 && Angle <= 90) //����һ���������˶�
    {
        PWMInfo.PWMout[LeftHThruster] =  -PIDOut/2*Pro/100 + STOP_PWM_VALUE + 50;   //����PIDOut���ƽ���
        PWMInfo.PWMout[RightHThruster] =  PIDOut /2*Pro/100 + STOP_PWM_VALUE + 50;   
    }
    else
    {
        if(PIDOut > 0)      //�����������˶�
        {
            //printf("3\r\n");
            PWMInfo.PWMout[LeftHThruster] =  -PIDOut * 1/4 /2* Pro/100 + STOP_PWM_VALUE -50;
            PWMInfo.PWMout[RightHThruster] = -PIDOut * 3/4 /2* Pro/100 + STOP_PWM_VALUE -50;
        }
        else                //�����������˶�
        {
            //printf("4\r\n");
            PWMInfo.PWMout[LeftHThruster] =  PIDOut * 3/4 /2* Pro/100 + STOP_PWM_VALUE -50;
            PWMInfo.PWMout[RightHThruster] = PIDOut * 1/4 /2* Pro/100 + STOP_PWM_VALUE -50;
        }
    }

    //PWM�޷���0.6A
    if(PWMInfo.PWMout[LeftHThruster] < 1350)  PWMInfo.PWMout[LeftHThruster] = 1350;
    if(PWMInfo.PWMout[LeftHThruster] > 1650)  PWMInfo.PWMout[LeftHThruster] = 1650;

    if(PWMInfo.PWMout[RightHThruster] < 1350)  PWMInfo.PWMout[RightHThruster] = 1350;
    if(PWMInfo.PWMout[RightHThruster] > 1650)  PWMInfo.PWMout[RightHThruster] = 1650;

    //����PID����ı�������ˮƽ�ƽ���PWM������1Ϊ��ˮƽ�ƣ�2Ϊ��ˮƽ�ƣ�����1500Ϊǰ������
    Task_Thruster_SpeedSet(LeftHThruster,PWMInfo.PWMout[LeftHThruster]);
    Task_Thruster_SpeedSet(RightHThruster,PWMInfo.PWMout[RightHThruster]);

    //printf("Handle left%d right%d\r\n",PWMInfo.PWMout[LeftHThruster],PWMInfo.PWMout[RightHThruster]);
    printf("T %d %d %d %d\r\n",PWMInfo.PWMout[LeftHThruster],PWMInfo.PWMout[RightHThruster],PWMInfo.PWMout[LeftVThruster],PWMInfo.PWMout[RightVThruster]);
}
