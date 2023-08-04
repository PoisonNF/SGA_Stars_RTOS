#include "task_conf.h"
#include "usercode.h"
#include "config.h"

static char AnalysisData[5][15];
static int AnalysisNum = 0;

/* ��������λ�������յ������� */
void Task_AnalysisData(uint8_t *DataBuf)
{
    //��������0
    memset(AnalysisData,0,sizeof(AnalysisData));
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
    //PIDֵ
    if(!strcmp(AnalysisData[0],"PID"))
    {
        //��Ȼ�PID
        if(!strcmp(AnalysisData[1],"DepthPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);

            //������Ȼ�PID
            Algo_PID_Update(&DepthPID,HMInfo.fNum);
            printf("DepthPID %f %f %f\r\n",DepthPID.fKp,DepthPID.fKi,DepthPID.fKd);
        }
        //����PID
        else if(!strcmp(AnalysisData[1],"YawPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);

            //��������PID
            Algo_PID_Update(&YawPID,HMInfo.fNum);
            printf("YawPID %f %f %f\r\n",YawPID.fKp,YawPID.fKi,YawPID.fKd);
        }
        //Ѳ�߻�PID
        else if(!strcmp(AnalysisData[1],"LinePatrolPID"))
        {
            HMInfo.fNum[0] = strtof(AnalysisData[2],NULL);
            HMInfo.fNum[1] = strtof(AnalysisData[3],NULL);
            HMInfo.fNum[2] = strtof(AnalysisData[4],NULL);
            
            //����Ѳ�߻�PID
            Algo_PID_Update(&LinePatrolPID,HMInfo.fNum);
            printf("LinePatrolPID %f %f %f\r\n",LinePatrolPID.fKp,LinePatrolPID.fKi,LinePatrolPID.fKd);
        }
    }
    //�ֱ�ҡ��ֵ
    else if(!strcmp(AnalysisData[0],"JSV"))
    {
        HMInfo.fNum[0] = strtof(AnalysisData[1],NULL);
        HMInfo.fNum[1] = strtof(AnalysisData[2],NULL);
        //printf("%f %f\r\n",HMInfo.fNum[0],HMInfo.fNum[1]);
        //printf("%x\r\n",fNum);

        //�����ݷ��͸��ֱ������߳�
        rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
    }
    //�ֱ�����״̬
    else if(!strcmp(AnalysisData[0],"JSB"))
    {
        HMInfo.fNum[0] = strtof(AnalysisData[1],NULL);
        if(!strcmp(AnalysisData[2],"Press"))
        {
            HMInfo.fNum[1] = 1; //��1����Press
            //printf("%d %d\r\n",(uint8_t)HMInfo.fNum[0],1);
        }
        else
        {
            HMInfo.fNum[1] = 0; //��0����Release
            //printf("%d %d\r\n",(uint8_t)HMInfo.fNum[0],0);
        }
        //�����ݷ��͸��ֱ������߳�
        rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
    }

    //ģʽ�л�����
    else if(!strcmp(AnalysisData[0],"MODE"))
    {
        //printf("%s\r\n",AnalysisData[1]);
        if(!strcmp(AnalysisData[1],"AUTO"))
        {
            //�����ֱ�����ģʽ�������Զ�Ѳ��ģʽ
            memcpy(HMInfo.ModeChange,"AUTO START",sizeof("AUTO START"));
            rt_mq_send(HandleModemq,&HMInfo,sizeof(HandleModeInfo));
        }
        else if(!strcmp(AnalysisData[1],"HANDLE"))
        {
            //�����Զ�Ѳ��ģʽ�������ֱ�ģʽ
            memcpy(AMInfo.ModeChange,"HANDLE START",sizeof("HANDLE START"));
            rt_mq_send(AutoModemq,&AMInfo,sizeof(AutoModeInfo));              
        }
    }

    //�Զ�Ѳ��ģʽ�Ƕ�ֵ
    else if(!strcmp(AnalysisData[0],"LP"))
    {
        AMInfo.BlackAngle = strtof(AnalysisData[1],NULL);
        rt_mq_send(AutoModemq,&AMInfo,sizeof(AutoModeInfo));
    }
}

