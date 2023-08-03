#include "task_conf.h"
#include "usercode.h"
#include "config.h"

static char AnalysisData[5][15];
static int AnalysisNum = 0;
float fNum[4];

/* ��������λ�������յ������� */
void Task_AnalysisData(uint8_t *DataBuf)
{
    //��������0
    memset(AnalysisData,0,sizeof(AnalysisData));
    memset(fNum,0,4);
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
    //PIDֵ
    if(!strcmp(AnalysisData[0],"PID"))
    {
        //��Ȼ�PID
        if(!strcmp(AnalysisData[1],"DepthPID"))
        {
            fNum[0] = strtof(AnalysisData[2],NULL);
            fNum[1] = strtof(AnalysisData[3],NULL);
            fNum[2] = strtof(AnalysisData[4],NULL);
            printf("%f %f %f\r\n",fNum[0],fNum[1],fNum[2]);

            //������Ȼ�PID
        }
        //����PID
        else if(!strcmp(AnalysisData[1],"YawPID"))
        {
            fNum[0] = strtof(AnalysisData[2],NULL);
            fNum[1] = strtof(AnalysisData[3],NULL);
            fNum[2] = strtof(AnalysisData[4],NULL);
            printf("%f %f %f\r\n",fNum[0],fNum[1],fNum[2]);

            //��������PID
        }
        //Ѳ�߻�PID
        else if(!strcmp(AnalysisData[1],"LinePatrolPID"))
        {
            fNum[0] = strtof(AnalysisData[2],NULL);
            fNum[1] = strtof(AnalysisData[3],NULL);
            fNum[2] = strtof(AnalysisData[4],NULL);
            printf("%f %f %f\r\n",fNum[0],fNum[1],fNum[2]);

            //����Ѳ�߻�PID
        }
    }
    //�ֱ�ҡ��ֵ
    else if(!strcmp(AnalysisData[0],"JSV"))
    {
        fNum[0] = strtof(AnalysisData[1],NULL);
        fNum[1] = strtof(AnalysisData[2],NULL);
        //printf("%f %f\r\n",fNum[0],fNum[1]);
        //printf("%x\r\n",fNum);
        //�����ݷ��͸��ֱ������߳�
        //rt_mq_send(HandleModemq,fNum,sizeof(fNum));
        rt_sem_release(JSData_Sem);
    }
    //�ֱ�����״̬
    else if(!strcmp(AnalysisData[0],"JSB"))
    {
        fNum[0] = strtof(AnalysisData[1],NULL);
        if(!strcmp(AnalysisData[2],"Press"))
        {
            fNum[1] = 1;
            //printf("%d %d\r\n",(uint8_t)fNum[0],1);
        }
        else
        {
            fNum[1] = 0;
            //printf("%d %d\r\n",(uint8_t)fNum[0],0);
        }
        //�����ݷ��͸��ֱ������߳�
        rt_sem_release(JSData_Sem);
    }

    //��������������
    else if(!strcmp(AnalysisData[0],"C"))
    {
        printf("%s\r\n",AnalysisData[1]);

        //�����ݷ��͸��ֱ������߳�
    }

    //ģʽ�л�����
    else if(!strcmp(AnalysisData[0],"MODE"))
    {
        //printf("%s\r\n",AnalysisData[1]);
        if(!strcmp(AnalysisData[1],"AUTO"))
        {
            //�����ֱ�����ģʽ�������Զ�Ѳ��ģʽ
            rt_mq_send(HandleModemq,"AUTO START",sizeof("AUTO START"));
        }
        else if(!strcmp(AnalysisData[1],"HANDLE"))
        {
            //�����Զ�Ѳ��ģʽ�������ֱ�ģʽ
            rt_mq_send(AutoModemq,"HANDLE START",sizeof("HANDLE START"));
        }
    }
}