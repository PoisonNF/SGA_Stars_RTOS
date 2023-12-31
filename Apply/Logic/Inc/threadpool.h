#ifndef __THREADPOOL_H_
#define __THREADPOOL_H_

#include "stdint.h"

//推进器宏定义
#define LeftHThruster   0       //左水平推进器
#define RightHThruster  3       //右水平推进器
#define LeftVThruster   1       //左垂直推进器
#define RightVThruster  2       //右垂直推进器

//PWM存储结构体
typedef struct
{
    uint16_t PWMout[4];
}PWMInfo_T;

/* 线程入口函数声明 */
void DataFromIPC(void* paramenter);
void JY901SReadThread(void* paramenter);
void MS5837ReadThread(void* paramenter);
void HANDLE_MODE(void* paramenter);
void AUTO_MODE(void* paramenter);
void DepthControl(void* paramenter);
void ReportPWMout(void* paramenter);
void TestThread(void* paramenter);

#endif
