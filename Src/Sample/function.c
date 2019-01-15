#include "function.h"
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
#include "spiflash/bsp_spiflash.h"
#include "led/bsp_led.h"

/**
  * ��������: ���˴�������Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Backups_Data(){

  printf("Backups_Data�����������Ϣ\n");
  Clean_Data(0);
  Clean_Data(1);
  Clean_Data(2);
  Clean_Data(3);
  Clean_Data(4);
  Clean_Data(5); 
  Clean_Data(6); 
  Clean_Data(7); 
  time_Control_flag = 0;
  sys_time_Control_flag = 0;
  index_time_control_flag = 0;
  Sensor_Cfg_Mode = 0; 
  //���ͱ�־�ð�׿���·�������
  HAL_UART_Transmit(&husart_debug,"[",1,1000);
  return;
  
}


/**
  * ��������: ���ڽ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Command_Data()
{
  printf("Command_Data����ʼִ������\n");
  char *port = NULL;
  char *cmd = NULL;
  port = strtok(Android_Rx_buf, ",");
  cmd = strtok( NULL,",");
  printf("Command_Data���˿ڣ�%s ���%s\n",port,cmd);
  if(port[0] > '5' || port[0] < '1')
  {
    printf("Command_Data������˿�%c����!!!\n",port[0]);
  }
  if(!strcmp(cmd,"0"))
  {
    Close_Delay(port[0]);
  }
  else if(!strcmp(cmd,"1"))
  {
    Open_Delay(port[0]);
  }
  else
  {
    printf("Command_Data���������,�������%s!!!\n",cmd);
  }
  return;
}
/**
  * ��������: ���ڽ������ݱ��棨�������豸��Ϣ��
  * �������: ��������
  * �� �� ֵ: ��
  * ˵    ������
  */
void Save_Data()
{
  printf("Save_Data����ʼ����������Ϣ\n");
  char temp[500] = {0};//�ݴ���Ϣ
  char *nowData = NULL;
  int error = 0;
  int len = 0;
  int data_len = 0;
  char buf[1000] = {0};//���ݷ����ݴ���
  int n = 0;
  char c = '0';
  int flag = 1;

  //�з�����
  strcpy(buf,Android_Rx_buf);
  //p = strtok(Android_Rx_buf, ";");
  data_len = strlen(buf);
  //�յ�����������
  if(data_len > 10){
    //��������ǰ�����������Ϣ
    Clean_Data(0);
    Clean_Data(1);
    Clean_Data(2);
    Clean_Data(3);
    Clean_Data(4);
    Clean_Data(5); 
    Clean_Data(6); 
    Clean_Data(7); 
    time_Control_flag = 0;
    sys_time_Control_flag = 0;
    index_time_control_flag = 0;
    Sensor_Cfg_Mode = 0; 
    Delay_Cfg_Mode = 0;//�̵��������ļ���־

    nowData = strtok(buf, ";");
    
    while (nowData != NULL) 
    {
      n++;
      strcpy(temp,nowData);
      //��ȡ���
      c = temp[0]; 
      len += strlen(nowData);
      len = len + 1;
      //ָ����һ�������
      //nowData = &buf[len];
      nowData = strtok(&buf[len], ";");
      //�и��������
      if(len >= data_len)
        nowData = NULL;

      switch(c)
      {
          case '0':
              //PH
              error = Get_Data(0,temp);
              break;
          case '1':
              //�絼��
              error = Get_Data(1,temp);
              break;
          case '2':
              //�¶�
              error = Get_Data(2,temp);
              break;
          case '3':
              //ѹ��
              error = Get_Data(3,temp);
              break;
          case '4':
              //Һλ
              error = Get_Data(4,temp);
              break;
          case '5':
              //�̵���
              error = Get_Data(5,temp);
              Delay_Cfg_Mode = 1;//�̵��������ļ���־
              break;
          case '6':
              //����
              error = Get_Data(6,temp);
              break;
          case '7':
              //�
              error = Get_Data(7,temp);
              break;
          default:
          //����
              flag = 0;
              printf("Save_Data��%d ������Ϣ����!!!\n",c);
              break;
      }
      if(error == -1)
      {
        printf("%c������ô��󣬵������ݻ���\n",c);
        Backups_Data();
        //���ò�ִ�е��籣��
        save_data_flag = 1;
        return;
      }
    }
    if(n > 0 && flag)
    {
     //ֻҪ��������Ϣ�����ñ�־λ,�������óɹ�
      printf("Save_Data��������Ϣ���!!!\n");
      Sensor_Cfg_Mode = 1;//���ڽ��մ����������ļ���־
      save_data_flag = 0;
      //�������õ�flash
      Save_Device_Data();
    }
    //�رռ̵���
    Close_Delay('1');
    Close_Delay('2');
    Close_Delay('3');
    Close_Delay('4');
    Close_Delay('5');
  }
  return;
}
/**
  * ��������: �������������
  * �������: ��Ӧ����
  * �� �� ֵ: ��
  * ˵    ������
  */

void Clean_Data(int type)
{
  Sensor *p = NULL;
  Sensor *q = NULL;

  if(type == 5)
  {
    //�̵���
    for(int i=0;i<5;i++)
    { 
        memset(delay[i].devices,'\0',sizeof(delay[i].devices));
        delay[i].port = 0;
        delay[i].state = 0;
        delay[i].type[0] = -1;
        delay[i].num = 0;
        delay[i].control = 0;
        delay[i].counter = 1000;
        memset(delay[i].interval_time,'\0',sizeof(delay[i].interval_time));
        delay[i].save_counter = 0;
        memset(delay[i].sustain_save_time,'\0',sizeof(delay[i].sustain_save_time));
        memset(delay[i].sustain_time,'\0',sizeof(delay[i].sustain_time));
        memset(delay[i].start_save_time,'\0',sizeof(delay[i].start_save_time));
        memset(delay[i].start_save_time,'\0',sizeof(delay[i].start_time));
        delay[i].error = 0;
        delay[i].volume = 0;
        delay[i].dosage = 0;
        delay[i].nowControlTime = 0;
    }
    printf("Clean_Data���̵�����Ϣ������\n");
    return ;
  }
  if(type >= 6)
  {
    type--;
  }
  sensor_array[type].max = 0;
  sensor_array[type].min = 0;
  sensor_array[type].num = 0;
  sensor_array[type].warn= 0;
  sensor_array[type].up = 0;
  sensor_array[type].down = 0;
  sensor_array[type].frist_node = NULL;
  sensor_array[type].old_value = 0;
  sensor_array[type].save_num = 0;
  sensor_array[type].control_delay = 6;//��ʼ����5����ʾû�м̵������Ƹ�ָ��
  sensor_array[type].warningFlag = 0;
  sensor_array[type].errorNum = 0;
  
  p = sensor_array[type].frist_node;
  if(p){
    p = p->next;
    sensor_array[type].frist_node = NULL;
    while(p)
    {
      q = p;
      p = p->next;
      free(q);
    }
  }
  printf("Clean_Data��%d ��������Ϣ������\n",type);
  return;
}
/**
  * ��������: �ϴ�����������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void UpData()
{
  printf("UpData����ʼ�ϴ�����\n");
  char devicesID[20] = {0};
  float value = 0.0;
  int type = 0,i = 0,j = 0;
  uint8_t data[50] = {0};
  Sensor *p = NULL;
  //������� 
  HAL_UART_Transmit(&husart_debug,"#",1,1000);
  for(i=0;i<7;i++)
  {
    //�ϴ����ڵĴ�����
   if(sensor_array[i].num >0){
    p = sensor_array[i].frist_node;
    type = sensor_data[i].type;
    if(type >= 5){
      type++;
    }
    if(sensor_array[i].errorNum == 0){
      value = sensor_data[i].value;
      sprintf(data,"%.2f,%d;",value,type);
      printf("UpData���ϴ����ݣ�%.2f,%d\n",value,type);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
    }
    else{
      printf("UpData�����ݳ����쳣������Ϊ%d���������ϴ�\n",sensor_array[i].errorNum);
      //sensor_array[i].errorNum = 0;
      continue;
    }
    
    if(i == 5){
        
      if(value <= sensor_array[i].min)
      {
          //�����澯
          printf("��Һ�÷����澯\n");
          Alarm();
      }
    }
    
    for(j=0;j<sensor_array[i].num;j++)
    {
      if(p->errorNum == 0){
        value = p->value;
        strcpy(devicesID,p->devices);
        sprintf(data,"%.2f,%d,%s;",value,type,devicesID);
        printf("UpData���ϴ����ݣ�%.2f,%d,%s\n",value,type,devicesID);
        HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
        p->value = 0.0;
        p = p->next;
      }
      else{
        printf("UpData��%s���ݳ����쳣������Ϊ%d���������ϴ�\n",p->devices,p->errorNum);
        continue;
      }
    }
   } 
  }
  HAL_UART_Transmit(&husart_debug,"%",1,1000);
  printf("UpData���ϴ��������\n");
  return;
}
/**
  * ��������: ���㴫�������ݣ�ƽ��ֵ�����ж��쳣,��֤ÿ���ϴ���ʾ�����ȶ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    �����ϴ�����ƽ��ֵ��ÿ����������ֵ�����ƽ��ֵΪ-10000����ʾ�д������쳣�����д���������Ϊ0��Ϊ�쳣������
  */
void Get_Average()
{
  Sensor *p = NULL;
  Sensor *q = NULL;//q��¼error�������Ĵ�����
  float temp_value = 0;//�ݴ�����
  int e = 0;
  int error = 0;//e��¼���������,errorΪ�ۼӴ������
  char data[20] = {0};
  char str[10] = {0};
  int error_num = 0;
  
  printf("Get_Average����ʼ�������ݾ�ֵ\n");
  for(int i=0;i<7;i++)
  {
    temp_value = 0;
    p = sensor_array[i].frist_node;
    if(!p)
      continue;
    
    while(p)
    {
        printf("Get_Average: ���%d  value = %.2f ���������%d,�ɼ�������%d\n",i+1,p->value,p->error,p->amount);
        //����ȫ������,ȫ����豸������ƽ��ֵ������ߴ����������
        if(((p->amount + p->error) > 0) && (p->amount == 0 || p->error > 20))
        {
            p->value = -0.01;
            //�澯�����ۼ�
            if(p->errorNum > 2){
              sprintf(data,"!%s",p->devices);
              HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
              printf("Get_Average��%d�Ŵ�����%s����,�����������Σ��ϴ��澯\n",i,p->devices);
              p->errorNum = 0;
            }
            else{
              p->errorNum ++;
              printf("Get_Average��%d�Ŵ�����%s�����ۼƴ���%d\n",i,p->devices,p->errorNum );
            }
            error_num ++;
        }
        //�������
        else if(p->amount > 0)
        {
            //��ƽ��ֵ����          
            p->value = (float)p->value / p->amount;
            if((p->value > sensor_array[i].up || p->value < sensor_array[i].down)){
                 //�������ý��з������澯
                if(!sensor_array[i].warningFlag)
                  sensor_array[i].warn++;
                else{
                 //�����澯
                  printf("Get_Average��%d�Ŵ�����%s�����澯\n",i,p->devices);
                  Alarm();
                }
            }
        }
        //p->amount = 0 p->error = 0 ���
        else{
            p->value = 0.0;
        }
        //ƽ��ֵ�ۼ�
        if(p->value > 0.0)
          temp_value = temp_value + p->value;
        //q�������������Ĵ�������e�������
        if(p->error > e)
        {
          e = p->error;
          q = p;
        }
        error = error + p->error;
        p->error = 0;
        p->amount = 0;
        
        p = p->next;
    }//while
    
    if(sensor_array[i].warn == sensor_array[i].num || sensor_array[i].warn > 2)
    {
        //�����澯
        printf("Detection��%d�Ŵ����������澯,����%d\n",i,sensor_array[i].warn);
        Alarm();
    }
    else if(error_num == sensor_array[i].num)
    {
      
        //�澯�����ۼ�
            if(sensor_array[i].errorNum > 2){
              //�����澯
              printf("Detection��%d�Ŵ�����ȫ�������쳣,�����������Σ��ϴ��澯\n",i);
              Alarm();
              Open_Error(delay[sensor_array[i].control_delay].port);
              sensor_array[i].errorNum = 0;
            }
            else{
              sensor_array[i].errorNum ++;
              printf("Detection��%d�Ŵ�����ȫ�������쳣���쳣����%d\n",i,sensor_array[i].errorNum);
            }
        
    }
    else
    {
      printf("Detection��%d�Ŵ���������\n",i);
    }        
    error_num = 0;
    sensor_array[i].warn = 0;
    sensor_data[i].type = i;
    //����Ե�����
    if(error > 10)
    {
        //�ɼ�����������
        sensor_data[i].value = -10000;
        strcpy(sensor_data[i].devices,q->devices);
    }
    else
    {
        temp_value = temp_value / sensor_array[i].num;
        sprintf(str,"%0.2f",temp_value);
        sensor_array[i].value = atof(str);    
        //�������
        sensor_data[i].value = atof(str);      
    }
    //ÿ�����ӱ���һ������
    if(sensor_array[i].save_num == 0)
    {
        sensor_array[i].old_value = temp_value; 
    }
    //���ڼ�ʱ
    sensor_array[i].save_num = (sensor_array[i].save_num + 1) % 3;
    error = 0;
    temp_value = 0;
  }//for
  return;
}
/**
  * ��������: ��ȡ�ؼ�����
  * �������: ����������,��������Ϣ�ַ���
  * �� �� ֵ: ���ش����� -1��������Ϣ���󣬷���0 ������ϢOK
  * ˵    ������
  */
int Get_Data(int type,char *data)
{
    char *result = NULL;
    Sensor *q = NULL;
    char *temp_data[140] = {0};
    uint8_t i = 0;
    uint8_t num = 0;
    uint8_t n = 0;
    uint8_t m = 0;//����
    double min = 0.0;
    double max = 0.0;
    double up = 0.0;
    double down = 0.0;
    int port = 0;
    
    result = strtok(data,",");
    
    //��ȡ����
    while( result != NULL ) 
    {
       result = strtok( NULL,",");
       temp_data[num] = result;
       num++;
    }
  
    if(num<10 && type != 5)
    {
      //������Ϣ�������Ի���
      printf("Get_Data:%d ������Ϣ����,������%d\n",type,num);
      //Backups_Data();
      return -1;
    }
    
    if(type == 5)
    {
        //��ȡ�̵�������
        n = atoi(temp_data[0]);
        for(i=0;i<n;i++)
        {
            port = temp_data[5*i+2][0] - 49;
            strcpy(delay[port].devices,temp_data[5*i+1]);
            delay[port].port = temp_data[5*i+2][0];
            if(delay[port].port < '1' || delay[port].port > '4')
            {
              printf("Get_Data:%d ���ö˿���Ϣ����,������%d\n",type,delay[port].port);
              return -1;
            }
            //ÿСʱ��ҩ��Ϣ
            delay[port].dosage = atof(temp_data[5*i+5]);
            if(delay[port].dosage < 0 || delay[port].dosage > 10000){
              printf("Get_Data:%d ���ü�ҩ��Ϣ����,������%0.2f\n",type,delay[port].dosage);
              return -1;
            }
           
            delay[port].control = atoi(temp_data[5*i+3]);
            //����5ģʽ����Ҫ����Ƭ�����ƣ���׿���п���
            if(delay[port].control == 5)
            {
              continue;
            }
            if(delay[port].control < 1 || delay[port].control > 7)
            {
              printf("Get_Data:%d ���ÿ���ģʽ��Ϣ����,������%d\n",type,delay[port].control);
              return -1;
            }
            
            if(delay[port].control == 4)
            {   
                    int data_len = strlen(temp_data[5*i+4]);
                    char *rs = strtok( temp_data[5*i+4],".");
                    //char temp_save_data[12][15] = "";
                    int l = 1;   
                    
                    rs[5] = '\0';
                    strcpy(delay[port].start_time,rs); 
                    strcpy(delay[port].sustain_time,&rs[6]); 
                    strcpy(delay[port].start_save_time[0],rs); 
                    strcpy(delay[port].sustain_save_time[0],&rs[6]); 
                 
                    //��ȡ����
                    while( rs != NULL ) 
                    {
                       rs = strtok( NULL,".");
                       //temp_save_data[l] = rs;
                  
                       rs[5] = '\0';
                       strcpy(delay[port].start_save_time[l],rs); 
                       strcpy(delay[port].sustain_save_time[l],&rs[6]); 
                       //printf("%s ||||| %s",rs,&rs[6]);
                       l++;             
                    }                                
            }
            else if(delay[port].control == 3)
            {
                temp_data[5*i+4][5] = '\0';
                strcpy(delay[port].interval_time,temp_data[5*i+4]); 
                strcpy(delay[port].sustain_time,&temp_data[5*i+4][6]); 
            }
            else if(delay[port].control == 6)
            {
               result = strtok(temp_data[5*i+4],"|");
               m = strlen(result);
               if(m > 7)
               {
                  printf("Get_Data:%d ���ÿ��ƴ�������Ϣ����,������%d\n",type,m);
                  return -1;
               }
               delay[port].num = m;
               for(int j=0;j<m;j++)
               {
                   delay[port].type[j] = result[j] - '0';
                   if(delay[port].type[j] >= 6)
                   {
                        delay[port].type[j]-- ;
                   }
                    //�󶨶�Ӧ������
                   sensor_array[delay[port].type[j]].control_delay = port;
               }
               result = strtok( NULL,"|");
               delay[port].volume = atof(result);   
            }
            else
            {
                m = strlen(temp_data[5*i+4]);
                if(m > 7)
                {
                  printf("Get_Data:%d ���ÿ��ƴ�������Ϣ����,������%d\n",type,m);
                  return -1;
                }
                delay[port].num = m;
                for(int j=0;j<m;j++)
                {
                    delay[port].type[j] = temp_data[5*i+4][j] - '0';
                    if(delay[port].type[j] >= 6)
                    {
                        delay[port].type[j]--;
                    }
                    //�󶨶�Ӧ������
                    sensor_array[delay[port].type[j]].control_delay = port;
                }
            }
            delay[port].state = 0;
       }
       return 0;
    }
    if(type >= 6)
    {
      type --;
    }
    sensor_array[type].type = type;
    sensor_array[type].num = atoi(temp_data[0]);
    //����������������10
    if(sensor_array[type].num < 0 || sensor_array[type].num > 10)
    {
      printf("Get_Data:%d ���ô�����������Ϣ����,������%d\n",type,sensor_array[type].num);
      return -1;
    }
    min = strtod(temp_data[1],NULL);
    max = strtod(temp_data[2],NULL);
    if(min > max)
    {
      printf("Get_Data:%d ���ô�����ָ����Ϣ����,���ֵΪ%.2f,��СֵΪ%.2f,\n",type,max,min);
      return -1;
    }
    sensor_array[type].min = min;
    sensor_array[type].max = max;
    up = strtod(temp_data[4],NULL);
    down = strtod(temp_data[3],NULL);
    //������Һ�ô�Χ
    overWeightNumSave = max;
    
    if(up < down)
    {
      printf("Get_Data:%d ���ô������澯��Ϣ����,���ֵΪ%.2f,��СֵΪ%.2f,\n",type,up,down);
      return -1;
    }
    if(!strcmp(temp_data[5],"1")){
    
      //���ø澯��־
      sensor_array[type].warningFlag = 1;
    }
    sensor_array[type].up = up;
    sensor_array[type].down = down;
    sensor_array[type].warn = 0;
    sensor_array[type].frist_node = NULL;
    sensor_array[type].old_value = 0;
    sensor_array[type].save_num = 0;
    sensor_array[type].errorNum = 0;

    //��������������
    for(i=0;i<sensor_array[type].num;i++)
    {
        //printf("%s %s %s\n",temp_data[9*i+5],temp_data[9*i+6],temp_data[9*i+7]);
        q = (Sensor *)malloc(sizeof(Sensor));
        strcpy(q->devices,temp_data[9*i+6]);
        if(strcpy(q->command,temp_data[9*i+7]) == NULL){
          printf("Get_Data:%d ���������%s\n",type,q->command);
          return -1;
        }
        strcpy(q->parsetype,temp_data[9*i+8]);
        int staradd = atoi(temp_data[9*i+9]);
        
        //printf("%d  %d  %s",q->startadder,atoi(temp_data[9*i+8]),temp_data[9*i+8]);
        if(staradd < 0 || staradd > 50)
        {
          printf("Get_Data:%d ������ʼ��ַ��Ϣ����,��ַ��ΧΪ0-50,������%d\n",type,staradd);
          return -1;
        }
        q->startadder =  staradd;
        int datanum = atoi(temp_data[9*i+10]);
        
        if(datanum < 0 || datanum > 30)
        {
          printf("Get_Data:%d ��������λ����Ϣ����,����λ����ΧΪ0-30,������%d\n",type,q->datanum);
          return -1;
        }
        q->datanum = datanum;
        int keep = atoi(temp_data[9*i+11]);
        if(keep < 0 || keep > 9)
        {
          printf("Get_Data:%d ���ñ���λ����Ϣ����,����λ����ΧΪ0-9,������%d\n",type,q->keep);
          return -1;
        }
        q->keep = keep;
        strcpy(q->mode,temp_data[9*i+12]);
        
        double elec_4ma = atof(temp_data[9*i+13]);
        
        if(elec_4ma < 0.0 || elec_4ma > 100000.0)
        {
          printf("Get_Data:%d ����4ma��Ӧ��Ϣ����,������%0.2f\n",type,q->elec_4ma);
          return -1;
        }
        q->elec_4ma = elec_4ma;
        double elec_20ma = atof(temp_data[9*i+14]);
        if(elec_20ma < 0.0 || elec_20ma > 10000000.0)
        {
          printf("Get_Data:%d ����20ma��Ӧ��Ϣ����,������%0.2f\n",type,q->elec_20ma);
          return -1;
        }
        q->elec_20ma = elec_20ma;
        q->error = 0;
        q->value = 0;
        q->amount = 0;
        q->errorNum = 0;
        q->next = NULL;
        //��������
        if(sensor_array[type].frist_node == NULL)
        {
            sensor_array[type].frist_node = q;
           // p->next = NULL;
        }
        else
        {
            q->next = sensor_array[type].frist_node;
            sensor_array[type].frist_node = q;
        }
     }//for
   // free(temp_data);//�ͷ�����
    return 0;
 }
    
/**
  * ��������: ��⴫�����쳣
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Detection()
{
  int i = 0;
  int j = 0;
  printf("Detection����ʼ��⴫�����쳣\n");
  
  for(i=0;i<7;i++)
  {
    j = sensor_array[i].control_delay;

    //����ÿ��������ݼ�⣬���ҩ�ô򿪵������ݱ仯û�а��������仯���رռ̵���
    //�м̵������ƶ�Ӧ���
    //printf("%d  %d",delay[j].state,sensor_array[i].save_num);
    if(delay[j].state == 1 && sensor_array[i].save_num >= 2 && delay[j].control <= 2 && sensor_array[i].errorNum < 2)
    { 
       //3�����ж�һ��
       if(fabs(sensor_array[i].value-sensor_array[i].old_value) < 0.1)
       {
          printf("Detection���̵���������������������ָ��������û�б仯\n");
        }
        else if(delay[j].control == 1 && (sensor_array[i].value-sensor_array[i].old_value)<0)
        {
          printf("Detection���̵������������ڿ��Ʒ�ʽһ����������ָ�귴������\n");
        }
        else if(delay[j].control == 2 && (sensor_array[i].value-sensor_array[i].old_value)>0)
        {
            printf("Detection���̵������������ڿ��Ʒ�ʽ������������ָ�귴������\n");
        }
        else
        {
          printf("Detection���̵�����������");
          continue;
        }
        Alarm();
        Open_Error(delay[j].port);
    }//if
  }//for
  return;

  

}

 /**
  * ��������: �����澯
  * �������: 
  * �� �� ֵ: ��
  * ˵    ������
  */
void Alarm()
{  
  printf("Alarm:�����쳣,�����澯\n");
  if(Android_Rx_Count >= 100) 
  {
      return;
  }
  Close_Light();
  Open_Beep();
  //��������3s
  //HAL_Delay(3000); 
  //delay_ms2(500);
  warn_timer_count = 300;
 
  return;
}

 /**
  * ��������: ��Ӧ�̵��������쳣���ƣ��ȴ�����
  * �������: �̵����˿�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Open_Error(char port)
{  
  int   p = port - 49;
  char data[10];
  
  if(port > '0' && port < '6' && delay[p].error == 0)
  {
    printf("Error:�����쳣,%c�ż̵���ֹͣ����\n",port);
    Close_Delay(port);
    delay[p].error = 1;
    sprintf(data,"$%c\n",port);
    HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
  }
  return;
}


 /**
  * ��������: ��Ӧ�̵����쳣������ɣ�����쳣
  * �������: �̵����˿�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Close_Error(char port)
{  
  int p = port - 49;

  if(port > '0' && port < '6' && delay[p].error == 1)
  {
    printf("Close_Erro:%c�ż̵�����������\n",port);
    delay[p].error = 0;
  }

  return;
}



 /**
  * ��������: �򿪼̵���
  * �������: �˿ں�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Open_Delay(char port)
{  
 
  char data[50] = {0};
  int flag = 1;
  int int_port = port - 48;
  uint8_t len = 0;
  int num = 0;

  while(1){
    switch(port)
    {
      case '1':
        RS485_Send_Data(D3_Open,(uint8_t)8);
        break;
      case '2':
        RS485_Send_Data(D4_Open,(uint8_t)8);
        break;
      case '3':
        RS485_Send_Data(D5_Open,(uint8_t)8);
        break;
      case '4':
        RS485_Send_Data(D6_Open,(uint8_t)8);
        break;
      case '5':
        RS485_Send_Data(D7_Open,(uint8_t)8);
        break;
      default:
        flag = 0;
        printf("Open_Delay:�򿪼̵���ʧ�ܣ��̵����˿ڴ���%c!\n",port);
        break;
    }
    if(!flag)
    {
      return;
    }
    delay_ms2(5);
    len = RS485_Receive_Data();
    Clear_RS485Buf();
    if(len > 0)
    {
        //ȷ���̵����ڹ���
        break;
    }
    else
    {
      num++;
    }
    if(num >= 3)
    {
      printf("Open_Delay:%c�˿ڼ̵���ʧ��\n",port);
      return;
    }
  }//while
  if(flag)
  {   
      //�޸ļ̵���״̬�����ϴ��̵���״̬
      sprintf(data,"*%s,%c,%d",delay[int_port - 1].devices,port,1);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      delay[int_port-1].state = 1;
      printf("Open_Delay:��%c�˿ڼ̵���\n",port);
  }
  return;
}


/**
  * ��������: �޸Ŀ���ʱ��
  * �������: int i��Ӧ������ 
  * �� �� ֵ: ��
  * ˵    ������
  */
void Chang_Start_time(int i)
{
    char *result = NULL;
    char start_temp_data[2][20];
    char interval_temp_data[2][20];
    int n = 0;
    int hours = 0,minute = 0;
    char temp[20] = {0};
    RTC_TimeTypeDef stimestructureget;
    RTC_DateTypeDef sdatestructureget;
    /* ��ȡ��ǰʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    sprintf(temp,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
    strcpy(delay[i].start_time,temp);
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
    
    strcpy(temp,delay[i].interval_time);
    result = strtok(temp,":");
       //��ȡ����
    while( result != NULL ) 
    {
       strcpy(interval_temp_data[n],result);
       result = strtok(NULL,":");
       n++;
    }
    n = 0;
    strcpy(temp,delay[i].start_time);
    result = strtok(temp,":");
       //��ȡ����
    while( result != NULL ) 
    {
       strcpy(start_temp_data[n],result);
       result = strtok(NULL,":");
       n++;
    }
    
    minute =  atoi(start_temp_data[1]) + atoi(interval_temp_data[1]);
    if(minute >= 60)
    {
        hours = minute / 60;
        minute = minute % 60;
    }
    hours += (atoi(start_temp_data[0]) + atoi(interval_temp_data[0])); 
    hours = hours % 24;
    sprintf(delay[i].start_time,"%02d:%02d",hours,minute);
    return;
}
/**
  * ��������: �رռ̵���
  * �������: �˿ں�
  * �� �� ֵ: ��
  * ˵    ������
  */
void Close_Delay(char port)
{
  char data[50] = {0};
  int flag = 1;
  int int_port = port - 48;
  uint8_t len = 0;
  int num = 0;//ʧ�ܼ���������������

  while(1){
    switch(port)
    {
      case '1':
      RS485_Send_Data(D3_Close,(uint8_t)8);
      break;
    case '2':
      RS485_Send_Data(D4_Close,(uint8_t)8); 
      break;
    case '3':
      RS485_Send_Data(D5_Close,(uint8_t)8);
      break;
    case '4':
      RS485_Send_Data(D6_Close,(uint8_t)8);
      break;
    case '5':
      RS485_Send_Data(D7_Close,(uint8_t)8);
      break;
    default:
      flag = 0;
      printf("Close_Delay:�رռ̵���ʧ�ܣ��˿ڴ���:%c\n",port);
      break;
    }
    if(!flag)
    {
      return;
    }
    delay_ms2(5);
    len = RS485_Receive_Data();
    Clear_RS485Buf();
    if(len > 0)
    {
      break;
    }
    else
    {
      num++;
    }
    if(num >= 3)
    {
      printf("Close_Delay:�ر�%c�˿ڼ̵���ʧ��\n",port);
      return;
    }
  }//while
  if(flag)
  { 
      sprintf(data,"*%s,%c,%d",delay[int_port - 1].devices,port,0);
      HAL_UART_Transmit(&husart_debug,data,strlen((char *)data),1000);
      delay[int_port - 1].state = 0;
      printf("Close_Delay:�ر�%c�˿ڼ̵���\n",port);      
  }
  
  return;
}

/**
  * ��������: ��ʾ��ǰʱ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void RTC_CalendarShow(void)
{
 
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  
  /* ��ȡ��ǰʱ�� */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* ��ȡ��ǰ���� */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  

  /* ��ʾ����*/
  printf("��ǰʱ��Ϊ: %02d��%02d��%02d��", 2000+sdatestructureget.Year,sdatestructureget.Month, sdatestructureget.Date);
  /* ��ʾʱ�� */
  printf("%02d:%02d:%02d\n", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  return;
}

/**
  * ��������: ���Ƽ̵�����ʽһ������ָ���С����
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_min_from_max(int i)
{
  int j = 0;
  int n = 0;
  int type = 0;
  
  printf("Control_min_from_max:�̵������ƶ˿�%d,��ʼ����\n",i+1);
  n = delay[i].num;
  //��ѯָ��
  for(j=0;j<n;j++)
  {
    type = delay[i].type[j];
    if(sensor_array[type].value < sensor_array[type].min && delay[i].state == 0)
     //С����Сֵ
     {
        //���ƶ�Ӧ�˿ڼ̵���
          Open_Delay(delay[i].port);
     }
    if(sensor_array[type].value > sensor_array[type].max && delay[i].state == 1)
          //�������ֵ
    {
            //���ƶ�Ӧ�˿ڼ̵���
          Close_Delay(delay[i].port);
    }
          
  }//for 
    
  return;
}
/**
  * ��������: ���Ƽ̵�����ʽ��������ָ��Ӵ�С
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_max_from_min(int i)
{
  int j = 0;
  int n = 0;
  int type = 0;
 
  printf("Control_max_from_min:�̵������ƶ˿�%d,��ʼ����\n",i+1);
  n = delay[i].num;
  //��ѯָ��
  for(j=0;j<n;j++){
    type = delay[i].type[j];
    if(sensor_array[type].value < sensor_array[type].min && delay[i].state == 1)
     //С����Сֵ
     {
         //���ƶ�Ӧ�˿ڼ̵���
        Close_Delay(delay[i].port);
     }
    if(sensor_array[type].value > sensor_array[type].max && delay[i].state == 0)
          //�������ֵ
    {
           
        //���ƶ�Ӧ�˿ڼ̵���
        Open_Delay(delay[i].port);
    }
          
  }//for 
  return;
}
/**
  * ��������: ���Ƽ̵�����ʽ������ǰ��ʱ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_temer(int i)
{
    
    char start_time[50] = {0};
    char sustain_time[50] ={0};
    char temp_data[5][10] ={0};
    int n = 0;//����
    char *result = NULL;
    //char hours[5],minutes[5],seconds[5];
   
    printf("Control_temer:�̵������ƶ˿�%d,��ʼ����\n",i+1);
    printf("Control_temer:�ӵ�ǰʱ�俪ʼ��ʱ��,ÿ%s��%sʱ��\n",delay[i].interval_time,delay[i].sustain_time);
    RTC_TimeTypeDef stimestructureget;
    RTC_DateTypeDef sdatestructureget;

    strcpy(sustain_time,delay[i].sustain_time);
   
    result = strtok(sustain_time,":");
       //��ȡ����
    while( result != NULL ) 
    {
       strcpy(temp_data[n],result);
      // printf("ʱ�䣺%s\n",result);
       result = strtok(NULL,":");
       n++;
    }
    if(n == 0)
    {
      printf("Control_temer�������쳣!\n");
      return;
    }
    int temp_s = atoi(temp_data[0]) * 60;
    int temp_f = atoi(temp_data[1]);
    
    delay[i].counter = (temp_s + temp_f) * 6000;
    delay[i].save_counter = delay[i].counter;
    
    //delay[i].counter = ((atoi(temp_data[0])) * 60 + atoi(temp_data[1]))*6000;
    //delay[i].save_counter = delay[i].counter;

    time_Control_flag = 1;
    
    /* ��ȡ��ǰʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    sprintf(start_time,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
    strcpy(delay[i].start_time,start_time);
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
    return;

}
/**
  * ��������: ���Ƽ̵�����ʽ�ģ�ϵͳʱ�䶨ʱ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_sys_temer(int i)
{
    char start_time[50] = {0};
    char sustain_time[50] = {0};
    char temp_data[5][10] = {0};
    int nowTimeHours = 0;
    int nowTimeMinutes = 0;
    int controlTimeHours = 0;
    int controlTimeMinutes = 0;
    int n = 0;//����
    char *result = NULL;
    RTC_TimeTypeDef stimestructureget;
    RTC_DateTypeDef sdatestructureget;
    int eflag = 0;
    
     /* ��ȡ��ǰʱ�� */
    HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);

    nowTimeHours = stimestructureget.Hours;
    nowTimeMinutes = stimestructureget.Minutes;
    
    printf("�˿�:%d,nowControlTime:%d,nowTimeHours:%d,nowTimeMinutes:%d\n",i+1,delay[i].nowControlTime,nowTimeHours,nowTimeMinutes);
    
    while(1){
      //�޸ĵ�ǰ����ʱ��
      if(delay[i].start_save_time[delay[i].nowControlTime][2]!=':'){
        delay[i].nowControlTime = 0;
        break;
      }
      strcpy(start_time ,delay[i].start_save_time[delay[i].nowControlTime]);
      //strcpy(sustain_time,delay[i].sustain_save_time[delay[i].nowControlTime]);
      
      start_time[2] = '\0';
      controlTimeHours = atoi(start_time);
      controlTimeMinutes = atoi(&start_time[3]);
      
      printf("nowControlTime:%d,controlTimeHours:%d,controlTimeMinutes:%d\n",delay[i].nowControlTime,controlTimeHours,controlTimeMinutes);
      
      if(nowTimeHours < controlTimeHours)
      {
          break;
      }
      else if(nowTimeHours > controlTimeHours)
      {
          delay[i].nowControlTime =  (delay[i].nowControlTime + 1) % 12;
      }
      else{
        if(nowTimeMinutes < controlTimeMinutes)
        {
            break;
        }
        else if(nowTimeMinutes > controlTimeMinutes)
        {
            delay[i].nowControlTime =  (delay[i].nowControlTime + 1) % 12;
        }
        else{
          eflag = 1;
          break;
        }
      }
      
    }
    
    if(!eflag){
      strcpy(delay[i].start_time,delay[i].start_save_time[delay[i].nowControlTime]);
    }
    else{
      strcpy(start_time,delay[i].start_save_time[delay[i].nowControlTime]);
      if(start_time[4] == '9'){
        start_time[3] ++;
        start_time[4] = '0';
      }
      else{
        start_time[4] ++;
      }
      strcpy(delay[i].start_time,start_time);
    }
    strcpy(delay[i].sustain_time,delay[i].sustain_save_time[delay[i].nowControlTime]);
        
    printf("Control_temer:�̵������ƶ˿�%d,��ʼ����\n",i+1);
    printf("Control_temer:ÿ���ض�ʱ���,ÿ��%s��%sʱ��\n",delay[i].start_time,delay[i].sustain_time);
    
    strcpy(start_time,delay[i].start_time);
    strcpy(sustain_time,delay[i].sustain_time);
   
    result = strtok(sustain_time,":");
       //��ȡ����
    while( result != NULL ) 
    {
       strcpy(temp_data[n],result);
      // printf("ʱ�䣺%s\n",result);
       result = strtok(NULL,":");
       n++;
    }
     
    if(n == 0)
    {
      printf("Control_sys_temer�������쳣!\n");
      return;
    }
    int temp_s = atoi(temp_data[0]) * 60;
    int temp_f = atoi(temp_data[1]);
    
    delay[i].counter = (temp_s + temp_f) * 6000;
    if(delay[i].counter == 1000){
      delay[i].counter = 1005;
    }
    if(delay[i].counter <= 5){
      delay[i].counter = 3000;
    }
    delay[i].save_counter = delay[i].counter;
      
    //printf("%d  ||||  %d\n",delay[i].save_counter,delay[i].counter);
    sys_time_Control_flag = 1;
    return;

}
/**
  * ��������: ���Ƽ̵�����ʽ����ָ����Сֵ��Χ�£���ʱ��(ֻ�ܿ����)
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Control_index_time(int i)
{
  
  int j = 0;
  int n = 0;
  int type = 0;
  char start_time[50] = {0};
 // char sustain_time[50] ={0};
 // char temp_data[5][10] ={0};
  //char *result = NULL;
  //int m = 0;
  double differ = 0.0;
  //char str[10];
  RTC_TimeTypeDef stimestructureget;
  RTC_DateTypeDef sdatestructureget;
  
  
  if(!index_time_control_flag){
 
    printf("Control_index_time:�̵������ƶ˿�%d,��ʼ����\n",i);
    n = delay[i].num;
    //��ѯָ��
    for(j=0;j<n;j++)
    {
      type = delay[i].type[j];
      //sensor_array[type].value = 5.98;
      if(sensor_array[type].value < sensor_array[type].min && delay[i].state == 0)
       //С����Сֵ
      {
           //������Ҫ��ҩ��ʱ��
            //sprintf(str,"%0.2f",sensor_array[type].value);
            differ = sensor_array[type].min - sensor_array[type].value;;
            differ = delay[i].volume * differ;
            if(delay[i].dosage < 0){
              printf("Control_index_time:ÿСʱ��ҩ����������\n");
              return;
            }
            differ = differ / (delay[i].dosage * 100.00);
            differ = differ * 60;
            if(differ <= 0){
              printf("Control_index_time:�ɼ�����С��ָ��,�����趨��ҩ����С��������\n");
              return;
            }
            delay[i].counter = (int)differ * 6000;
            delay[i].save_counter = delay[i].counter;
            time_Control_flag = 1;
            printf("Control_index_time:�ɼ�����%.2fС��ָ��%.2f����ʼ��ҩ����%.2f����\n",sensor_array[type].value,sensor_array[type].min,differ);
            /* ��ȡ��ǰʱ�� */
            HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
            sprintf(start_time,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
            strcpy(delay[i].start_time,start_time);
            HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
            time_num = 0;
            index_time_control_flag = 1;
      }
      if(sensor_array[type].value > sensor_array[type].max && delay[i].state == 1)
            //�������ֵ
      {
          //���ƶ�Ӧ�˿ڼ̵���
         Close_Delay(delay[i].port);
         index_time_control_flag = 1;
      }    
    }
  }
  if(!index_time_control_flag_assist){
    
    printf("Control_index_time:�̵���ʵʱ���ƶ˿�%d,��ʼ����\n",i);
    n = delay[i].num;
    //��ѯָ��
    for(j=0;j<n;j++)
    {
      type = delay[i].type[j];
      if(sensor_array[type].value > sensor_array[type].max && delay[i].state == 1)
            //�������ֵ
      {
          //���ƶ�Ӧ�˿ڼ̵���
         Close_Delay(delay[i].port);
      }
          
    }
    
  }
  return;
}

/**
  * ��������: ���Ƽ̵���
  * �������: ��Ӧ�˿ڵļ̵���
  * �� �� ֵ: ��
  * ˵    ����������ú��˵ļ̵������ж�devices�Ƿ�Ϊ0��0��ʾû��������Ϣ
  */
void Control()
{
  int i = 0;
  int n = 0;
  for(i=0;i<5;i++)
  {
    if(delay[i].control != 0 && !delay[i].error)
    {
      //�����ã����ݲ�ͬ������ѡ����Ʒ�ʽ
        n = delay[i].control;
        printf("��ǰ����ֵ:%d\n",delay[i].counter);
        switch(n)
        {
            case 1:
              Control_min_from_max(i);
              break;
            case 2:
              Control_max_from_min(i);
              break;
            case 3:
              if(delay[i].counter == 1000)
              {
                Control_temer(i);
              }
              break;
            case 4:
              if(delay[i].counter == 1000)
              {
                Control_sys_temer(i);
              }
              break;
           case 6:
             if(!index_time_control_flag || !index_time_control_flag_assist)
                Control_index_time(i);
              break;
            default:
              break;
        }
   
    }//if
  
  }//for
  return;
}

/**
  * ��������: �ַ�תʮ������
  * �������: �ַ�
  * �� �� ֵ: ʮ����
  * ˵    ������
  */
uint8_t CharToBit(char c)
{

  uint8_t b = c -  (uint8_t)48;
 // printf("%x",b);
  return b;

}
void Clear_RS485Buf()
{
  for(int i=0;i<RS485_Rx_Count_Old;i++)
    RS485_Rx_buf[i] = 0x00;
  RS485_Rx_Count = 0;
  return;
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
    //printf("......%.2f\n",f);
    switch(num)	
    {
      case 0:
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
       // printf("......%.2f\n",f);
    return f;

}
/**
  * ��������: 485�������ݴ���
  * �������: cmd���ݣ�Sensor �������ṹ
  * �� �� ֵ: ��
  * ˵    ����������ɶԽ��յ����ݽ���CRC��֤����֤ʧ��error�����ۼ�
  */

int Modbusprocess(Sensor *sensor,int type)
{
   uint8_t temp[20]={0x00,0x00,0x00,0x00};
   float value = 0.0;
   float value_float = 0.0;
   int value_int = 0;
   uint8_t buf[50] = {0};

   for(int i=0;i<RS485_Rx_Count_Old;i++)
     buf[i] = RS485_Rx_buf[i];
   //mencpy(buf,RS485_Rx_buf,RS485_Rx_Count_Old);
   if(CRC16_MODBUS(buf,RS485_Rx_Count_Old) == 0)
   {
        printf("Modbusprocess������������������ȷ\n");
        //��֤ͨ��
   }
   else
   {
        printf("Modbusprocess���������������ݴ���:%s\n",sensor->devices);
       // sprintf(Data_Anroid,"[Modbusprocess���������������ݴ���]");
        //Send_Data_To_Android();
        sensor->error++;
        Clear_RS485Buf();
        return -1;
   }
   Show_Data(buf,10);
   if(!strcmp(sensor->mode,"small"))
   {
     if(sensor->datanum == 4)
     {
        temp[2] = buf[sensor->startadder-1];
        temp[3] = buf[sensor->startadder];
        temp[0] = buf[sensor->startadder + 1];
        temp[1] = buf[sensor->startadder + 2];
     }
     else
     {
        temp[2] = buf[sensor->startadder];
        temp[3] = buf[sensor->startadder-1];
        temp[0] = 0x00;
        temp[1] = 0x00;
      }
   }
   else
   {
      if(sensor->datanum == 4)
      {
          temp[0] = buf[sensor->startadder-1];
          temp[1] = buf[sensor->startadder];
          temp[2] = buf[sensor->startadder + 1];
          temp[3] = buf[sensor->startadder + 2];
       }
       else
       {
          temp[2] = buf[sensor->startadder-1];
          temp[3] = buf[sensor->startadder];
          temp[0] = 0x00;
          temp[1] = 0x00;
        }
   }
   
   if(!strcmp(sensor->parsetype,"Float"))
   {
        value_float = BitToFloat(temp);	
	value = value_float;
   }
   else if(!strcmp(sensor->parsetype,"Integer"))
   {
	value_int = BitToInt(temp);
	value = movedigit(value_int,sensor->keep);				
    }
    else{
      printf("Modbusprocess:Not compatibility\n");
        //continue;
      return -1;
    }
   
   printf("Modbusprocess�ɼ����� value:%.2f\n",value);

   if(sensor->elec_4ma > 0.0  || sensor->elec_20ma > 0.0)
   {
     float f = 0.0;
     printf("Modbusprocessת����elec_20ma=%0.2f  elec_4ma=%0.2f\n",sensor->elec_20ma ,sensor->elec_4ma);
     f = value * (sensor->elec_20ma - sensor->elec_4ma);
     value = f / 20000.0;
     value= value + sensor->elec_4ma;
     printf("Modbusprocess:value:%.2f\n",value); 
   }
    switch(type)
    {
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
              //value = value * 0.08 + 185;
                    
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
        case 5:
            //����
              
              //�ж��Ƿ���������Χ
              if(value >= sensor_array[type].max && overWeightNumSave >= sensor_array[type].max){
                //�������γ������ر���Һ��
                overWeightNum++;
                if(overWeightNum >= 3){
                  Close_YiYe_pupm();
                  printf("�������γ������ֵ���ر���Һ��\n");
                  overWeightNum = 0;
                }
              }
              else{
                overWeightNum = 0;
              }

              if(WEIGHT_Low <= value && WEIGHT_High >= value)
              {
                sensor->value = sensor->value + value;
                sensor->amount++;
              }
              else
              {
                sensor->error++;
              }
              overWeightNumSave = value;
          
            break;
        case 6:
            //�
              value = value * 2;
              //�ж��Ƿ���������Χ
              if(value >= 0)
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
          printf("Modbusprocess:��������������\n");
          //sprintf(Data_Anroid,"[Modbusprocess:��������������]");
          //Send_Data_To_Android();
          break;
    }//switch
   Clear_RS485Buf();
   return 0;
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
void Show_Data(uint8_t *bit,int len)
{
 
  for(int i=0;i<len;i++)
  {
    printf("%02x ",bit[i]);
    
  }
    printf("\n");
  return;
}

/**
  * ��������: ����������Ϣ(���籣��)
  * �������: �ֽ���
  * �� �� ֵ: ��
  * ˵    ������
  */
void Save_Device_Data()
{
  if(!save_data_flag){ 
    int size = strlen(Android_Rx_buf);
    size = size + 1;
    char c[20] = {0};
    
    uint8_t buffer[1000];
  
    itoa(size,c);
    if(size < 30){
      printf("Save_Device_Data����������ʧ�ܣ�%s\n��С��%d\n",Android_Rx_buf,size);
      return;
    }

   // strcpy(Rx_buf,buf);
    strcpy(buffer,Android_Rx_buf);
  
    buffer[size - 1] = '#';
    if(SCM_state == SCM_RUN)
      buffer[size] = '1';
    else
      buffer[size] = '0';
  
    buffer[size + 1] = '\0';
    buffer[size + 2] = '\0';
    size++;
  
    /* ����SPI��������д�� */
    SPI_FLASH_SectorErase(FLASH_SectorToErase);	
    SPI_FLASH_BufferWrite(c, FLASH_WriteAddress+10,strlen(c));
    SPI_FLASH_BufferWrite(buffer, FLASH_WriteAddress+20,strlen(buffer));
    printf("Save_Device_Data���������ã�%s\n��С��%d\n",buffer,size);
    
    //��֤д��ɹ�
    uint8_t sizeRead[10] = {0};
    SPI_FLASH_BufferRead(sizeRead, FLASH_WriteAddress,10);
    SPI_FLASH_BufferRead(sizeRead, FLASH_WriteAddress+10,10);
    int n = atoi(sizeRead);
    if(n > 1000)
    {
      n = n / 10;
    }
    n++;
    if(n == size){
      printf("Save_Device_Data��д��ɹ�\n");
      
    }
    else{
      printf("Save_Device_Data��д��ʧ��,�ٴ�д��\n");
      Backups_Data();
    }
  }
  else{
    save_data_flag = 0;
  }
  return;
    
}

/**
  * ��������: ����ת�ַ���
  * �������: ��
  * �� �� ֵ: ��
  * ˵    �������10λ��
  */
void itoa (int n,char s[])
{
  
    int i = 0,j = 0,sign = 0;
    char temp[10] = {0};
    if((sign = n) < 0)//��¼����
        n = -n;//ʹn��Ϊ����
    do
    {
       temp[i++] = n % 10 + '0';//ȡ��һ������
    }
    while ((n /= 10) > 0);//ɾ��������
    if(sign<0)
        temp[i++]='-';
    temp[i]='\0';
    
    for(j=i;j>=0;j--)//���ɵ�����������ģ�����Ҫ�������
      s[i-j] = temp[j-1];
    return;
} 

/**
  * ��������: ��ȡ������Ϣ
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void Get_Device_Data(char* buf)
{
  
  int n = 0;
  uint8_t size[10] = {0};
  uint8_t buffer[1000] = {0};

  SPI_FLASH_BufferRead(size, FLASH_WriteAddress,10);
  SPI_FLASH_BufferRead(size, FLASH_WriteAddress+10,10);
  //printf("Get_Device_Data:��ַ %s  \nsize:%s \n",FLASH_WriteAddress,size);
  n = atoi(size);
  if(n > 1000)
  {
   n = n / 10;
  }
  n++;
  SPI_FLASH_BufferRead(buffer, FLASH_WriteAddress+20,n);
  printf("Get_Device_Data:%s\nsize:%d\n",buffer,n);
  
  if(buffer[n-1] == '0')
  {
    SCM_state = SCM_STOP;
    printf("Get_Device_Data:ϵͳ��ǰ״̬stop\n");
  }
  else if(buffer[n-1] == '1')
  {
    SCM_state = SCM_RUN;
    printf("Get_Device_Data:ϵͳ��ǰ״̬run\n");
  }
  else
  {
    SCM_state = SCM_STOP;
    printf("Get_Device_Data:��ȡϵͳ״̬ʧ�ܣ�ϵͳ����ֹͣ״̬\n");
    //sprintf(Data_Anroid,"[Get_Device_Data:��ȡϵͳ״̬ʧ�ܣ�ϵͳ����ֹͣ״̬]");
    //Send_Data_To_Android();
  }
  if(buffer[n-2] == '#')
  {
    buffer[n-2] = '\0';
    strcpy(buf,buffer);
    printf("Get_Device_Data����ȡ����%s\n",buffer);
    Save_Data();
  }
  else
  {
    
    printf("Get_Device_Data�����ػ�ȡ����ʧ��\n");
   // sprintf(Data_Anroid,"[Get_Device_Data�����ػ�ȡ����ʧ��]");
   // Send_Data_To_Android();
    
  }
  return;
    
}


/**
  * ��������: ��������
  * �������: 
  * �� �� ֵ: 
  * ˵    ������
  */
void Reboot()
{
    HAL_UART_Transmit(&husart_debug,"}",1,1000);
    __set_FAULTMASK(1);//�ر����ж�
    NVIC_SystemReset();//����Ƭ������
    return;
}

/**
  * ��������: �򿪷�����
  * �������: 
  * �� �� ֵ: 
  * ˵    ��������������1012 D1�������
  */
void Open_Beep()
{
  //HAL_Delay(10);
  int len = 0;
  RS485_Send_Data(D1_Open,(uint8_t)8);
  //HAL_Delay(10);
  
  Clear_RS485Buf();
 
  return;
}

/**
  * ��������: �رշ�����
  * �������: 
  * �� �� ֵ: 
  * ˵    ��������������1012 D1�������
  */
void Close_Beep()
{
   int len = 0;
   RS485_Send_Data(D1_Close,(uint8_t)8);
   Clear_RS485Buf();
   
   return;
}

/**
  * ��������: ���̵�
  * �������: 
  * �� �� ֵ: 
  * ˵    ��������������1012 D0�������
  */
void Open_Light()
{
  int len = 0;
  
  RS485_Send_Data(D0_Open,(uint8_t)8);
  Clear_RS485Buf();
  return;
}

/**
  * ��������: �ر��̵�
  * �������: 
  * �� �� ֵ: 
  * ˵    ��������������1012 D0�������
  */
void Close_Light()
{
    int len = 0;
    
    RS485_Send_Data(D0_Close,(uint8_t)8);
    Clear_RS485Buf();
    return;
}


/**
  * ��������: ����Һ��
  * �������: 
  * �� �� ֵ: 
  * ˵    ������Һ�ý���1055 D2�������
  */
void Open_YiYe_pupm()
{
  
  int len = 0;
  RS485_Send_Data(D2_Open,(uint8_t)8);
  
  Clear_RS485Buf();
  return;
}

/**
  * ��������: �ر���Һ��
  * �������: 
  * �� �� ֵ: 
  * ˵    ������Һ�ý���1055 D2�������
  */
void Close_YiYe_pupm()
{
    int len = 0;
    RS485_Send_Data(D2_Close,(uint8_t)8);
   
    Clear_RS485Buf();
    return;
}


void Clear_FLASH_Data(){
     printf("Clear_FLASH_Data:��ʼ���flash����\n");
     SPI_FLASH_SectorErase(0x0000);
     printf("Clear_FLASH_Data:���flash�������\n");
     return;
}


/*һ�����ϵͳ��ʱ����ʵ�ַ��ж��ӳ٣�����ϵͳ��ʱ��stmtickֻ��24λ���������ʱ��������*/
void delay_ms2(int32_t nms){ 
  int32_t temp; 
  SysTick->LOAD = 8000*nms; 
  SysTick->VAL=0X00;//��ռ����� 
  SysTick->CTRL=0X01;//ʹ�ܣ����������޶����������ⲿʱ��Դ 
  
  do {  
    temp=SysTick->CTRL;//��ȡ��ǰ������ֵ 
  }while((temp&0x01)&&(!(temp&(1<<16))));//�ȴ�ʱ�䵽�� 
 
  SysTick->CTRL=0x00; //�رռ�����    
  SysTick->VAL =0X00; //��ռ�����
}
          

