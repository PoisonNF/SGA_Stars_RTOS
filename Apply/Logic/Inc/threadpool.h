#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include "stdint.h"

//�ƽ����궨��
#define LeftHThruster   0       //��ˮƽ�ƽ���
#define RightHThruster  1       //��ˮƽ�ƽ���
#define LeftVThruster   2       //��ֱ�ƽ���
#define RightVThruster  3       //�Ҵ�ֱ�ƽ���

//PWM�洢�ṹ��
typedef struct
{
    uint16_t PWMout[4];
}PWMInfo_T;

/* �߳���ں������� */
void DataFromIPC(void* paramenter);
void JY901SReadThread(void* paramenter);
void MS5837ReadThread(void* paramenter);
void MODE_HANDLE(void* paramenter);
void MODE_AUTO(void* paramenter);
void TestThread(void* paramenter);

#endif
