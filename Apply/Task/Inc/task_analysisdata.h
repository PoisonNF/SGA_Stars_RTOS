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
    //uint8_t Status;       //ֱ��״̬Ϊ0��ת��״̬Ϊ1
    float BlackAngle;       //����ƫ�ƽǶ�[-90,90],˳ʱ��������ʱ�븺
    float CenterShift;      //����ƫ�ƾ���[-5000,5000],�������¸�
}AutoModeInfo;

typedef struct
{
    float setDepth;         //�趨���
}DepthControlInfo;


void Task_AnalysisData(uint8_t *DataBuf);

#endif

