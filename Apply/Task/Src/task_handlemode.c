#include "task_conf.h"
#include "usercode.h"
#include "config.h"

void Task_HandleMode_Process(HandleModeInfo HMInfo)
{
    float Angle = 0.0f;         //�洢�˶��Ƕ� [-180,180]
    float Pro = 0.0f;           //�洢�ƽ����� [0,100]
    float CurrYaw = 0.0f;       //��ǰ���������JY901S
    float ExpYaw = 0.0f;        //���������
    float PIDOut = 0.0;         //PID�����Ľ��

    uint16_t leftPWM;
    uint16_t rightPWM;

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
        leftPWM =  -PIDOut/2*Pro/100 + STOP_PWM_VALUE + 50;   //����PIDOut���ƽ���
        rightPWM =  PIDOut /2*Pro/100 + STOP_PWM_VALUE + 50;   
    }
    else
    {
        if(PIDOut > 0)      //�����������˶�
        {
            //printf("3\r\n");
            leftPWM =  -PIDOut * 1/4 /2* Pro/100 + STOP_PWM_VALUE -50;
            rightPWM = -PIDOut * 3/4 /2* Pro/100 + STOP_PWM_VALUE -50;
        }
        else                //�����������˶�
        {
            //printf("4\r\n");
            leftPWM =  PIDOut * 3/4 /2* Pro/100 + STOP_PWM_VALUE -50;
            rightPWM = PIDOut * 1/4 /2* Pro/100 + STOP_PWM_VALUE -50;
        }
    }

    //����PID����ı�������ˮƽ�ƽ���PWM������1Ϊ��ˮƽ�ƣ�2Ϊ��ˮƽ�ƣ�����1500Ϊǰ������
    Task_Thruster_SpeedSet(1,leftPWM);
    Task_Thruster_SpeedSet(2,rightPWM);

    printf("Handle left%d right%d\r\n",leftPWM,rightPWM);
}
