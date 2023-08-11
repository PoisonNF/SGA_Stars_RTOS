#include "task_conf.h"

#include "config.h"

extern PWMInfo_T PWMInfo;
/**
 * @brief �����ʼ��
 * @param null
 * @retval null
*/
void Task_Thruster_Init(void)
{
	Drv_Delay_Ms(6000);		/* ��ʼͣת�źź�ȴ��ȶ� */
	
	/* ͣת��1500ms(7.5%ռ�ձ�) */
	Drv_PWM_HighLvTimeSet(&PWM[0], STOP_PWM_VALUE);
	Drv_PWM_HighLvTimeSet(&PWM[1], STOP_PWM_VALUE);
	Drv_PWM_HighLvTimeSet(&PWM[2], STOP_PWM_VALUE);
	Drv_PWM_HighLvTimeSet(&PWM[3], STOP_PWM_VALUE);
	Drv_Delay_Ms(2000);
}

/**
 * @brief ���ת�������Ӻ���
 * @param index ѡ�񼸺��ƽ���
 * @param _Htime �ߵ�ƽʱ��	_Htime������Ϊ500-2500,1500ֹͣ
							500-1500 ��ʱ����ת 1500-2500˳ʱ����ת
 * @retval null
*/
void Task_Thruster_SpeedSet(int index,uint16_t _Htime)
{
	Drv_PWM_HighLvTimeSet(&PWM[index], _Htime);
}

/**
 * @brief ���е����ͬת�����ú���
 * @param _Htime �ߵ�ƽʱ��	_Htime������Ϊ500-2500,1500ֹͣ
							500-1500 ��ʱ����ת 1500-2500˳ʱ����ת
 * @retval null
*/
void Task_Thruster_AllSpeedSet(uint16_t _HTime)
{
	Drv_PWM_HighLvTimeSet(&PWM[0], _HTime);
	Drv_PWM_HighLvTimeSet(&PWM[1], _HTime);
	Drv_PWM_HighLvTimeSet(&PWM[2], _HTime);
	Drv_PWM_HighLvTimeSet(&PWM[3], _HTime);
}	

/**
 * @brief �����ʼ����
 * @param index ѡ�񼸺��ƽ���
 * @param _Htime �ߵ�ƽʱ��
 * @retval null
*/
void Task_Thruster_Start(int index,uint16_t _HTime)
{
	/* ��������ٶ����� */
	Task_Thruster_SpeedSet(index,_HTime);
}

/**
 * @brief ���е����ʼ����
 * @param *adress ����ĸ��ƽ����ķ���������׵�ַ
 * @retval null
*/
void Task_Thruster_AllStart(float *adress)
{
	Task_Thruster_Start(0,*adress+STOP_PWM_VALUE);
	Task_Thruster_Start(1,*(adress+1)+STOP_PWM_VALUE);
	Task_Thruster_Start(2,*(adress+2)+STOP_PWM_VALUE);
	Task_Thruster_Start(3,*(adress+3)+STOP_PWM_VALUE);
}

/**
 * @brief ���ֹͣ����
 * @param index ѡ�񼸺��ƽ���
 * @retval null
*/
void Task_Thruster_Stop(int index)
{
	/* ��������ٶ����� */
	Drv_PWM_HighLvTimeSet(&PWM[index], STOP_PWM_VALUE);
	PWMInfo.PWMout[index] = STOP_PWM_VALUE;
}

/**
 * @brief ���е��ֹͣ����
 * @param null
 * @retval null
*/
void Task_Thruster_AllStop(void)
{
	Task_Thruster_Stop(0);
	Task_Thruster_Stop(1);
	Task_Thruster_Stop(2);
	Task_Thruster_Stop(3);
}

