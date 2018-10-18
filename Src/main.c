/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: RS-485ʵ������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "usart/bsp_debug_usart.h"
#include "RS485/bsp_usartx_RS485.h"
#include "led/bsp_led.h"
#include "BasicTIM/bsp_BasicTIM.h"
#include "string.h"
#include "function.h"
#include "sample.h"
#include "gpio/bsp_gpio.h"
#include "variable.h"
#include "rtc/bsp_rtc.h"
#include "spiflash/bsp_spiflash.h"

    
/* ˽�����Ͷ��� --------------------------------------------------------------*/

__IO uint16_t minute_timer_count = 0;//��
__IO uint16_t second_timer_count = 0;//��

__IO uint16_t count = 0;

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/

uint8_t aRxBuffer = 0;
uint8_t frist = 0;
uint8_t RevDevicesData = 0;
uint8_t RevCommand = 0;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ϵͳʱ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
 
  __HAL_RCC_PWR_CLK_ENABLE();                                     //ʹ��PWRʱ��

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);  //���õ�ѹ�������ѹ����1

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;      // �ⲿ����8MHz
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                        //��HSE 
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;                    //��PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;            //PLLʱ��Դѡ��HSE
  RCC_OscInitStruct.PLL.PLLM = 8;                                 //8��ƵMHz
  RCC_OscInitStruct.PLL.PLLN = 336;                               //336��Ƶ
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;                     //2��Ƶ���õ�168MHz��ʱ��
  RCC_OscInitStruct.PLL.PLLQ = 7;                                 //USB/SDIO/������������ȵ���PLL��Ƶϵ��
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;       // ϵͳʱ�ӣ�168MHz
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;              // AHBʱ�ӣ� 168MHz
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;               // APB1ʱ�ӣ�42MHz
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;               // APB2ʱ�ӣ�84MHz
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_RCC_EnableCSS();                                            // ʹ��CSS���ܣ�����ʹ���ⲿ�����ڲ�ʱ��ԴΪ����
  
 	// HAL_RCC_GetHCLKFreq()/1000    1ms�ж�һ��
	// HAL_RCC_GetHCLKFreq()/100000	 10us�ж�һ��
	// HAL_RCC_GetHCLKFreq()/1000000 1us�ж�һ��
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);                // ���ò�����ϵͳ�δ�ʱ��
  /* ϵͳ�δ�ʱ��ʱ��Դ */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* ϵͳ�δ�ʱ���ж����ȼ����� */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */

int main(void)
{
  
    int i =0;
    char start_time[20];
    RTC_TimeTypeDef stimestructureget;
    
  
  /* ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� */
  HAL_Init();
  /* ����ϵͳʱ�� */
  SystemClock_Config();
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_DEBUG_USART_Init();
  
  /* ��ʼ�����ڲ����ô����ж����ȼ� */
  MX_USARTx_Init();
  
  MX_SPIFlash_Init();
  
  /* ��ʼ��RTCʵʱʱ�� */
  MX_RTC_Init();
  
  RS485_USARTx_Init();
  /* ����LED��ʼ�� */
  LED_GPIO_Init();
    /* ���������ʼ�� */
  OUTPUT_GPIO_Init();
  
  
  
  /* ������ʱ����ʼ����1ms�ж�һ�� */
  BASIC_TIMx_Init();
  
  /* ���ж�ģʽ��������ʱ�� */
  HAL_TIM_Base_Start_IT(&htimx);
 
  
  printf("����\n");
  
   
   /* ʹ�ܽ��գ������жϻص����� */
  HAL_UART_Receive_IT(&husart_debug,&aRxBuffer,1);
  HAL_UART_Receive_IT(&husartx_rs485,&aRxBuffer,1);
  HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
  
  RX_MODE();
  LED2_ON;
  
  RTC_CalendarShow();
 
  //��ȡ����
  Get_Device_Data(Android_Rx_buf);
  
  while (1)
  {
    if(RevDevicesData){
      if(Debug_flag){

            Save_Device_Data(RS232_Rx_buf);
      }
      else{
            Save_Device_Data(Android_Rx_buf);
      }
        Save_Data();
        
        RevDevicesData= 0;
    }
    if(RevCommand){
        Command_Data();  
        RevCommand= 1;
    }
   if(Sample_flag){
        
       Sample_RS485();
        Sample_flag = 0;
    }
    if(UpData_flag){
        //Get_Device_Data(Android_Rx_buf);
        RTC_CalendarShow();
        Get_Average();
        UpData();
        UpData_flag = 0;
    }
    if(Control_flag && frist){
    //��һ���Ӳ�����
        Control();
        Control_flag = 0;
        
     }
    if(time_Control_flag || sys_time_Control_flag){
          
        /* ��ȡ��ǰʱ�� */
        HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  
        /* ��ʾʱ�� */
        sprintf(start_time,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);
        
        //printf("ʱ��=============%02d:%02d\n", stimestructureget.Hours, stimestructureg                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             et.Minutes);
        for(i=0;i<4;i++){
            if(delay[i].devices != 0){
                //printf("vvvvvvvvvvvv%d\n",strcmp(delay[i].start_time,start_time));
                if(strcmp(delay[i].start_time,start_time) == 0){
                    //ʱ�䵽
                    strcpy(delay[i].start_time,"00:00");
                   // printf("ʱ��=============%s\n",start_time);
                    if(delay[i].state == 0){
                        Open_Delay(delay[i].port);
                    }
                }

                if(delay[i].counter <= 0){
                    //��������
                    delay[i].counter = delay[i].save_counter;
                    if(delay[i].state == 1){
                        Close_Delay(delay[i].port);
                    }
                    if(delay[i].control == 3){
                        Chang_Start_time(i);
                    }

                }
    
            }
        }

    }
  }
 }

/**
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
   //232����
  if(UartHandle->Instance == USARTx){
   
    RS232_Rx_buf[RS232_Rx_Count] = aRxBuffer;
    RS232_Rx_Count ++;
    //�������
    if(aRxBuffer == '#'){
        printf("RS232������Ϣ���������\n");
        RS232_Rx_buf[RS232_Rx_Count-1] = '\0';
        RS232_Rx_Count = 0;
       // Save_Data();
       // Sensor_Cfg_Mode = 1;
         Debug_flag = 1;
         RevDevicesData = 1;
         RevComplete = 1;
      //  Sample_RS485();
    }
      if(aRxBuffer == '!'){
        printf("RS232������Ϣ���������\n");
        RS232_Rx_buf[RS232_Rx_Count-1] = '\0';
        RS232_Rx_Count = 0;
        //Command_Data();
       // Sensor_Cfg_Mode = 1;
        Debug_flag = 1;
        RevCommand = 1;
        RevComplete = 1;
      //  Sample_RS485();
    }
       if(aRxBuffer == '?'){
        printf("RS232���������\n");
        RS485_Send_Data(BHZY_Conductivity,8);
        Debug_flag = 1;
        Sample_flag = 1;
        
    }
    HAL_UART_Receive_IT(&husartx,&aRxBuffer,1);
  }
  //485����
  else if(UartHandle->Instance == RS485_USARTx && Sample_flag == 1){
//  else if(UartHandle->Instance == USARTx){
   // printf("RS485:%02x\n",aRxBuffer);
    RS485_Rx_buf[RS485_Rx_Count] = aRxBuffer;
    RS485_Rx_Count ++;
    //printf("\n%d %d\n",aRxBuffer,Rx_Count); 
    HAL_UART_Receive_IT(&husartx_rs485,&aRxBuffer,1);
  }
  //��׿�˽���
  else if(UartHandle->Instance == DEBUG_USARTx){
   // printf("adada");
   // HAL_UART_Transmit(&husartx,&aRxBuffer,1,0);

    if(Android_Rx_Count != 0 || aRxBuffer != 0x20){
          Android_Rx_buf[Android_Rx_Count] = aRxBuffer;
          //printf("aaaaaaaaaa\n%d  %c  %02x\n",Android_Rx_Count,aRxBuffer,aRxBuffer);
          Android_Rx_Count ++;
    }
    //�������
    if(aRxBuffer == '#'){
        printf("��׿������Ϣ���������\n");
        Android_Rx_buf[Android_Rx_Count-1] = '\0';
        printf("%s\n",Android_Rx_buf);
        Android_Rx_Count = 0;
        //Save_Data();
        RevDevicesData = 1;
    }
      if(aRxBuffer == '!'){
        printf("��׿������Ϣ���������\n");
        Android_Rx_buf[Android_Rx_Count-1] = '\0';
        printf("%s\n",Android_Rx_buf);
        Android_Rx_Count = 0;
        
       // Command_Data();
        RevCommand = 1;
    }
    HAL_UART_Receive_IT(&husart_debug,&aRxBuffer,1);
  }
 
}

/**
  * ��������: ������ģʽ�¶�ʱ���Ļص�����
  * �������: htim����ʱ�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

    int i;
    //�ɼ���ʱ
    second_timer_count++;
  
    minute_timer_count++;
    
    
    //1s�Ӳɼ�һ��
    LED1_TOGGLE;
    if(second_timer_count == 2567)
    {
        second_timer_count = 0;
     
        LED2_TOGGLE;
      
        //��������Ϣ�ɼ�
        if(Sensor_Cfg_Mode){
            Sample_flag = 1;
            if(Delay_Cfg_Mode){
            // Control();
                Control_flag = 1;

            }
        }
        
    }
 
    //1�����ϴ�һ��
     if(minute_timer_count == 60000)
     {
        
        LED3_TOGGLE;
        minute_timer_count = 0;
            //��������Ϣ�ɼ�
        if(Sensor_Cfg_Mode){
        UpData_flag = 1;
            if(!frist){
                frist = 1;
            }
        }
     }
    for(i=0;i<4;i++){
        if(delay[i].devices != 0 && delay[i].state == 1){
            delay[i].counter --;
        }
    }
    
}


/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
