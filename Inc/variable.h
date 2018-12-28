#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "stm32f4xx_hal.h"


#define FALSE                                 0
#define TRUE                                  1

#define SCM_RUN                               1 
#define SCM_STOP                              0 // ϵͳ״̬


//���������ݽṹ(0:PH��1:�絼�ʣ�2:�¶ȣ�3:ѹ����4:Һλ)
typedef struct sensor_info{
    int num; //����������
    int type;//����
    float value;
    double min,max;//����ָ����Ϣ
    double up,down;//���Ƹ澯��������Χ
    int warn;//�����澯������������
    float old_value;//֮ǰ��ƽ��ֵ
    int save_num;
    int control_delay;
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
    int elec_4ma;
    int elec_20ma;
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
    int state;//״̬��0���� 1������
    int num;//����ָ������
    int type[6];//�������(0��ʾPH)
    int control;//��������(1������Сֵ�ӵ����ֵ   2�������ֵ�ӵ���Сֵ   3����ǰʱ�䶨ʱ��     4��ϵͳʱ���)
    char start_time[10];//��ʼʱ��
    char sustain_time[10];//����ʱ��
    char interval_time[10];//���ʱ��
    int counter;//��ʱ��
    int save_counter;//�����ʱ��
    int error;//���Ƴ��������쳣��ǿ�ƹرգ������п����߼����ȴ�����
    double volume;//��Ӧҩ�õ����
    double dosage;//��ҩ��ÿСʱ��ҩ��
}Delay;   //�̵����ڵ�
extern Delay delay[5];  //port���±��Ӧ

//����������
extern Data sensor_data[7];//�ϴ����ݣ�type���±��Ӧ
extern Sensor_data sensor_array[7];
extern uint8_t RS485_Rx_buf[50];
extern uint8_t Android_Rx_buf[1000];

extern uint16_t Android_Rx_Count;
extern uint16_t RS485_Rx_Count;
extern uint16_t RS485_Rx_Count_Old;
extern uint8_t Sample_flag;//�ɼ���־
extern uint8_t UpData_flag;//�ϴ����ݱ�־
extern uint8_t Reboot_flag;
extern uint8_t Save_flag;
extern uint8_t YiYe_pump_flag;//��Һ��״̬,0��ʾ�أ�1��ʾ��
extern uint8_t YiYe_pump_control_flag;//��Һ�ÿ���״̬
extern uint8_t Error_flag;
extern uint8_t RTC_Config_flag;
extern uint8_t index_time_control_flag;
extern uint8_t index_time_control_flag_assist;
extern __IO uint32_t time_num;

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
extern int LEVEL;
extern float WEIGHT_Low; //���� kg
extern float WEIGHT_High;

extern uint16_t	RevComplete;					//������ɱ�־λ

extern uint8_t Control_flag;//���Ʊ�־
extern uint8_t Sensor_Cfg_Mode;//���ڽ��մ����������ļ���־

extern uint8_t Delay_Cfg_Mode;//�̵��������ļ���־

extern uint8_t time_Control_flag;//���Ʊ�־
extern uint8_t sys_time_Control_flag;//��ǰϵͳʱ����Ʊ�־

extern int contral_time;//����ʱ��  ��λ��
extern char alarm_Clock[50];

extern int SCM_state;

extern uint8_t D0_Open[8];
extern uint8_t D0_Close[8];
extern uint8_t D1_Open[8];
extern uint8_t D1_Close[8];

extern uint8_t D2_Open[8];
extern uint8_t D2_Close[8];
extern uint8_t D3_Open[8];
extern uint8_t D3_Close[8];

extern uint8_t D4_Open[8];
extern uint8_t D4_Close[8];
extern uint8_t D5_Open[8];
extern uint8_t D5_Close[8];

extern uint8_t D6_Open[8];
extern uint8_t D6_Close[8];
extern uint8_t D7_Open[8];
extern uint8_t D7_Close[8];

extern char Data_Anroid[100];
extern uint8_t FLAG;
extern uint8_t FLAG_REC;
extern uint8_t save_data_flag;
extern uint8_t clear_flag;

#endif
