#include "task_conf.h"
#include "usercode.h"
#include "config.h"

static char AnalysisData[5][15];
static int AnalysisNum = 0;

/* ��������λ�������յ������� */
void Task_AnalysisData(uint8_t *DataBuf)
{
    //��������0
    rt_memset(AnalysisData,0,sizeof(AnalysisData));
    HandleModeInfo HMInfo = {" ",0,0,0};
    AutoModeInfo AMInfo = {" ",0};
    AnalysisNum = 0;

    //���ݸ�ʽ�з��ַ�����ŵ���ά������
    AnalysisNum = sscanf((char *)DataBuf,"%s %s %s %s %s",
                        AnalysisData[0],
                        AnalysisData[1],
                        AnalysisData[2],
                        AnalysisData[3],
                        AnalysisData[4]);
    //printf("%d ,%s,%s,%s,%s,%s\r\n",AnalysisNum,AnalysisData[0],AnalysisData[1],AnalysisData[2],AnalysisData[3],AnalysisData[4]);

    if(!AnalysisNum) return;
    /* ����֡ͷѡ������ */

    //�ֱ�ҡ��ֵ
    else if(!rt_strcmp(AnalysisData[0],"JSV"))
    {
        HMInfo.fNum[0] = strtof(AnalysisData[1],NULL);
        HMInfo.fNum[1] = strtof(AnalysisData[2],NULL);
        //printf("%f %f\r\n",HMInfo.fNum[0],HMInfo.fNum[1]);
        //printf("%x\r\n",fNum);

        //�����ݷ��͸��ֱ������߳�
        rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
    }

    //�ֱ�����״̬
    else if(!rt_strcmp(AnalysisData[0],"JSB"))
    {
        uint8_t key = strtod(AnalysisData[1],NULL);

        if(!rt_strcmp(AnalysisData[2],"Press"))
        {
            HMInfo.keyPressed = true;//��1����Press
        }
        else
        {
            HMInfo.keyPressed = false; //��0����Release
        }

        switch(key)
        {
            //����
            case 0: 
                HMInfo.fNum[0] = 180.0f;
                HMInfo.fNum[1] = 100.0f;
                break;
            //��ת
            case 1:
                HMInfo.fNum[0] = 90.0f;
                HMInfo.fNum[1] = 100.0f;
                break;
            //��ת
            case 2:
                HMInfo.fNum[0] = -90.0f;
                HMInfo.fNum[1] = 100.0f;
                break;
            //ǰ��
            case 3:
                HMInfo.fNum[0] = 0.0f;
                HMInfo.fNum[1] = 100.0f;
                break;
            default:;
        }
        //�����ݷ��͸��ֱ������߳�
        rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
    }

    //�Զ�Ѳ��ģʽ�Ƕ�ֵ
    else if(!rt_strcmp(AnalysisData[0],"LP"))
    {
        AMInfo.BlackAngle = strtof(AnalysisData[1],NULL);
        rt_mq_send(AutoModemq,&AMInfo,sizeof(AutoModeInfo));
    }

    //ģʽ�л�����
    else if(!rt_strcmp(AnalysisData[0],"MODE"))
    {
        //printf("%s\r\n",AnalysisData[1]);
        if(!rt_strcmp(AnalysisData[1],"AUTO"))
        {
            //�����ֱ�����ģʽ�������Զ�Ѳ��ģʽ
            rt_memcpy(HMInfo.ModeChange,"AUTO START",sizeof("AUTO START"));
            rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
        }
        else if(!rt_strcmp(AnalysisData[1],"HANDLE"))
        {
            //�����Զ�Ѳ��ģʽ�������ֱ�ģʽ
            rt_memcpy(AMInfo.ModeChange,"HANDLE START",sizeof("HANDLE START"));
            rt_mq_send(AutoModemq,&AMInfo,sizeof(AutoModeInfo));              
        }
    }

    //PIDֵ
    else if(!rt_strcmp(AnalysisData[0],"PID"))
    {
        //��Ȼ�PID
        if(!rt_strcmp(AnalysisData[1],"DepthPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);

            //������Ȼ�PID
            Algo_PID_Update(&DepthPID,HMInfo.fNum);
            printf("DepthPID %.2f %.2f %.2f\r\n",DepthPID.fKp,DepthPID.fKi,DepthPID.fKd);
        }
        //����PID
        else if(!rt_strcmp(AnalysisData[1],"YawPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);

            //��������PID
            Algo_PID_Update(&YawPID,HMInfo.fNum);
            printf("YawPID %.2f %.2f %.2f\r\n",YawPID.fKp,YawPID.fKi,YawPID.fKd);
        }
        //Ѳ�߻�PID
        else if(!rt_strcmp(AnalysisData[1],"LinePatrolPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);
            
            //����Ѳ�߻�PID
            Algo_PID_Update(&LinePatrolPID,HMInfo.fNum);
            printf("LinePatrolPID %.2f %.2f %.2f\r\n",LinePatrolPID.fKp,LinePatrolPID.fKi,LinePatrolPID.fKd);
        }
    }
}

