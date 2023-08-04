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
            //ƽʱ����ʹ�ô���1����ʽʹ�ò��ô���3,�޸�PRINTF_UART�꼴��
            if(PRINTF_UART == USART1)
                ReceNum = Drv_Uart_Receive_DMA(&Uart1,ReceBuf);
            else if(PRINTF_UART == USART3)
                ReceNum = Drv_Uart_Receive_DMA(&Uart3,ReceBuf);
            
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
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
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
    HandleModeInfo HMInfo;
    AutoModeInfo ClearBuf;

    while(1)
    {
        //�ֱ�������Ϣ���н��յ����ݣ����л����Զ�ģʽ
        if(rt_mq_recv(HandleModemq,&HMInfo,sizeof(HandleModeInfo),RT_WAITING_NO) == RT_EOK)
        {
            if(!strcmp(HMInfo.ModeChange,"AUTO START"))
            {
                //��AutoModemq��Ϣ�����������������
                while(1)
                {
                    if(rt_mq_recv(AutoModemq,&ClearBuf,sizeof(AutoModeInfo),RT_WAITING_NO) != RT_EOK)
                        break;
                }
                printf("Switch to AUTO Mode\r\n");
                rt_enter_critical();                    //����������
                rt_thread_suspend(rt_thread_self());    //�����߳�
                rt_thread_resume(thread5);              //�ָ��Զ������߳�
                rt_exit_critical();                     //����������
                rt_schedule();                          //����ִ��һ�ε���
            }
            else
            {
                Task_HandleMode_Process(HMInfo);    //�ֱ�����ģʽ������
                Drv_Delay_Ms(500);  //ִ��ʱ������λ���ֱ�����һ֡����ʱ����ͬ
            }    
        }
        // printf("HANDLE\r\n");
        
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
        rt_thread_yield();
    }
}

/* Ѳ��ģʽ�߳� */
void MODE_AUTO(void* paramenter)
{
    AutoModeInfo AMInfo;
    HandleModeInfo ClearBuf;

    //Ĭ�Ϲ����Զ�ģʽ
    rt_thread_suspend(rt_thread_self());
    rt_schedule();
    while(1)
    {
        //�Զ�������Ϣ���н��յ����ݣ����л����ֱ�ģʽ
        if(rt_mq_recv(AutoModemq,&AMInfo,sizeof(AutoModeInfo),RT_WAITING_NO) == RT_EOK)
        {
            if(!strcmp(AMInfo.ModeChange,"HANDLE START"))
            {
                //��HandleModemq�����������������
                while(1)
                {
                    if(rt_mq_recv(HandleModemq,&ClearBuf,sizeof(HandleModeInfo),RT_WAITING_NO) != RT_EOK)
                        break;
                }
                printf("Switch to HANDLE Mode\r\n");
                rt_enter_critical();                    //����������
                rt_thread_suspend(rt_thread_self());    //�����߳�
                rt_thread_resume(thread4);              //�ָ��ֶ������߳�
                rt_exit_critical();                     //����������
                rt_schedule();                          //����ִ��һ�ε���
            }
            else
            {
                //�Զ�ģʽ��������������Ϣ�����д����ĺ��߽Ƕȸı��ƽ���PWM
                Task_AutoMode_Process(AMInfo);
                //printf("%f\r\n",AMInfo.BlackAngle);
                Drv_Delay_Ms(300);
            }
        }

        // printf("AUTO\r\n");
        // Drv_Delay_Ms(1000);
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
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



