#include "sample.h"
#include "variable.h"
#include "string.h"
#include "function.h"
#include "RS485/bsp_usartx_RS485.h"
#include "crc16.h"
#include <ctype.h>




/**
  * ��������: ��str�ַ���spl�ָ�,����dst�У�������
  * �������: Ŀ���ַ��� dst��Դ�ַ��� str �ָ��ַ��� spl 
  * �� �� ֵ: ���ַ�������
  * ˵    ������
  */ 
int split(char dst[][80], char* str, const char* spl)
{
    int n = 0;
    char *result = NULL;
    result = strtok(str, spl);
    
    while( result != NULL )
    {
        strcpy(dst[n++], result);
        result = strtok(NULL, spl);
    }
    return n;
}

/**
  * ��������: ��ʮ�����Ƶ��ַ���ת�������� 
  * �������: ʮ�������ֽ���s 
  * �� �� ֵ: ת����� 
  * ˵    ������
  */ 
 
int htoi(char s[])  
{  
    int i = 0;  
    int n = 0;  
    if (s[0] == '0' && (s[1]=='x' || s[1]=='X'))  
    {  
        i = 2;  
    }  
    else  
    {  
        i = 0;  
    }  
    for (; (s[i] >= '0' && s[i] <= '9') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >='A' && s[i] <= 'Z');++i)  
    {  
        if (tolower(s[i]) > '9')  
        {  
            n = 16 * n + (10 + tolower(s[i]) - 'a');  
        }  
        else  
        {  
            n = 16 * n + (tolower(s[i]) - '0');  
        }  
    }  
    return n;  
} 
/**
  * ��������: �ɼ�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    �����ɼ����ݣ�error������������������Χ�ʹ�������������ʧ�ܣ�ѭ���ɼ��������õĴ�����
  */
void Sample_RS485(void)
{
  char dst[20][80] = {0};
  int n = 0,i = 0,j = 0,e = 0;
  int cnt = 0,t = 0;
  unsigned char command[50] = {0};
  Sensor *p = NULL;
  uint8_t len = 0;
 
  for(i=0;i<6;i++)
  {
    //�д�����
   // if(sensor_array[i].num > 0){
    //�ɼ���ʱ���ж�Һλ���ɼ�ģʽ��Һλ�ɼ�ģʽ��Ϊģ��ɼ������ֲɼ���
    //���ݹ������жϣ����Ϊ30��Ϊģ��ɼ�

    p = sensor_array[i].frist_node;
    for(j=0;j<sensor_array[i].num;j++)
    {
      printf("Sample_RS485:collection cmd:%s\n",p->command);

        if(strlen(p->command)>0)
        {
            strcpy(command,p->command);
            cnt = split(dst,command, ".");
            for (e = 0; e < cnt; e++)
            {
                n = htoi(dst[e]);
               // Bit.i = n;
                command[e] = (char)n;
            }
            if(i == 4)
            {
            //Һλ        
              if(command[1] == 0x30)
              {
                //ģ��ɼ�
                t = (int)command[0];
                if(Simulation_Level(t))
                {
                  p->amount++;
                  p->value++;
                  
                }
                else
                {
                  p->error++;
                } 
                continue;
              }
    
            }
            RS485_Send_Data(command,cnt);
            RS485_Receive_Data(&len);
            //printf("Send num:%d\n ",cnt);
            if(len > 0)
            {
              //��������
                Modbusprocess(RS485_Rx_buf,p,sensor_array[i].type);
            }
            else
            {
                p->error++;
            }
        
        }//if
        p = p->next; 
    }//for
    //�ɼ�����
  }//for
}



