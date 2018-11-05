#ifndef _VARIABLE_H_
#define _VARIABLE_H_

#include "stm32f4xx_hal.h"


//extern uint8_t dir; // 0 ��˳ʱ��   1����ʱ�� 
//extern uint8_t ena; // 0 ���������� 1��ͣ��


#define FALSE                                 0
#define TRUE                                  1
#define CW                                    0 // ˳ʱ��
#define CCW                                   1 // ��ʱ��

#define SCM_RUN                                   1 //
#define SCM_STOP                                  0 // ϵͳ״̬

#define STOP                                  0 // �Ӽ�������״̬��ֹͣ
#define ACCEL                                 1 // �Ӽ�������״̬�����ٽ׶�
#define DECEL                                 2 // �Ӽ�������״̬�����ٽ׶�
#define RUN                                   3 // �Ӽ�������״̬�����ٽ׶�
#define T1_FREQ                               (SystemCoreClock/(STEPMOTOR_TIM_PRESCALER+1)) // Ƶ��ftֵ
#define FSPR                                  200         //���������Ȧ����
#define MICRO_STEP                            32          // �������������ϸ����
#define SPR                                   (FSPR*MICRO_STEP)   // ��תһȦ��Ҫ��������

// ��ѧ����
#define ALPHA                                 ((float)(2*3.14159/SPR))       // ��= 2*pi/spr
#define A_T_x10                               ((float)(10*ALPHA*T1_FREQ))
#define T1_FREQ_148                           ((float)((T1_FREQ*0.676)/10)) // 0.676Ϊ�������ֵ
#define A_SQ                                  ((float)(2*100000*ALPHA)) 
#define A_x200                                ((float)(200*ALPHA))

typedef struct {
  __IO uint8_t  run_state ;  // �����ת״̬
  __IO uint8_t  dir ;        // �����ת����
  __IO int32_t  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
  __IO uint32_t decel_start; // ��������λ��
  __IO int32_t  decel_val;   // ���ٽ׶β���
  __IO int32_t  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
  __IO int32_t  accel_count; // �Ӽ��ٽ׶μ���ֵ
  __IO int32_t  speed; // �����ٶȣ�rpm��
  __IO int32_t  step; //����
  __IO int32_t  min_speed; //��С�ٶ�  �����ݵ����ͬ������ͬ��
  __IO int32_t  max_speed; //����ٶ�
  __IO uint16_t Toggle_Pulse;//����Ƶ��
}speedRampData;


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
    int state;//״̬��0���� 1������
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
extern uint8_t Rx_buf[1000];

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

extern speedRampData Stepper_Motor[4];//�������


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
extern int LEVEL;



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


extern int SCM_state;


#endif
