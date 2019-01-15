/**
  ******************************************************************************
  * �ļ�����: bsp_usartx_RS485.c 
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ���ش��ڵײ���������
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "RS485/bsp_usartx_RS485.h"
#include "variable.h"
#include "function.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
UART_HandleTypeDef husartx_rs485;

//test
int static h = 0;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: NVIC����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void MX_NVIC_USART3_Init(void)
{
  /* USART1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(RS485_USARTx_IRQn,2,0);
  HAL_NVIC_EnableIRQ(RS485_USARTx_IRQn);
}


/**
  * ��������: ���ڲ�������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void RS485_USARTx_Init(void)
{ 
  /* ʹ�ܴ��ڹ�������GPIOʱ�� */
  RS485_USARTx_GPIO_ClK_ENABLE();
  
  husartx_rs485.Instance = RS485_USARTx;
  husartx_rs485.Init.BaudRate = RS485_USARTx_BAUDRATE;
  husartx_rs485.Init.WordLength = UART_WORDLENGTH_8B;
  husartx_rs485.Init.StopBits = UART_STOPBITS_1;
  husartx_rs485.Init.Parity = UART_PARITY_NONE;
  husartx_rs485.Init.Mode = UART_MODE_TX_RX;
  husartx_rs485.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  husartx_rs485.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&husartx_rs485);
  
  /* ���ô����жϲ�ʹ�ܣ���Ҫ����HAL_UART_Init������ִ���޸Ĳ���Ч */
  MX_NVIC_USART3_Init();
}
/**
  * ��������: RS485����len���ֽ�.
  * �������: buf:�������׵�ַ��len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
  * �� �� ֵ: ��
  * ˵    ������
  */
void RS485_Send_Data(uint8_t *buf,uint8_t len){
    //int i = 0;
   // HAL_Delay(100);
    delay_ms2(100);
    //���淢�͵�ַ
    saveRS485Adder = buf[0];
   
    /* ���뷢��ģʽ */
    TX_MODE();
    
    /* ��������,��ѯֱ������������� */
    if(HAL_UART_Transmit(&husartx_rs485,buf,len,0xFFFF)==HAL_OK)
    {
      while(__HAL_UART_GET_FLAG(&husartx_rs485,UART_FLAG_TC)!=1);
      /* �������ģʽ */
        RS485_Rx_Count = 0; 
        RX_MODE();
        //Show_Data(RS485_Rx_buf,20);  
      //  Sample_flag = 1;
      /* ʹ�õ��Դ��ڴ�ӡ������Ϣ�����ڵ������� */
     // Show_Data(buf,8);  
        //printf("\nRS485�������ݳɹ�:"); 
       // Show_Data(buf,len); 
    }
    //HAL_Delay(10);
    delay_ms2(5);
}
/**
  * ��������: RS485��ѯ���յ�������
  * �������: buf:���ջ����׵�ַ��len:���������ݳ���
  * �� �� ֵ: ��
  * ˵    ����
  */
uint16_t RS485_Receive_Data(){
  
	uint16_t len = 0;				//Ĭ��Ϊ0
	
        delay_ms2(1000);
        /*
        
	if(h % 3 == 0){
          RS485_Rx_buf[0] = 0x02;
          RS485_Rx_buf[1] = 0x03;
          RS485_Rx_buf[2] = 0x02;
          RS485_Rx_buf[3] = 0x00;
          RS485_Rx_buf[4] = 0x00;
          RS485_Rx_buf[5] = 0xfc;
          RS485_Rx_buf[6] = 0x44;
        }
    
        else if(h % 2 == 0){
          RS485_Rx_buf[0] = 0x02;
          RS485_Rx_buf[1] = 0x03;
          RS485_Rx_buf[2] = 0x02;
          RS485_Rx_buf[3] = 0x03;
          RS485_Rx_buf[4] = 0x00;
          RS485_Rx_buf[5] = 0xfc;
          RS485_Rx_buf[6] = 0xb4;
        }
        else{
              RS485_Rx_buf[0] = 0x02;
            RS485_Rx_buf[1] = 0x03;
            RS485_Rx_buf[2] = 0x02;
            RS485_Rx_buf[3] = 0xF3;
            RS485_Rx_buf[4] = 0x00;
            RS485_Rx_buf[5] = 0xb8;
            RS485_Rx_buf[6] = 0xb4;
        }
        h++;
       
        RS485_Rx_Count = 7;
         
    */
	
        printf("485�������\n");
        Show_Data(RS485_Rx_buf,10);  
        len = RS485_Rx_Count;
        if(len == 0){
            printf("���ݲɼ�ʧ��\n");
        }
        RS485_Rx_Count_Old = RS485_Rx_Count;
        RS485_Rx_Count = 0; 
        //HAL_Delay(10);
        delay_ms2(5);
        //Sample_flag = 0;  
        printf("���ճ��ȣ�%d\n",len);
 //}//while
        return len;
}



