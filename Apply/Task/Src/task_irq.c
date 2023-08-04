#include "task_conf.h"
#include "usercode.h"		/* usercodeͷ�ļ� */
#include "config.h"


/**
 * @brief �ⲿ�ж�5-9������
 * @retval Null
*/
void EXTI9_5_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&DS3231_ALARM_IRQ);	/* ����ӣ��������޸� */
}

/**
 * @brief �ⲿ�ж�10-15������
 * @retval Null
*/
void EXTI15_10_IRQHandler(void)
{
    //Drv_GPIO_EXTI_IRQHandler(&tPCUart.tGPIO[1]);	/* ����ӣ��������޸� */
}

/**
 * @brief ����1�жϷ�����
 * @retval Null
*/
void USART1_IRQHandler(void)
{   
    rt_interrupt_enter();
    Drv_Uart_DMA_RxHandler(&Uart1);
    rt_sem_release(DataFromIPC_Sem);	//�ͷ��ź���,������λ�������߳̿�ʼ����
    rt_interrupt_leave();
}

/**
 * @brief ����2�жϷ�����
 * @retval Null
*/
void USART2_IRQHandler(void)
{
    rt_interrupt_enter();    //�����ٽ���
    Drv_Uart_DMA_RxHandler(&JY901S.tUART);
    rt_sem_release(JY901S_Sem);	//�ͷ��ź���,�̻߳�ȡ�ź�����ʼ����
	rt_interrupt_leave();    //�˳��ٽ���
}

/**
 * @brief ����3�жϷ�����
 * @retval Null
*/
void USART3_IRQHandler(void)
{
    rt_interrupt_enter();
    Drv_Uart_DMA_RxHandler(&Uart3);
    rt_sem_release(DataFromIPC_Sem);	//�ͷ��ź���,������λ�������߳̿�ʼ����
    rt_interrupt_leave();
}

/**
 * @brief ����4�жϷ�����
 * @retval Null
*/
void UART4_IRQHandler(void)
{

}

/**
 * @brief ����5�жϷ�����
 * @retval Null
*/
void UART5_IRQHandler(void)
{

}

/**
 * @brief ��ʱ��2�жϷ�����
 * @retval Null
*/
void TIM2_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��3�жϷ�����
 * @retval Null
*/
void TIM3_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��4�жϷ�����
 * @retval Null
*/
void TIM4_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��5�жϷ�����
 * @retval Null
*/
void TIM5_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��6�жϷ�����
 * @retval Null
*/
void TIM6_IRQHandler(void)
{
    /* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}

/**
 * @brief ��ʱ��7�жϷ�����
 * @retval Null
*/
void TIM7_IRQHandler(void)
{
	/* ʾ�� */
    //Drv_Timer_IRQHandler(&demoTIM);
}



