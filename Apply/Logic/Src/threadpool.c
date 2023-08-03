#include "usercode.h"		/* usercodeͷ�ļ� */
#include "threadpool.h"		/* threadpoolͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */


/* �߳���ں�����ʹ��������Դ��ļ��� */
/* ��ȡ��λ�������߳� */
void DataFromIPC(void* paramenter)
{
    uint8_t ReceBuf[100];
    uint8_t ReceNum = 0;

    while(1)
    {
        if(rt_sem_take(DataFromIPC_Sem,RT_WAITING_FOREVER) == RT_EOK)
        {
            //ƽʱ����ʹ�ô���1����ʽʹ�ò��ô���3
            ReceNum = Drv_Uart_Receive_DMA(&Uart1,ReceBuf);
            if(ReceNum != 0)
            {
                //printf("%s\r\n",ReceBuf);
                //��������
                Task_AnalysisData(ReceBuf);

                //����ʱ������0
                memset(ReceBuf,0,ReceNum);
                ReceNum = 0;
            }
        }
        rt_thread_yield();
    }
}

/* ��ȡJY901S�����߳� */
void JY901SReadThread(void* paramenter)
{
    while(1)
    {
        //�����ȡ���ź�����˵�����յ�����
		if(rt_sem_take(JY901S_Sem,RT_WAITING_FOREVER) == RT_EOK)
		{
            //����ɹ������������
			if(OCD_JY901_DataProcess(&JY901S))
            {
                //����ת��
                OCD_JY901_DataConversion(&JY901S);
                //��ӡŷ����
                if(JY901S.tConfig.usType & JY901_OUTPUT_ANGLE)	    
                    printf("J Angle %.3f %.3f %.3f\r\n",
                            JY901S.stcAngle.ConRoll,
                            JY901S.stcAngle.ConPitch,
                            JY901S.stcAngle.ConYaw);
            }
		}
		rt_thread_yield();
    }
}

/* ��ȡMS5837�����߳� */
void MS5837ReadThread(void* paramenter)
{
    while(1)
    {
        OCD_MS5837_GetData(&MS5837);
        printf("M %f\r\n",MS5837.fDepth);
        Drv_Delay_Ms(1000);
        rt_thread_yield();
    }
}

/* �ֱ������߳� */
void MODE_HANDLE(void* paramenter)
{
    uint8_t buf[15];
    float DataBuf[4];

    while(1)
    {
        //�ֱ�������Ϣ���н��յ����ݣ����л����Զ�ģʽ
        if(rt_mq_recv(HandleModemq,buf,sizeof(buf),RT_WAITING_NO) == RT_EOK)
        {
            if(!strcmp((char *)buf,"AUTO START"))
            {
                printf("Switch to AUTO Mode\r\n");
                rt_enter_critical();                    //����������
                rt_thread_suspend(rt_thread_self());    //�����߳�
                rt_thread_resume(thread5);              //�ָ��Զ������߳�
                rt_exit_critical();                     //����������
                rt_schedule();                          //����ִ��һ�ε���
            }
        }

        //���Ի�ȡ�ź���������ȡ����˵�����ֱ�����
        if(rt_sem_trytake(JSData_Sem) == RT_EOK)
        {
            memset(DataBuf,0,4);
            DataBuf[0] = fNum[0];
            DataBuf[1] = fNum[1];
            printf("%f %f\r\n",DataBuf[0],DataBuf[1]);
        }
        // printf("HANDLE\r\n");
        
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
        rt_thread_yield();
    }
}

/* Ѳ��ģʽ�߳� */
void MODE_AUTO(void* paramenter)
{
    uint8_t buf[15];

    //Ĭ�Ϲ����Զ�ģʽ
    rt_thread_suspend(rt_thread_self());
    rt_schedule();
    while(1)
    {
        //�Զ�������Ϣ���н��յ����ݣ����л����ֱ�ģʽ
        if(rt_mq_recv(AutoModemq,buf,sizeof(buf),RT_WAITING_NO) == RT_EOK)
        {
            if(!strcmp((char *)buf,"HANDLE START"))
            {
                printf("Switch to HANDLE Mode\r\n");
                rt_enter_critical();                    //����������
                rt_thread_suspend(rt_thread_self());    //�����߳�
                rt_thread_resume(thread4);              //�ָ��ֶ������߳�
                rt_exit_critical();                     //����������
                rt_schedule();                          //����ִ��һ�ε���
            }
        }
        printf("AUTO\r\n");
        Drv_Delay_Ms(1000);
        rt_thread_yield();
    }
}

/* �����߳� */
void TestThread(void* paramenter)
{
    while(1)
    {
        Drv_Delay_Ms(1000);
        rt_thread_yield();
    }
}



