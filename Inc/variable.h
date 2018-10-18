#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "stm32f4xx_hal.h"

//���������ݽṹ(0:PH��1:�絼�ʣ�2:�¶ȣ�3:Һλ��4:ѹ��)
typedef struct sensor_info{
    int num; //����������
    int type;//����
    float value;
    double min,max;//������Ϣ
    struct sensor_node *frist_node;//��һ��������
}Sensor_data;   //������
typedef struct sensor_node{
    char devices[30];//�豸ID
    char command[50];//����
    float value;////�ۼ�ֵ����ƽ��ֵ
    int error;//�����־��������Ĵ���
    int amount;//����
    char parsetype[10];
    int startadder;
    int datanum;
    int keep;
    char mode[10];
    struct sensor_node *next;
}Sensor;   //�������ڵ�

//�ϴ����ݽṹ
typedef struct data{
    int type;
    char devices[30];//�豸ID
    float value;//����-10000��ʾ������Ϣ
}Data;  

//���Ʒ�ʽ��1������Сֵ�ӵ����ֵ           2�������ֵ�ӵ���Сֵ           3����ǰʱ�䶨ʱ��      4��ϵͳʱ���
//�̵������ݽṹ��5���̵�����
typedef struct Delay_node{
    char devices[30];//�豸ID
    char port;//�˿�(1���˿�1)
    int state;//״̬
    int num;//����ָ������
    int type[5];//�������(0��ʾPH)
    int control;//��������
    char start_time[10];//��ʼʱ��
    char sustain_time[10];//����ʱ��
    char interval_time[10];//���ʱ��
    int counter;//��ʱ��
    int save_counter;//�����ʱ��
}Delay;   //�̵����ڵ�
extern Delay delay[4];  //port���±��Ӧ

//����������
extern Data sensor_data[5];//�ϴ����ݣ�type���±��Ӧ
extern Sensor_data sensor_array[5];
extern uint8_t RS485_Rx_buf[500];
extern uint8_t RS232_Rx_buf[500];//���ڻ���
extern uint8_t Android_Rx_buf[1000];

extern uint16_t Android_Rx_Count;
extern uint16_t RS485_Rx_Count;
extern uint16_t RS485_Rx_Count_Old;
extern uint16_t RS232_Rx_Count;
extern uint8_t Sample_flag;//�ɼ���־
extern uint8_t UpData_flag;//�ϴ����ݱ�־
extern uint8_t Debug_flag;//���Ա�־

//����������
//�¶�
extern uint8_t BHZY_Temp[10];
//������Զ�絼��
extern uint8_t BHZY_Conductivity[10]; 
//������ԶPH������
extern uint8_t BHZY_PH[10];
extern uint8_t BHZY_PH_4[10];
extern uint8_t BHZY_PH_9_18[10];
extern uint8_t BHZY_PH_6_86[10];
//GreenPrima�絼���Ǳ�
extern uint8_t GP_Conductivity[10];
//�Ӻ�PH
extern uint8_t PH_PH[10];
extern uint8_t PH_PH_4[20];
extern uint8_t PH_PH_9_18[20];

//����ѹ��������
extern uint8_t XY_Strees[10];
//Һλ
extern uint8_t Level[10];


//����������********************************************************************************************************

extern uint8_t STREES_Type ; //ѹ��           
extern uint8_t LEVEL_Type ; //Һλ
extern uint8_t WPH_Type  ; //PH
extern uint8_t WCOND_Type ; //�絼��
extern uint8_t TEMP_Type   ; //�¶�


//����������****************************************************************************************************
extern float LEVEL_Low; //Һλ      
extern float LEVEL_High; //Һλ    
extern float STREES_Low ; //ѹ��  
extern float STREES_High; //ѹ��
extern float WPH_Low; //PH
extern float WPH_High ; //PH
extern float WCOND_Low; //�絼��
extern float WCOND_High; //�絼��
extern float TEMP_Low; //�¶�
extern float TEMP_High; //�¶�


//******************��������********************
extern uint16_t	RevComplete;					//������ɱ�־λ

extern uint8_t Control_flag;//���Ʊ�־
extern uint8_t Sensor_Cfg_Mode;//���ڽ��մ����������ļ���־

extern uint8_t Delay_Cfg_Mode;//�̵��������ļ���־

extern uint8_t time_Control_flag;//���Ʊ�־
extern uint8_t sys_time_Control_flag;//��ǰϵͳʱ����Ʊ�־

extern int contral_time;//����ʱ��  ��λ��
extern char alarm_Clock[50];


extern int dosage_sum;//�ܼ�ҩ��
extern int dosing_frequency;//ÿСʱ��ҩ�� ÿСʱ9L
extern int dosage;//Ĭ��1L
#endif
