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
  HAL_NVIC_SetPriority(RS485_USARTx_IRQn,1,0);
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
    /* ���뷢��ģʽ */
    TX_MODE();
    /* ��������,��ѯֱ������������� */
    if(HAL_UART_Transmit(&husartx_rs485,buf,len,0xFFFF)==HAL_OK)
    {
      while(__HAL_UART_GET_FLAG(&husartx_rs485,UART_FLAG_TC)!=1);
      /* �������ģʽ */
        RX_MODE();
        Show_Data(RS485_Rx_buf,20);  
      //  Sample_flag = 1;
      /* ʹ�õ��Դ��ڴ�ӡ������Ϣ�����ڵ������� */
     // Show_Data(buf,8);  
        printf("\nRS485�������ݳɹ�:"); 
        Show_Data(buf,len); 
    }
}
/**
  * ��������: RS485��ѯ���յ�������
  * �������: buf:���ջ����׵�ַ��len:���������ݳ���
  * �� �� ֵ: ��
  * ˵    ����
  */
void RS485_Receive_Data(uint8_t *len){
  
	*len=0;				//Ĭ��Ϊ0
	
    //if(Sample_flag == 1){
   // while(1){
        printf("���ݲɼ���\n");
        HAL_Delay(500);//20ms����
        //if(RS485_Rx_Count == RS485_Rx_Count_Old){
        printf("�������������\n");
        Show_Data(RS485_Rx_buf,20);  
        printf("���ճ��ȣ�%d\n",RS485_Rx_Count);
        *len = RS485_Rx_Count;
        if(*len == 0){
            printf("���ݲɼ�ʧ��\n");
        }
        RS485_Rx_Count_Old = RS485_Rx_Count;
        RS485_Rx_Count = 0; 
        //Sample_flag = 0;    
 //}//while
}



