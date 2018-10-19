#include "variable.h"
//const unsigned int Address_Offset = 0x08035000;//��ַƫ��


//ȫ�ֱ�־λ***************************************************************************************************
Delay delay[4];

long Invalid_Value = -10000; 

//����������
//������Զ�¶ȴ�����
uint8_t BHZY_Temp[10] = {0x02, 0x03, 0x00, 0x00, 0x00, 0x02,0xC4,0x38};

//������Զ�絼��
uint8_t BHZY_Conductivity[10] = {0x04, 0x03, 0x00, 0x00, 0x00, 0x03, 0x05,0x9E}; 
//������ԶPH������
uint8_t BHZY_PH[10] = {0x02,0x03,0x00,0x00,0x00,0x01,0x84,0x39};
uint8_t BHZY_PH_4[10] = {0x02, 0x03, 0x00, 0x04, 0x00, 0x01,0xC5,0xF8};
uint8_t BHZY_PH_9_18[10] = {0x02, 0x03, 0x00, 0x06, 0x00, 0x01,0x64,0x38};
uint8_t BHZY_PH_6_86[10] = {0x02, 0x03, 0x00, 0x05, 0x00, 0x01,0x94,0x38};
//GreenPrima�絼���Ǳ�
uint8_t GP_Conductivity[10] ={0x01,0x04,0x00,0x00,0x00,0x02,0xD5,0xCA};
//�Ӻ�PH
uint8_t PH_PH[10] = {0x0A,0x03,0x00,0x20,0x00,0x0A,0x00,0x00};
uint8_t PH_PH_4[20]={0x01,0x10,0x00,0x20,0x00,0x02,0x04,0x00,0x00,0x40,0x80,0xC1,0xD7};
uint8_t PH_PH_9_18[20]={0x01,0x10,0x00,0x20,0x00,0x02,0x04,0xE1,0x48,0x41,0x12,0xF7,0xC0};

//����ѹ��������
uint8_t XY_Strees[10] = {0x01,0x03,0x00,0x00,0x00,0x01,0x84,0x0A};
//Һλ
uint8_t Level[10] = {0x01,0x00,0x03,0x01,0x00,0x02,0x95,0xCB};


//����������********************************************************************************************************
//unsigned int Sensor_Number;  //����������
uint8_t STREES_Type = 4; //ѹ��           
uint8_t LEVEL_Type = 3; //Һλ
uint8_t WPH_Type = 0; //PH
uint8_t WCOND_Type = 1; //�絼��
uint8_t TEMP_Type = 2; //�¶�




//����������****************************************************************************************************
float LEVEL_Low = 0; //Һλ   cm
float LEVEL_High = 100;
float STREES_Low = 0; //ѹ��  
float STREES_High = 200; //ѹ��(kpa)
float WPH_Low = 1; //PH
float WPH_High = 14; //PH
float WCOND_Low = 0.0; //�絼��
float WCOND_High = 20000.0; //�絼��(m)
float TEMP_Low = -20; //�¶�
float TEMP_High = 100; //�¶�
int LEVEL = 0;//0 ��Ӧ��Һ��  1û�и�Ӧ

/*******************************************************
const unsigned int Sensor_Enable_Address = Address_Offset+0;
const unsigned int Data_Send_Interval_Address =Address_Offset+0x100;
const unsigned int Power_Batt_Address =Address_Offset+0x104; //���緽ʽ��ַ
const unsigned int LED_OPEN_CLOSE_TIME_Address=Address_Offset+0x108;    //LED��ʱ�䷶Χ��ַ
const unsigned int LED_STATE_Address=Address_Offset+0x120;    //LED״̬��ַ

const unsigned int System_Init_Address =Address_Offset+0x124;
const unsigned int MODE_Address =Address_Offset+0x128;
const unsigned int GPRSMODE_Address =Address_Offset+0x12C;



const unsigned int Sample_Hz_Address = Address_Offset+0x130; //����Ƶ�ʵ�ַ
const unsigned int Sample_Noise_Time_Address = Address_Offset+0x134; //�����������
const unsigned int GPRS_Port_Address = Address_Offset+0x138; //GPRS�˿�
const unsigned int GPRS_IP_Address = Address_Offset+0x13C;
const unsigned int SERVER_Address = Address_Offset+0x140;
const unsigned int SN_Address = Address_Offset+0x240;
const unsigned int SERVER_LEN_Address = Address_Offset+0x340;
const unsigned int SN_LEN_Address = Address_Offset+0x344;

const unsigned int Correcting_Flag_Address =Address_Offset+0x348; //�����������ص�ַ
const unsigned int Correct_K_WLVL_Address = Address_Offset+0x34C;
unsigned int SENSOR_NUM_Address=Address_Offset+0x350;
unsigned int SENSOR_Address = Address_Offset+0x354;
unsigned int HEART_Address = Address_Offset+0x1010;
unsigned int HEART_LEN_Address = Address_Offset+0x1110;

//��������Ϣ��ַ
unsigned int Sensor_info_Address = Address_Offset+0x2000;
//���������ݵ�ַ
unsigned int Sensor_data_Address = Address_Offset+0x3000;
/*******************************************************/


//******************��������********************
uint16_t	RevComplete = 0;					//������ɱ�־λ


uint8_t Sensor_Cfg_Mode = 0;//���ڽ��մ����������ļ���־
uint8_t Delay_Cfg_Mode = 0;//�̵��������ļ���־
 

//����������
Data sensor_data[5];//�ϴ�����
Sensor_data sensor_array[5];
uint8_t RS485_Rx_buf[500];
uint8_t RS232_Rx_buf[500];//���ڻ���
uint8_t Android_Rx_buf[1000];

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

int dosage_sum = 0;//�ܼ�ҩ��
int dosing_frequency = 9;//ÿСʱ��ҩ�� ÿСʱ9L
int dosage = 1;//Ĭ��1L

uint8_t dir = 0; // 0 ��˳ʱ��   1����ʱ�� 
uint8_t ena = 1; // 0 ���������� 1��ͣ��

float Speed_Motor = 92.16;//����ٶ�


//Һλ��12����

