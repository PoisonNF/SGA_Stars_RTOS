#ifndef __TASK_ANALYSISDATA_H_
#define __TASK_ANALYSISDATA_H_

typedef struct 
{
    char ModeChange[15];    //���ģʽ�л�����
    float fNum[3];          //��Ÿ���������
    bool keyPressed;
}HandleModeInfo;

typedef struct 
{
    char ModeChange[15];    //���ģʽ�л�����
    float BlackAngle;       //���߽Ƕ�
}AutoModeInfo;

void Task_AnalysisData(uint8_t *DataBuf);

#endif

