#ifndef _VARIABLE_H_
#define _VARIABLE_H_
#include "stm32f4xx_hal.h"

#define FALSE                                 0
#define TRUE                                  1

#define SCM_RUN                               1 
#define SCM_STOP                              0 // ϵͳ״̬

#define  LOGIC_SWITCH  0


extern __IO uint16_t warn_timer_count;
//���������ݽṹ(0:PH��1:�絼�ʣ�2:�¶ȣ�3:ѹ����4:Һλ)
typedef struct sensor_info{
    uint8_t num; //����������
    uint8_t type;//����
    float value;
    double min,max;//����ָ����Ϣ
    double up,down;//���Ƹ澯��������Χ
    uint8_t warn;//�����澯������������
    float old_value;//֮ǰ��ƽ��ֵ
    int save_num;
    uint8_t control_delay;
    uint8_t warningFlag;//�澯��־��Ϊ1 ��ʱ���ʾ ÿ���������쳣���澯��
    uint8_t errorNum;//�쳣�������������β��ϴ�
    struct sensor_node *frist_node;//��һ��������
}Sensor_data;   //������
typedef struct sensor_node{
    char devices[32];//�豸ID
    char command[32];//����
    float value;////�ۼ�ֵ����ƽ��ֵ
    char parsetype[8];
    uint8_t startadder;
    uint8_t datanum;
    uint8_t keep;
    uint8_t error;//�����־��������Ĵ���
    uint8_t amount;//����
    uint8_t errorNum;//�쳣�������������β��ϴ�
    char mode[8];
    double elec_4ma;
    double elec_20ma;
    struct sensor_node *next;
}Sensor;   //�������ڵ�

//�ϴ����ݽṹ
typedef struct data{
    int type;
    char devices[32];//�豸ID
    float value;//����-10000��ʾ������Ϣ
}Data;  

//���Ʒ�ʽ��1������Сֵ�ӵ����ֵ           2�������ֵ�ӵ���Сֵ           3����ǰʱ�䶨ʱ��      4��ϵͳʱ���   5:��׿����  6���ָ�긨������
//�̵������ݽṹ��5���̵�����
typedef struct Delay_node{
    char devices[32];//�豸ID
    char port;//�˿�(1���˿�1)��1-5 ��Ӧ��
    int state;//״̬��0���� 1������
    int num;//����ָ������
    int type[6];//�������(0��ʾPH)
    int control;//��������(1������Сֵ�ӵ����ֵ   2�������ֵ�ӵ���Сֵ   3����ǰʱ�䶨ʱ��     4��ϵͳʱ���)
    char start_save_time[12][10];//��ʼʱ��
    char sustain_save_time[12][10];//����ʱ��
    char start_time[12];//��ʼʱ��
    char sustain_time[12];//����ʱ��
    char interval_time[12];//���ʱ��
    int nowControlTime;//��¼��ǰ����ʱ��
    int counter;//��ʱ��
    int save_counter;//�����ʱ��
    int error;//���Ƴ��������쳣��ǿ�ƹرգ������п����߼����ȴ�����
    double volume;//��Ӧҩ�õ����
    double dosage;//��ҩ��ÿСʱ��ҩ��
    uint8_t WarningFlag;
}Delay;   //�̵����ڵ�
extern Delay delay[5];  //port���±��Ӧ


extern __IO uint8_t ManualContorlFlag;
//����������
extern Data sensor_data[7];//�ϴ����ݣ�type���±��Ӧ
extern Sensor_data sensor_array[7];
extern uint8_t RS485_Rx_buf[64];
extern char Android_Rx_buf[1024];

extern __IO uint16_t Android_Rx_Count;
extern __IO uint16_t RS485_Rx_Count;
extern uint16_t RS485_Rx_Count_Old;
extern uint8_t Sample_flag;//�ɼ���־
extern uint8_t UpData_flag;//�ϴ����ݱ�־
extern __IO uint8_t Reboot_flag;
extern __IO uint8_t Save_flag;
extern __IO uint8_t YiYe_pump_flag;//��Һ��״̬,0��ʾ�أ�1��ʾ��
extern __IO uint8_t YiYe_pump_control_flag;//��Һ�ÿ���״̬
extern __IO uint8_t Error_flag;
extern __IO uint8_t RTC_Config_flag;
extern uint8_t index_time_control_flag;
extern uint8_t index_time_control_flag_assist;
extern __IO uint32_t time_num;
extern uint8_t saveRS485Adder;
extern int overWeightNum;
extern float overWeightNumSave;
extern uint8_t ErrorNum;
extern uint8_t delayContorlFlag;


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

extern __IO uint16_t	RevComplete;					//������ɱ�־λ

extern uint8_t Control_flag;//���Ʊ�־
extern uint8_t Sensor_Cfg_Mode;//���ڽ��մ����������ļ���־

extern uint8_t Delay_Cfg_Mode;//�̵��������ļ���־

extern uint8_t time_Control_flag;//���Ʊ�־
extern uint8_t sys_time_Control_flag;//��ǰϵͳʱ����Ʊ�־

extern int contral_time;//����ʱ��  ��λ��
extern char alarm_Clock[50];

extern __IO int SCM_state;
extern char ErrorInfo[100];

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
extern uint8_t selectStateCommand[8];

extern __IO uint8_t FLAG;
extern uint8_t FLAG_REC;
extern __IO uint8_t clear_flag;

#endif
