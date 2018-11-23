#include "variable.h"

//ȫ�ֱ�־λ***************************************************************************************************
Delay delay[4];

long Invalid_Value = -10000; 


//����������****************************************************************************************************
float LEVEL_Low = 0; //Һλ   mm
float LEVEL_High = 2000;
float STREES_Low = 0; //ѹ��  
float STREES_High = 400; //ѹ��(kpa)
float WPH_Low = 0; //PH
float WPH_High = 14; //PH
float WCOND_Low = 0.0; //�絼��
float WCOND_High = 20000.0; //�絼��(m)
float TEMP_Low = -20; //�¶�
float TEMP_High = 100; //�¶�
int LEVEL = 0;//0 ��Ӧ��Һ��  1û�и�Ӧ


//******************��������********************
uint16_t RevComplete = 0;  //������ɱ�־λ
uint8_t Sensor_Cfg_Mode = 0;//���ڽ��մ����������ļ���־
uint8_t Delay_Cfg_Mode = 0;//�̵��������ļ���־
 
//����������
Data sensor_data[5];//�ϴ�����
Sensor_data sensor_array[5];
uint8_t RS485_Rx_buf[50];
uint8_t RS232_Rx_buf[50];//���ڻ���
uint8_t Android_Rx_buf[1000];
uint8_t Rx_buf[1000];//���籣����������

uint16_t Android_Rx_Count = 0;
uint16_t RS485_Rx_Count = 0;
uint16_t RS485_Rx_Count_Old = -1;
uint16_t RS232_Rx_Count = 0;

uint8_t Sample_flag = 0;//�ɼ���־

uint8_t UpData_flag = 0;//�ϴ����ݱ�־
uint8_t Control_flag = 0;//���Ʊ�־

uint8_t Debug_flag = 0;//���Ա�־

uint8_t time_Control_flag = 0;//��ǰʱ����Ʊ�־
uint8_t sys_time_Control_flag = 0;//��ǰϵͳʱ����Ʊ�־
char alarm_Clock[50] = "0"; 

int contral_time = 0;//����ʱ��  ��λ��

int SCM_state = SCM_RUN;//1:run 0:stop   ϵͳ״̬
uint8_t Save_flag = 0;//����״̬
uint8_t YiYe_pump_flag = 0;//��Һ��״̬
uint8_t YiYe_pump_control_flag = 0;//��Һ�ÿ���״̬
uint8_t Reboot_flag = 0;

/*

̨��1012ģ��ɼ�����
�������������
D0
����01 05 00 10 FF 00 8D FF
�أ�01 05 00 10 00 00 CC 0F

D1
����01 05 00 11 FF 00 DC 3F
�أ�01 05 00 11 00 00 9D CF
*/

uint8_t D0_Open[8] = {0x01,0x05,0x00,0x10,0xFF,0x00,0x8D,0xFF};
uint8_t D0_Close[8] = {0x01,0x05,0x00,0x10,0x00,0x00,0xCC,0x0F};
uint8_t D1_Open[8] = {0x01,0x05,0x00,0x11,0xFF,0x00,0xDC,0x3F};
uint8_t D1_Close[8] = {0x01,0x05,0x00,0x11,0x00,0x00,0x9D,0xCF};



