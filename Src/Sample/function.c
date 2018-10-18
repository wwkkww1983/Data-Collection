#include "function.h"
#include "variable.h"
#include "variable.h" 
#include "string.h"
# include "stdio.h"
# include "stdlib.h"
#include "math.h"
#include "sample.h"
#include "crc16.h"
#include "usart/bsp_debug_usart.h"
#include "RS485/bsp_usartx_RS485.h"
#include "gpio/bsp_gpio.h"
#include "rtc/bsp_rtc.h"
#include "rtc/bsp_calendar.h"
#include "spiflash/bsp_spiflash.h"

/**
  * ��������: ���ڽ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Command_Data(){
  
  printf("Command_Data����ʼִ������\n");
  char *port = NULL;
  char *cmd = NULL;
  
  if(Debug_flag){
    port = strtok(RS232_Rx_buf, ",");
    Debug_flag = 0;
  }
  else{
    port = strtok(Android_Rx_buf, ",");
  }

  cmd = strtok( NULL,",");
  if(cmd[0] == '0'){
    Close_Delay(port[0]);
  }
  else if(cmd[0] == '1'){
    Open_Delay(port[0]);
  }
  else{
    printf("Command_Data���������!!!\n");
  }
}
/**
  * ��������: ���ڽ������ݱ��棨�������豸��Ϣ��
  * �������: ��������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Save_Data(){
  
  printf("Save_Data����ʼ����������Ϣ\n");
  char temp[6][200] = {0};//�ݴ���Ϣ
  char *p = NULL;
  uint8_t s,n = 0;
  int i;
  //�з�����
  if(Debug_flag){
    p = strtok(RS232_Rx_buf, ";");
    Debug_flag = 0;
  }
  else{
    p = strtok(Android_Rx_buf, ";");
  }
  while (p != NULL) {
  //  printf("ptr=%s\n", p);
    strcpy(temp[n],p);
    n++;
    p = strtok(NULL, ";");
  }
  i = n;
  Clean_Data(0);
  Clean_Data(1);
  Clean_Data(2);
  Clean_Data(3);
  Clean_Data(4);
  Clean_Data(5);
  //������Ϣ
  for(s=0;s<i;s++){
    char a = temp[s][0];
    
    switch(a)
    {
        case '0':
            //PH
            //Clean_Data(0);
            Get_Data(0,temp[s]);
           // Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
            break;
        case '1':
            //�絼��
           // Clean_Data(1);
            Get_Data(1,temp[s]);
           // Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
            break;
        case '2':
            //�¶�
            //Clean_Data(2);
            Get_Data(2,temp[s]);
           // Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
            break;
        case '3':
            //ѹ��
           // Clean_Data(3);
            Get_Data(3,temp[s]);
           // Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
            break;
        case '4':
            //Һλ
           // Clean_Data(4);
            Get_Data(4,temp[s]);
           // Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
            break;
        case '5':
            //�̵���
            //Clean_Data(5);
            Get_Data(5,temp[s]);
            Delay_Cfg_Mode = 1;//�̵��������ļ���־
            break;
        default:
        //����
            printf("Save_Data��%d ������Ϣ����!!!\n",a);
            break;
    }
   
  }
  printf("Save_Data��������Ϣ���!!!");

  
  if(n > 0)
   //ֻҪ��������Ϣ�����ñ�־λ
    Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
 
}
/**
  * ��������: �������������
  * �������: ��Ӧ����
  * �� �� ֵ: ��
  * ˵    ������
  */

void Clean_Data(int type){
  if(type == 5){
    //�̵���
    for(int i=0;i<5;i++){
        delay[i].devices[0] = '\0';
        delay[i].port = 0;
        delay[i].state = 0;
        delay[i].type[0] = -1;
        delay[i].num = 0;
        delay[i].control = 0;
        delay[i].counter = 0;
        delay[i].interval_time[0] = '\0';
        delay[i].save_counter = 0;
        delay[i].start_time[0] = '\0';
        delay[i].sustain_time[0] = '\0' ;
    }
    printf("�̵�����Ϣ������\n");
    return ;
  }
    Sensor *p,*q;
    p = sensor_array[type].frist_node;
    q = p;
    for(int j=1;j<sensor_array[type].num;j++){
        q = p->next;
        free(p);
        p = q;
    }
    sensor_array[type].max = 0;
    sensor_array[type].min = 0;
    sensor_array[type].num = 0;
    sensor_array[type].frist_node = NULL;
    printf("%d ��������Ϣ������\n",type);
    return;
}
/**
  * ��������: �ϴ�����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void UpData(){
  
  printf("UpData����ʼ�ϴ�����\n");
  char devicesID[20];
  float value;
  int type,i,j;
  char data[100];
  Sensor *p = NULL;
  //������� 
  for(i=0;i<5;i++){
    //�ϴ����ڵĴ�����
   if(sensor_array[i].num >0){
    p = sensor_array[i].frist_node;
    type = sensor_data[i].type;
    value = sensor_data[i].value;
    sprintf(data,"%.2f,%d;",value,type);
    printf("UpData���ϴ����ݣ�%.2f,%d\n",value,type);
    HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
    for(j=0;j<sensor_array[i].num;j++){
        value = p->value;
        strcpy(devicesID,p->devices);
        sprintf(data,"%.2f,%d,%s;",value,type,devicesID);
        printf("UpData���ϴ����ݣ�%.2f,%d,%s\n",value,type,devicesID);
        HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
         //�������
        p->error = 0;
        p->amount = 0;
        p->value = 0;
        p = p->next;
    }
 
   
   } 
  }
  HAL_UART_Transmit(&husart_debug,"#",1,1000);
  printf("UpData���ϴ��������\n");
}
/**
  * ��������: ���㴫�������ݣ�ƽ��ֵ�����ж��쳣,��֤ÿ���ϴ���ʾ�����ȶ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Get_Average(){
  
  printf("Get_Average����ʼ�������ݾ�ֵ\n");
  Sensor *p = NULL,*q = NULL;//q��¼error�������Ĵ�����
  float temp_value = 0;//�ݴ�����
  int e = 0,error = 0;//e��¼���������,errorΪ�ۼӴ������
  
  for(int i=0;i<5;i++){
    p = sensor_array[i].frist_node;
    if(!p){
        continue;
    }
    while(p){
      //����ȫ������,ȫ����豸������ƽ��ֵ������ߴ����������
        if(p->amount == 0 || p->error >30){
            p->value = 0;
        }
        else{
            p->value = (float)p->value / p->amount;
        }
        temp_value = temp_value + p->value;
        if(p->error > e){
          e = p->error;
          q = p;
        }
        error = error + p->error;
        p = p->next;
    }//while
    sensor_data[i].type = i;
    //����Ե�����
    if(error > 30)
    {
      //�ɼ�����������
        sensor_data[i].value = -10000;
        strcpy(sensor_data[i].devices,q->devices);
    }
    else
    {
        temp_value = temp_value / sensor_array[i].num;
        sensor_array[i].value = temp_value;        
        //�������
        sensor_data[i].value = temp_value;      
    }
  }//for
 
}
/**
  * ��������: ��ȡ�ؼ�����
  * �������: ����������,��������Ϣ�ַ���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Get_Data(int type,char * data){
  
    char *result = NULL;
    Sensor *p = NULL;
    char temp_data[20][50];
    int i,num = 0;//����
    double min,max;
    
    result = strtok(data,",");
    //result = strtok( NULL,",");
    //��ȡ����
    while( result != NULL ) {
       result = strtok( NULL,",");
       strcpy(temp_data[num],result);
       num++;
    }
  
    if(num<10 && type != 5){
      printf("Get_Data:%d ������Ϣ����!!\n",type);
 
      return;
    }
    
    if(type == 5){
        //�̵���
        int n = atoi(temp_data[0]);
        int m;
        for(i=0;i<n;i++){
            strcpy(delay[i].devices,temp_data[4*i+1]);
            delay[i].port = temp_data[4*i+2][0];
            delay[i].control = atoi(temp_data[4*i+3]);
            if(delay[i].control == 4){   
                    temp_data[4*i+4][5] = '\0';
                    strcpy(delay[i].start_time,temp_data[4*i+4]); 
                    strcpy(delay[i].sustain_time,&temp_data[4*i+4][6]); 
            }
            else if(delay[i].control == 3){
                temp_data[4*i+4][5] = '\0';
                strcpy(delay[i].interval_time,temp_data[4*i+4]); 
                strcpy(delay[i].sustain_time,&temp_data[4*i+4][6]); 
            
            }
            else{
                m = strlen(temp_data[4*i+4]);
                delay[i].num = m;
                for(int j=0;j<m;j++){
                    delay[j].type[j] = temp_data[4*i+4][j] - 48;
                }
            }
          
            delay[i].state = 0;
       }
       return ;
    }
    
    sensor_array[type].type = type;
    sensor_array[type].num = atoi(temp_data[0]);
    min = strtod(temp_data[1],NULL);
    max = strtod(temp_data[2],NULL);
    sensor_array[type].min = min;
    sensor_array[type].max = max;
    sensor_array[type].frist_node = NULL;
    //��������������
    for(i=0;i<sensor_array[type].num;i++){
       // printf("%s %c %c\n",temp_data[3*i+3],temp_data[3*i+4][0],temp_data[3*i+5][0]);
        p = (Sensor *)malloc(sizeof(Sensor));
        strcpy(p->devices,temp_data[7*i+3]);
        strcpy(p->command,temp_data[7*i+4]);
        strcpy(p->parsetype,temp_data[7*i+5]);
        p->startadder =  atoi(temp_data[7*i+6]);
        p->datanum = atoi(temp_data[7*i+7]);
        p->keep = atoi(temp_data[7*i+8]);
        strcpy(p->mode,temp_data[7*i+9]);
        p->error = 0;
        p->value = 0;
        p->amount = 0;
        p->next = NULL;
        //��������
        if(sensor_array[type].frist_node == NULL){
            sensor_array[type].frist_node = p;
           // p->next = NULL;
        }
        else{
            p->next = sensor_array[type].frist_node;
            sensor_array[type].frist_node = p;
        }
     }//for
   // free(temp_data);//�ͷ�����
 }
    
/**
  * ��������: ��⴫�����쳣
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Detection(){
  Sensor * p;
  int i;
  char data[20];
  
  for(i=0;i<5;i++){
    p = sensor_array[i].frist_node;
    while(p){
      if(p->error > 30){
        sprintf(data,"!%s",p->devices);
        HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      }
      p = p->next;
    }
  
  }
  

}
/**
  * ��������: �������ղ�ֵ��
  * �������: x���飬y���飬��ֵx
  * �� �� ֵ: yֵ
  * ˵    ������
  */
float lagrange(float *x,float *y,float xx,int n)     /*�������ղ�ֵ�㷨*/

{

    int i,j;

    float *a,yy=0.0;    /*a��Ϊ��ʱ��������¼�������ղ�ֵ����ʽ*/

    a=(float *)malloc(n*sizeof(float));

    for(i=0; i<=n-1; i++)

    {

        a[i]=y[i];

        for(j=0; j<=n-1; j++)

            if(j!=i) a[i]*=(
			-x[j])/(x[i]-x[j]);

        yy+=a[i];

    }

    free(a);

    return yy;

}

 /**
  * ��������: �򿪼̵���
  * �������: �˿ں�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Open_Delay(char port){  
  
  printf("�򿪼̵���:%c\n",port);
  RTC_CalendarShow();
  char data[50];
  switch(port){
    case '1':
      OUTPUT1_GPIO_ON;
      delay[0].state = 1;
      sprintf(data,"%s,1,%d",delay[0].devices,delay[0].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '2':
      OUTPUT2_GPIO_ON;
      delay[1].state = 1;
      sprintf(data,"%s,2,%d",delay[1].devices,delay[1].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '3':
      OUTPUT3_GPIO_ON;
      delay[2].state = 1;
      sprintf(data,"%s,3,%d",delay[2].devices,delay[2].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '4':
      OUTPUT4_GPIO_ON;
      delay[3].state = 1;
      sprintf(data,"%s,4,%d",delay[3].devices,delay[3].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    default:
      break;
  }
}
/**
  * ��������: �޸Ŀ���ʱ��
  * �������: int i��Ӧ������ 
  * �� �� ֵ: ��
  * ˵    ������
  */
void Chang_Start_time(int i){
  
    char *result = NULL;
    char start_temp_data[2][20];
    char interval_temp_data[2][20];
    int n = 0;
    int hours,minute;
    char temp[20];
    RTC_TimeTypeDef stimestructureget;
    /* ��ȡ��ǰʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  
    /* ��ʾʱ�� */
    sprintf(temp,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
    strcpy(delay[i].start_time,temp);
    
    strcpy(temp,delay[i].interval_time);
    result = strtok(temp,":");
       //��ȡ����
    while( result != NULL ) {
       strcpy(interval_temp_data[n],result);
       result = strtok(NULL,":");
       n++;
    }
    n = 0;
    strcpy(temp,delay[i].start_time);
    result = strtok(temp,":");
       //��ȡ����
    while( result != NULL ) {
       strcpy(start_temp_data[n],result);
       result = strtok(NULL,":");
       n++;
    }
    
    minute =  atoi(start_temp_data[1]) + atoi(interval_temp_data[1]);
    if(minute >= 60){
        hours = minute / 60;
        minute = minute % 60;
    }
    hours += (atoi(start_temp_data[0]) + atoi(interval_temp_data[0])); 
    hours = hours % 24;
    sprintf(delay[i].start_time,"%02d:%02d",hours,minute);

}
/**
  * ��������: �رռ̵���
  * �������: �˿ں�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Close_Delay(char port){
  char data[50];
  
  printf("�رռ̵���:%c\n",port);
  RTC_CalendarShow();
    switch(port){
    case '1':
      OUTPUT1_GPIO_OFF; 
      delay[0].state = 0;
      sprintf(data,"%s,1,%d",delay[0].devices,delay[0].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '2':
      OUTPUT2_GPIO_OFF; 
      delay[1].state = 0;
      sprintf(data,"%s,2,%d",delay[1].devices,delay[1].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '3':
      OUTPUT3_GPIO_OFF;
      delay[2].state = 0;
      sprintf(data,"%s,3,%d",delay[2].devices,delay[2].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    case '4':
      OUTPUT4_GPIO_OFF; 
      delay[3].state = 0;
      sprintf(data,"%s,4,%d",delay[3].devices,delay[3].state);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      break;
    default:
      break;
  }
                                    
}

/**
  * ��������: ��ʾ��ǰʱ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void RTC_CalendarShow(void)
{
  //uint8_t str[30]; // �ַ����ݴ�  

  
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  /* ��ȡ��ǰʱ�� */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* ��ȡ��ǰ���� */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

  /* ��ʾ����*/
  printf("��ǰʱ��Ϊ: %02d��(%s��)%02d��%02d��(����%s)  ", 2000+sdatestructureget.Year,zodiac_sign[(2000+sdatestructureget.Year-3)%12],
  sdatestructureget.Month, sdatestructureget.Date,WEEK_STR[sdatestructureget.WeekDay]);
  /* ��ʾʱ�� */
  printf("%02d:%02d:%02d\n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  
}

/**
  * ��������: ���Ƽ̵�����ʽһ������ָ���С����
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_min_from_max(int i){
  int j,n,type;
 
  n = delay[i].num;
  //��ѯָ��
  for(j=0;j<n;j++){
    type = delay[i].type[j];
    if(sensor_array[type].value < sensor_array[type].min)
     //С����Сֵ
     {
        //���ƶ�Ӧ�˿ڼ̵���
        Open_Delay(delay[i].port);
     }
    if(sensor_array[type].value > sensor_array[type].max)
          //�������ֵ
    {
            //���ƶ�Ӧ�˿ڼ̵���
        Close_Delay(delay[i].port);
    }
          
  }//for 
    
 
}
/**
  * ��������: ���Ƽ̵�����ʽ��������ָ��Ӵ�С
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_max_from_min(int i){
  int j,n,type;
 
  n = delay[i].num;
  //��ѯָ��
  for(j=0;j<n;j++){
    type = delay[i].type[j];
    if(sensor_array[type].value < sensor_array[type].min)
     //С����Сֵ
     {
         //���ƶ�Ӧ�˿ڼ̵���
        Close_Delay(delay[i].port);
     }
    if(sensor_array[type].value > sensor_array[type].max)
          //�������ֵ
    {
           
        //���ƶ�Ӧ�˿ڼ̵���
        Open_Delay(delay[i].port);
    }
          
  }//for 



}
/**
  * ��������: ���Ƽ̵�����ʽ������ǰ��ʱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_temer(int i){
    
    char start_time[50],sustain_time[50];
    char temp_data[5][10];
    int n = 0;//����
    char *result = NULL;
    //char hours[5],minutes[5],seconds[5];
   
    RTC_TimeTypeDef stimestructureget;
    /* ��ȡ��ǰʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  
    /* ��ʾʱ�� */
    sprintf(start_time,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
     
    strcpy(delay[i].start_time,start_time);
    strcpy(sustain_time,delay[i].sustain_time);
   
    result = strtok(sustain_time,":");
       //��ȡ����
    while( result != NULL ) {
       strcpy(temp_data[n],result);
      // printf("ʱ�䣺%s\n",result);
       result = strtok(NULL,":");
       n++;
    }
     
    if(n == 0){
      printf("Control_temer�������쳣!\n");
      return;
    }
    delay[i].counter = ((atoi(temp_data[0])) * 60 + atoi(temp_data[1]))*60000;
    delay[i].save_counter = delay[i].counter;

    time_Control_flag = 1;

}
/**
  * ��������: ���Ƽ̵�����ʽ�ģ�ϵͳʱ�䶨ʱ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_sys_temer(int i){
    char start_time[50],sustain_time[50];
    char temp_data[5][10];
    int n = 0;//����
    char *result = NULL;
        
    strcpy(start_time,delay[i].start_time);
    strcpy(sustain_time,delay[i].sustain_time);
   
    result = strtok(sustain_time,":");
       //��ȡ����
    while( result != NULL ) {
       strcpy(temp_data[n],result);
      // printf("ʱ�䣺%s\n",result);
       result = strtok(NULL,":");
       n++;
    }
     
    if(n == 0){
      printf("Control_sys_temer�������쳣!\n");
      return;
    }
    
    delay[i].counter = ((atoi(temp_data[0])) * 60 + atoi(temp_data[1]))*60000;
    delay[i].save_counter = delay[i].counter;
      
    sys_time_Control_flag = 1;

}
/**
  * ��������: ���Ƽ̵���
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ����������ú��˵ļ̵������ж�devices�Ƿ�Ϊ0��0��ʾû��������Ϣ
  */
void Control(){
  int i,n;
  for(i=0;i<5;i++){
    if(delay[i].devices[0] != '0'){
      //������
        n = delay[i].control;
        switch(n){
            case 1:
              Control_min_from_max(i);
              break;
            case 2:
              Control_max_from_min(i);
              break;
            case 3:
              if(delay[i].counter == 0){
                Control_temer(i);
              }
              break;
            case 4:
              if(delay[i].counter == 0){
                Control_sys_temer(i);
              }
              
              break;
            default:
              break;
        }
   
    }//if
  
  }//for
}

/**
  * ��������: �ַ�תʮ������
  * �������: �ַ�
  * �� �� ֵ: ʮ����
  * ˵    ������
  */
uint8_t CharToBit(char c){

  uint8_t b = c -  (uint8_t)48;
 // printf("%x",b);
  return b;

}
void Clear_RS485Buf(){
  for(int i=0;i<RS485_Rx_Count_Old;i++)
    RS485_Rx_buf[i] = 0x00;
}


/**
  * ��������: int��λ��ɸ��� 
  * �������: n ����λ��������num �ƶ���λ�� 
  * �� �� ֵ: ת���ĸ����� 
  * ˵    ������
  */
float movedigit(const int n,const int num)
{
    float f = (float)n;
    printf("......%.2f\n",f);
    switch(num)	
	{
		case 0:
			//f = f * 0.01;
			break;
		case 1:
			f = f * 0.1;
			break;
		case 2:
			f = f * 0.01;
			break;
		case 3:
			f = f * 0.001;
			break;
		case 4:
			f = f * 0.0001;
			break;
		case 5:
			f = f * 0.00001;
			break;
		case 6:
			f = f * 0.000001;
			break;
		case 7:
			f = f * 0.0000001;
			break;
		case 8:
			f = f * 0.00000001;
			break;
		case 9:
			f = f * 0.000000001;
			break;
		default:
			printf("movedigit��num erro!\n");
			break;

	}
        printf("......%.2f\n",f);
	return f;

}
/**
  * ��������: 485�������ݴ���
  * �������: cmd���ݣ�Sensor �������ṹ
  * �� �� ֵ: ��
  * ˵    ����������ɶԽ��յ����ݽ���CRC��֤����֤ʧ��error�����ۼ�
  */

void Modbusprocess(uint8_t * data,Sensor *sensor,int type)
{
  
   uint8_t temp[20]={0x00,0x00,0x00,0x00};

   float value = 0.0;
   float value_float = 0.0;
   int value_int = 0;
  // char buf[10];
   
   if(CRC16_MODBUS(RS485_Rx_buf,RS485_Rx_Count_Old) == 0){
        printf("Modbusprocess������������������ȷ\n");
        //��֤ͨ��
   }
   else{
        printf("Modbusprocess���������������ݴ���\n");
        sensor->error++;
        Clear_RS485Buf();
        return;
   }
   
   if(!strcmp(sensor->parsetype,"Float")){
        if(!strcmp(sensor->mode,"big")){
            if(sensor->datanum == 4){
                temp[2] = data[sensor->startadder-1];
                temp[3] = data[sensor->startadder];
                temp[0] = data[sensor->startadder + 1];
                temp[1] = data[sensor->startadder + 2];
            }
			else{
                temp[2] = data[sensor->startadder];
                temp[3] = data[sensor->startadder-1];
                temp[0] = 0x00;
                temp[1] = 0x00;
            }

        }
		else{
            if(sensor->datanum == 4){
                temp[0] = data[sensor->startadder-1];
                temp[1] = data[sensor->startadder];
                temp[2] = data[sensor->startadder + 1];
                temp[3] = data[sensor->startadder + 2];
            }
            else{
                temp[2] = data[sensor->startadder-1];
                temp[3] = data[sensor->startadder];
                temp[0] = 0x00;
                temp[1] = 0x00;
			}
		}
		value_float = BitToFloat(temp);	
                sensor->value += value_float;
        //sprintf(buf, "%.2f", value_float);

		//save_data
         printf("\nvalue-float:%.2f\n",value_float);
   }
   else if(!strcmp(sensor->parsetype,"Integer")){
        if(sensor->datanum == 2){
            temp[0] = 0x00;
            temp[1] = 0x00;
            temp[2] = data[sensor->startadder - 1];
            temp[3] = data[sensor->startadder];
	}
	else{
            temp[0] = data[sensor->startadder-1];
            temp[1] = data[sensor->startadder];
            temp[2] = data[sensor->startadder + 1];
            temp[3] = data[sensor->startadder + 2];
	}
	value_int = BitToInt(temp);
	value = movedigit(value_int,sensor->keep);
					
	printf("\nvalue-int:%d\n",value_int);
	printf("\nvalue-float:%.2f\n",value);
	//sprintf(buf, "%.2f", value);
					
	}
   else{
        printf("\nNOt compatibility\n");
        //continue;
        return;
   }
   switch(type){
        case 0:
            //PH
              //�ж��Ƿ���������Χ
              if(WPH_Low <= value && WPH_High >= value)
              {
                sensor->value = sensor->value + value;
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }           
            break;
        case 1:
            //�絼��
          //�ж��Ƿ���������Χ
              if(WCOND_Low <= value && WCOND_High >= value)
              {
                sensor->value = sensor->value + value;
      
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }
            break;
        case 2:
            //�¶�   
              //�ж��Ƿ���������Χ
              if(TEMP_Low <= value && TEMP_High >= value)
              {
                sensor->value = sensor->value + value;
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }
            break;
        
        case 3:
            
              //�ж��Ƿ���������Χ
              if(STREES_Low <= value && STREES_High >= value)
              {
                sensor->value = sensor->value + value;
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }
          
            break;
        case 4:
            //Һλ
                    
              //�ж��Ƿ���������Χ
              if(LEVEL_Low <= value && LEVEL_High >= value)
              {
                sensor->value = sensor->value + value;
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }
          
            break;
        default:
        //����
            break;
    }//switch
   Clear_RS485Buf();

}

//16����������
void Hex_Copy(uint8_t * a,uint8_t * b,char num)
{
	uint8_t i=0;
	for(i=0;i<num;i++)
	{
		a[i]=b[i];
	
	}

}

//���ַ�����ǰ׺
//data��Ҫ�ӵ�ǰ׺
//string��Ҫ��ǰ׺���ַ���
void String_Add(char * data,char * string)
{
	uint8_t data_len=0;   //ǰ׺����
	uint8_t string_len=0;   //�ַ�������
	short i=0;
	data_len=strlen(data);			//��ȡǰ׺����
	string_len=strlen(string);		//��ȡ�ַ�������
	for(i=string_len-1;i>=0;i--)
	{
		string[i+data_len]=string[i];
	
	}
	for(i=0;i<data_len;i++)
	{
		string[i]=data[i];
	
	}
}
/**
  * ��������: ʮ�������ֽ���ת��Ϊint
  * �������: �ֽ���
  * �� �� ֵ: int
  * ˵    ������
  */
int BitToInt(uint8_t *bit)
{
    int a = bit[0]<<24;
  	int b = bit[1]<<16;
  	int c = bit[2]<<8;
  	a = a + b + c + bit[3]; 
  	
  return a;
  
}

/**
  * ��������: ʮ�������ֽ���ת��Ϊfloat
  * �������: �ֽ���
  * �� �� ֵ: float
  * ˵    ������
  */
float BitToFloat(uint8_t *bit)
{
    uint8_t m0 = bit[0];
    uint8_t m1 = bit[1];
    uint8_t m2 = bit[2];
    uint8_t m3 = bit[3];
    
    // �����λ
    float sig = 1.;
    if (m0 >=128.)
        sig = -1.;
   // printf("sig %f\n",sig);

    //�����
    float jie = 0.;
        if (m0 >=128.)
        {
            jie = m0-128.;
        }
        else
        {
            jie = m0;
        }
    jie = jie * 2.;
    if (m1 >=128.)
        jie += 1.;
    //printf("jie %f\n",jie);
    jie -= 127.;
    //printf("jie %f\n",jie);
    //��β��
    float tail = 0.;
    if (m1 >=128.)
        m1 -= 128.;
    tail =  m3 + (m2 + m1 * 256.) * 256.;
    //printf("tail %f\n",tail);
    tail  = (tail)/8388608;   //   8388608 = 2^23
    //printf("tail %f\n",tail);
    float f = sig * pow(2., jie) * (1+tail);
    return f;
}
/**
  * ��������: ��ʾ�ֽ�����
  * �������: �ֽ���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Show_Data(uint8_t *bit,int len){
 
  for(int i=0;i<len;i++){
    printf("%02x ",bit[i]);
    
  }
    printf("\n");
}

/**
  * ��������: ����������Ϣ
  * �������: �ֽ���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Save_Device_Data(char* buf){
  
  int size = strlen(buf);
  size = size + 1;
  char c[20];
  
  itoa(size,c);

  buf[size - 1] = '#';
  buf[size] = '\0';
    /* ����SPI��������д�� */
  SPI_FLASH_SectorErase(FLASH_SectorToErase);	
 
  SPI_FLASH_BufferWrite(c, FLASH_WriteAddress,strlen(c));
  SPI_FLASH_BufferWrite(buf, FLASH_WriteAddress+10,strlen(buf));
  
  printf("Save_Device_Data���������ã�%s  \n��С��%d\n",buf,size);
  
 
}

/**
  * ��������: ����ת�ַ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    �������10λ��
  */
void itoa (int n,char s[]){
  
    int i,j,sign;
    char temp[10];
    if((sign = n) < 0)//��¼����
        n = -n;//ʹn��Ϊ����
    i = 0;
    do{
       temp[i++] = n % 10 + '0';//ȡ��һ������
    }
    while ((n /= 10) > 0);//ɾ��������
    if(sign<0)
        temp[i++]='-';
    temp[i]='\0';
    
    for(j=i;j>=0;j--)//���ɵ�����������ģ�����Ҫ�������
      s[i-j] = temp[j-1];
   // printf("aaaaaaaaaaaaaa%s\n",s);
} 

/**
  * ��������: ��ȡ������Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Get_Device_Data(char* buf){
  
  int n;
  char size[10];

  SPI_FLASH_BufferRead(size, FLASH_WriteAddress,10);
  n = atoi(size);
  
  SPI_FLASH_BufferRead(buf, FLASH_WriteAddress+10,n);
 
  
  if(buf[n-1] == '#'){
    buf[n-1] = '\0';
    printf("��ȡ���ã�%s\n",buf);
    Save_Data();
  }
  else
    printf("Get_Device_Data�����ػ�ȡ����ʧ��\n");
}





