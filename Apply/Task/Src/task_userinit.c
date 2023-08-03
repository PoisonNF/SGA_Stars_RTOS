#include "task_conf.h"
#include "ocd_conf.h"
#include "config.h"

/* �����ʼ������ */
void Task_UserInit(void)
{   
    //��ʼ������1�ʹ���3
    Drv_Uart_DMAInit(&Uart1);
    Drv_Uart_DMAInit(&Uart3);

    //��ʼ��JY901S
    OCD_JY901_DMAInit(&JY901S);
    printf("JY901S INIT!\r\n");

    //��ʼ��MS5837
    if(!OCD_MS5837_Init(&MS5837))
        printf("MS5837 ERROR\r\n");

    //��ʼ��PWM
    Drv_PWM_Init(PWM,8);
    printf("PWM INIT!\r\n");
}
