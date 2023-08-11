#include "usercode.h"		/* usercodeͷ�ļ� */
#include "threadpool.h"		/* threadpoolͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

//�洢�����ƽ���������
PWMInfo_T PWMInfo = {1500,1500,1500,1500};

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
                rt_memset(ReceBuf,0,ReceNum);
                ReceNum = 0;
            }
        }
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
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
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
		rt_thread_yield();
    }
}

/* ��ȡMS5837�����߳� */
void MS5837ReadThread(void* paramenter)
{
    while(1)
    {
        OCD_MS5837_GetData(&MS5837);
        if(MS5837.fDepth == 153150.250000)  //δ��MS5837�Ĵ�������
            MS5837.fDepth = 0;
        printf("M %0.2f\r\n",MS5837.fDepth);
        Drv_Delay_Ms(600);
    }
}

/* �ֱ������߳� */
void HANDLE_MODE(void* paramenter)
{
    HandleModeInfo HMInfo;
    AutoModeInfo ClearBuf;

    while(1)
    {
        //�ֱ�������Ϣ���н��յ����ݣ����л����Զ�ģʽ
        if(rt_mq_recv(HandleModemq,&HMInfo,sizeof(HandleModeInfo),RT_WAITING_NO) == RT_EOK)
        {
            if(!rt_strcmp(HMInfo.ModeChange,"AUTO START"))
            {
                //��AutoModemq��Ϣ�����������������
                while(1)
                {
                    if(rt_mq_recv(AutoModemq,&ClearBuf,sizeof(AutoModeInfo),RT_WAITING_NO) != RT_EOK)
                        break;
                }
                Task_Thruster_AllStop();                //�����ƽ���ͣת
                
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
                Task_Thruster_Stop(LeftHThruster);
                Task_Thruster_Stop(RightHThruster);
            }
        }
        // printf("HANDLE\r\n");

        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
    }
}

/* Ѳ��ģʽ�߳� */
void AUTO_MODE(void* paramenter)
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
            if(!rt_strcmp(AMInfo.ModeChange,"HANDLE START"))
            {
                //��HandleModemq�����������������
                while(1)
                {
                    if(rt_mq_recv(HandleModemq,&ClearBuf,sizeof(HandleModeInfo),RT_WAITING_NO) != RT_EOK)
                        break;
                }
                Task_Thruster_AllStop();                //�����ƽ���ͣת

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
                Task_Thruster_Stop(LeftHThruster);
                Task_Thruster_Stop(RightHThruster);
            }
        }

        // printf("AUTO\r\n");
        // Drv_Delay_Ms(1000);
        Drv_Delay_Ms(1);    //�ó�CPU��Դ�������ȼ��߳�
    }
}

/* ������� */
void DepthControl(void* paramenter)
{
    DepthControlInfo DCInfo;
    float ExpDepth = 0.0f;
    float CurrDepth = 0.0f;

    while(1)
    {
        //����������Ϣ���н��յ����ݣ�����ʼ�������
        if(rt_mq_recv(DepthControlmq,&DCInfo,sizeof(DepthControlInfo),RT_WAITING_NO) == RT_EOK)
        {
            ExpDepth = DCInfo.setDepth;
            //printf("%f",ExpDepth);
        }

        //��ȡ��ǰ���
        CurrDepth = MS5837.fDepth;
        //������ƺ���
        task_DepthControl_Process(CurrDepth,ExpDepth);

        Drv_Delay_Ms(600);    //ÿ��һ��ʱ�����һ�ζ���
    }
}

/* �㱨PWMoutֵ */
void ReportPWMout(void* paramenter)
{
    while(1)
    {
        printf("T %d %d %d %d\r\n",
                    PWMInfo.PWMout[LeftHThruster],
                    PWMInfo.PWMout[RightHThruster],
                    PWMInfo.PWMout[LeftVThruster],
                    PWMInfo.PWMout[RightVThruster]);
        Drv_Delay_Ms(1000);    //ÿ��һ��ʱ�����һ�λ㱨
    }
}

/* �����߳� */
void TestThread(void* paramenter)
{
    while(1)
    {
        
        Task_Thruster_AllSpeedSet(1500);
        //printf("test\r\n");
        Drv_Delay_Ms(1000);
        
        Task_Thruster_AllSpeedSet(1550);
        
        Drv_Delay_Ms(1000);
    }
    //����1500Ϊǰ�� 0��Ϊ���ˮƽ�ƽ���
    //����1500Ϊ�ϸ� 1��Ϊ��ഹֱ�ƽ���
    //����1500Ϊ�ϸ� 2��Ϊ�Ҳഹֱ�ƽ���
    //����1500Ϊǰ�� 3��Ϊ�Ҳ�ˮƽ�ƽ���
}



