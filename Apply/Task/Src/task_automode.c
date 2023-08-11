#include "task_conf.h"
#include "usercode.h"
#include "config.h"

extern PWMInfo_T PWMInfo;

void Task_AutoMode_Process(AutoModeInfo AMInfo)
{
    float Angle = 0.0f;         //�洢�˶��Ƕ� [-180,180]
    float CurrYaw = 0.0f;       //��ǰ���������JY901S
    float ExpYaw = 0.0f;        //���������
    float PIDOut = 0.0;         //PID�����Ľ��
    float Pro = 50;
    float temp;

    //�洢���߽Ƕ�
    Angle = AMInfo.BlackAngle;

    //�趨��ǰ�ǶȺ������Ƕ�
    CurrYaw = JY901S.stcAngle.ConYaw;
    ExpYaw = CurrYaw - Angle;
    //printf("ExpYaw %f\r\n",ExpYaw);

    //Ѳ��PID����
    PIDOut = Algo_PID_Calculate(&LinePatrolPID,CurrYaw,ExpYaw);
    //printf("PIDOut %f\r\n",PIDOut);

    //����1500usΪǰ������
    if(Angle >= -90 && Angle <= 90) //����һ���������˶�
    {
        PWMInfo.PWMout[LeftHThruster] =  -PIDOut*Pro/100 + STOP_PWM_VALUE + 100;
        PWMInfo.PWMout[RightHThruster] =  PIDOut*Pro/100 + STOP_PWM_VALUE + 100;

        if(Angle < 0 && Angle >= -90)
        {
            if(PWMInfo.PWMout[LeftHThruster] > PWMInfo.PWMout[RightHThruster])
            {
                temp = PWMInfo.PWMout[LeftHThruster];
                PWMInfo.PWMout[LeftHThruster] = PWMInfo.PWMout[RightHThruster];
                PWMInfo.PWMout[RightHThruster] = temp;   
            }
        }
        else
        {
            if(PWMInfo.PWMout[LeftHThruster] < PWMInfo.PWMout[RightHThruster])
            {
                temp = PWMInfo.PWMout[LeftHThruster];
                PWMInfo.PWMout[LeftHThruster] = PWMInfo.PWMout[RightHThruster];
                PWMInfo.PWMout[RightHThruster] = temp;   
            }
        }
    }
    
    if(Angle == 0)
    {
        PWMInfo.PWMout[LeftHThruster] = 1550;
        PWMInfo.PWMout[RightHThruster] = 1550;
    }

    //PWM�޷���0.6A
    if(PWMInfo.PWMout[LeftHThruster] < 1350)  PWMInfo.PWMout[LeftHThruster] = 1350;
    if(PWMInfo.PWMout[LeftHThruster] > 1650)  PWMInfo.PWMout[LeftHThruster] = 1650;

    if(PWMInfo.PWMout[RightHThruster] < 1350)  PWMInfo.PWMout[RightHThruster] = 1350;
    if(PWMInfo.PWMout[RightHThruster] > 1650)  PWMInfo.PWMout[RightHThruster] = 1650;

    //����PID����ı�������ˮƽ�ƽ���PWM������1Ϊ��ˮƽ�ƣ�2Ϊ��ˮƽ�ƣ�����1500Ϊǰ������
    Task_Thruster_SpeedSet(LeftHThruster,PWMInfo.PWMout[LeftHThruster]);
    Task_Thruster_SpeedSet(RightHThruster,PWMInfo.PWMout[RightHThruster]);

    //printf("Auto left%d right%d\r\n",PWMInfo.PWMout[LeftHThruster],PWMInfo.PWMout[RightHThruster]);
}
