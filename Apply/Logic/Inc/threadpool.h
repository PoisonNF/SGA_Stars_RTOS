#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include "stdint.h"

//�ƽ����궨��
#define LeftHThruster   0       //��ˮƽ�ƽ���
#define RightHThruster  3       //��ˮƽ�ƽ���
#define LeftVThruster   1       //��ֱ�ƽ���
#define RightVThruster  2       //�Ҵ�ֱ�ƽ���

//PWM�洢�ṹ��
typedef struct
{
    uint16_t PWMout[4];
}PWMInfo_T;

/* �߳���ں������� */
void DataFromIPC(void* paramenter);
void JY901SReadThread(void* paramenter);
void MS5837ReadThread(void* paramenter);
void HANDLE_MODE(void* paramenter);
void AUTO_MODE(void* paramenter);
void DepthControl(void* paramenter);
void ReportPWMout(void* paramenter);
void TestThread(void* paramenter);

#endif
