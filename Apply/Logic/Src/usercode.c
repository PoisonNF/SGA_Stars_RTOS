#include "usercode.h"		/* usercodeͷ�ļ� */
#include "drv_hal_conf.h"   /* SGA��ͷ�ļ����� */
#include "task_conf.h"      /* task��ͷ�ļ����� */
#include "ocd_conf.h"       /* OCD��ͷ�ļ����� */
#include "dev_conf.h"		/* Dev��ͷ�ļ����� */
#include "algo_conf.h"		/* Algo��ͷ�ļ����� */
#include "config.h"			/* I/O����ͷ�ļ����� */

/* �߳̾�� */
rt_thread_t thread1 = RT_NULL;
rt_thread_t thread2 = RT_NULL;
rt_thread_t thread3 = RT_NULL;
rt_thread_t thread4 = RT_NULL;
rt_thread_t thread5 = RT_NULL;
rt_thread_t thread6 = RT_NULL;

/* �ź������*/
rt_sem_t DataFromIPC_Sem = RT_NULL;	//��λ�������ź���
rt_sem_t JY901S_Sem = RT_NULL;		//JY901S�����ź���

/*��Ϣ���о��*/
rt_mq_t AutoModemq = RT_NULL;	//�Զ�ģʽ��Ϣ����
rt_mq_t HandleModemq = RT_NULL;	//�ֶ�ģʽ��Ϣ����

ALIGN(RT_ALIGN_SIZE)
/* �û��߼����� */
void UserLogic_Code(void)
{
	/* �����ź��� */
	JY901S_Sem = rt_sem_create("JY901Sem",0,RT_IPC_FLAG_FIFO);
	//if(RT_NULL != JY901S_Sem)	printf("JY901Sem create successful\r\n");

	DataFromIPC_Sem = rt_sem_create("DataFromIPC_Sem",0,RT_IPC_FLAG_FIFO);
	//if(RT_NULL != DataFromIPC_Sem)	printf("DataFromIPC_Sem create successful\r\n");

	/* ������Ϣ���� */
	AutoModemq = rt_mq_create("AutoModemq",50,10,RT_IPC_FLAG_FIFO);
	//if(RT_NULL != AutoModemq)		printf("AutoModemq create successful\r\n");
	HandleModemq = rt_mq_create("HandleModemq",50,10,RT_IPC_FLAG_FIFO);
	//if(RT_NULL != HandleModemq)		printf("HandleModemq create successful\r\n");

	/* 
		�߳�1 ��λ�����ݶ�ȡ
		�߳�2 JY901S���ݶ�ȡ
		�߳�3 MS5837���ݶ�ȡ
		�߳�4 �ֱ��˶�ģʽ
		�߳�5 �Զ�Ѳ��ģʽ
		�߳�6 �����߳�
	 */

	/* �����߳� */
	thread1 = rt_thread_create("DataFromIPC",DataFromIPC,NULL,1024,1,20);
	// if(RT_NULL != thread1)
	// 	printf("RT-Thread create thread1 successful\r\n");

	thread2 = rt_thread_create("JY901SReadThread",JY901SReadThread,NULL,512,4,20);
	// if(RT_NULL != thread2)
	// 	printf("JY901SReadThread create successful\r\n");


	thread3 = rt_thread_create("MS5837ReadThread",MS5837ReadThread,NULL,512,4,20);
	// if(RT_NULL != thread3)
	// 	printf("MS5837ReadThread create successful\r\n");	


	thread4 = rt_thread_create("MODE_HANDLE",MODE_HANDLE,NULL,512,2,40);
	// if(RT_NULL != thread4)
	// 	printf("MODE_HANDLE create successful\r\n");	
	

	thread5 = rt_thread_create("MODE_AUTO",MODE_AUTO,NULL,512,2,40);
	// if(RT_NULL != thread5)
	// 	printf("MODE_AUTO create successful\r\n");

	thread6 = rt_thread_create("TestThread",TestThread,NULL,512,8,20);
	// if(RT_NULL != thread6)
	// 	printf("TestThread create successful\r\n");
	

	// if(rt_thread_startup(thread1) == RT_EOK) printf("DataFromIPC start\r\n");
	// if(rt_thread_startup(thread2) == RT_EOK) printf("JY901SReadThread start\r\n");
	// if(rt_thread_startup(thread3) == RT_EOK) printf("MS5837ReadThread start\r\n");
	// if(rt_thread_startup(thread4) == RT_EOK) printf("MODE_HANDLE start\r\n");
	// if(rt_thread_startup(thread5) == RT_EOK) printf("MODE_AUTO start\r\n");	//�����Զ�Ѳ��ģʽ,�ȴ���λ�����������Կ���
	// if(rt_thread_startup(thread6) == RT_EOK) printf("TestThread start\r\n");

	rt_thread_startup(thread1);
	rt_thread_startup(thread2);
	rt_thread_startup(thread3);
	rt_thread_startup(thread4);
	rt_thread_startup(thread5);
	rt_thread_startup(thread6);

}
