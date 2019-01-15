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
#include <stdlib.h>
    
/* ˽�����Ͷ��� --------------------------------------------------------------*/

__IO uint16_t minute_timer_count = 0;//��
__IO uint16_t second_timer_count = 0;//��

uint16_t count = 0;
uint8_t error = 0;
uint8_t error_num = 0;
uint8_t recv_flag_save = 0;
uint8_t keyTimeFlag = 0;

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
uint8_t debug_test = 1;
uint8_t aRxBuffer= 0;
uint8_t aRxBuffer_485 = 0;
int8_t frist = -1;
uint8_t RevDevicesData = 0;
uint8_t RevCommand = 0;
uint8_t com_flag = 0;
uint8_t ret = HAL_OK;
uint8_t Rx_Count = 0;
uint8_t aRxBuffer_Android = 0;
uint8_t recv_flag_num = 0;
/* ˽�к���ԭ�� --------------------------------------------------------------*/
void SystemClock_Config(void);
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ������.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
int main(void)
{
  
    int i = 0;
    char start_time[20];
    uint8_t info[5];
    RTC_TimeTypeDef stimestructureget;
    RTC_DateTypeDef sdatestructureget;
    
    // ��λ�������裬��ʼ��Flash�ӿں�ϵͳ�δ�ʱ�� 
    HAL_Init();
    //����ϵͳʱ�� 
    SystemClock_Config();
    MX_SPIFlash_Init();
    MX_USARTx_Init();
    //��ʼ��RTCʵʱʱ�� 
    MX_RTC_Init();
    
    RS485_USARTx_Init();
    // ����LED��ʼ�� 
    LED_GPIO_Init();
    //���������ʼ�� 
    OUTPUT_GPIO_Init();
    //�����ʼ��
    INPUT_GPIO_Init();
    //������ʱ����ʼ����1ms�ж�һ�� 
    BASIC_TIMx_Init();
    MX_DEBUG_USART_Init();
    printf("Զ�����ǡ�������5.11\n");
      
    //ʹ�ܽ��գ������жϻص����� 
    HAL_UART_Receive_IT(&husart_debug,&aRxBuffer_Android,1);
    HAL_UART_Receive_IT(&husartx_rs485,&aRxBuffer_485,1);

    //����485����
    RX_MODE();
    //��ʾ��ʼʱ��
    RTC_CalendarShow();
    //���Ͱ汾��
    sprintf(info,"|24");
    HAL_UART_Transmit(&husart_debug,info,strlen((char *)info),1000); 
    //�رյơ���Һ��
    Close_YiYe_pupm();
    Close_Light();
    Close_Beep();
  
    //��ȡflash����
    Get_Device_Data(Android_Rx_buf);
    
    //����Ĭ������
    SCM_state = SCM_RUN;
    Open_Light();
    LED1_ON;
    
    //���ж�ģʽ��������ʱ�� 
    HAL_TIM_Base_Start_IT(&htimx);
    
    //����������ѭ��
    while (1)
    {
        //��ʼ�ɼ�
        if(Sample_flag)
        {
           LED2_TOGGLE;
           Sample_RS485();
           Sample_flag = 0;
        }
        //���յ���׿��������Ϣ
        if(RevDevicesData)
        {
            //printf("��׿��%s\n",Android_Rx_buf);
            //�Ƴ�frist����ʱ��
            frist = -1;
            //��ʱ������
            second_timer_count = 0;
            minute_timer_count = 0;;
            time_num = 0;
            //��������
            Save_Data();
            //�����־λ
            UpData_flag = 0;
            RevDevicesData = 0;
            FLAG = 0;
        }
        //���յ���׿��������Ϣ
        if(RevCommand)
        {
            Command_Data();  
            RevCommand= 0;
            FLAG = 0;
        }
        //���յ�����ָ��
        if(Reboot_flag)
        {
            //RTC_CalendarShow();
            FLAG = 1;
            Close_Delay('1');
            Close_Delay('2');
            Close_Delay('3');
            Close_Delay('4');
            Close_Delay('5');
            Close_YiYe_pupm();
            Close_Light();
            Close_Beep();
            if(!error)
              HAL_UART_Transmit(&husart_debug,"&",1,1000);
            FLAG = 0;
            Reboot();
            Reboot_flag = 0; 
        }
        //�ϴ�����
        if(UpData_flag)
        {
            //RTC_CalendarShow();
            FLAG = 1;
            LED3_TOGGLE;
            //RTC_CalendarShow();
            Get_Average();
            UpData();
            Control();
            Detection();
            UpData_flag = 0;
            FLAG = 0;
        }
        if(Control_flag && frist == 1 && Android_Rx_Count <= 5)
        {
            FLAG = 1;
            //��һ���Ӳ�����
            printf("��ʼ�����߼�\n");
            LED3_TOGGLE;
            Control();
            Control_flag = 0;
            FLAG = 0;
        }
        //����ʱ��
        if(RTC_Config_flag)
        {
           RTC_Config_flag = 0;
           RTC_CalendarConfig(Android_Rx_buf);
           RTC_CalendarShow();
           FLAG = 0;
        }

        //ֹͣ���������ָ�״̬
        if(Save_flag)
        {
            //RTC_CalendarShow();
            FLAG = 1;
            if(SCM_state == SCM_RUN)
            {
                Open_Light();
                for(i=0;i<5;i++)
                {
                  if(delay[i].control != 0 && delay[i].state == 1)
                  {
                      Open_Delay(delay[i].port);
                  }
                }
                if(YiYe_pump_flag)
                {
                    Open_YiYe_pupm();
                }
            }
            else
            {
                Close_Delay('1');
                Close_Delay('2');
                Close_Delay('3');
                Close_Delay('4');
                Close_Delay('5');
                Close_YiYe_pupm();
                Close_Light();
                Close_Beep();
            }
            Save_flag = 0;
            FLAG = 0;
        }
        
        //������Һ��
        if(YiYe_pump_control_flag)
        {
            if(YiYe_pump_flag)
            {
                Open_YiYe_pupm();
            }
            else
            {
                Close_YiYe_pupm();
            }
            YiYe_pump_control_flag = 0;
        }
        //����澯
        if(Error_flag)
        {
            Close_Error(Android_Rx_buf[0]);
            Error_flag = 0;
            FLAG = 0;
        }
        //���flash�ڴ�
        if(clear_flag)
        {
            Clear_FLASH_Data();
            clear_flag = 0;
        }
        //���յ���׿����ʱ�򲻽��вɼ��Ȳ���
        if(Android_Rx_Count <= 5){
            //3s�Ӳɼ�һ��
            if(second_timer_count % 395 >= 390)
            {
                //��������Ϣ�ɼ�
                if(Sensor_Cfg_Mode)
                {
                    Sample_flag = 1;
                }     
                if(keyTimeFlag)
                {
                    keyTimeFlag = 0;
                }
            }
            if(second_timer_count >= 1007)
            {
                second_timer_count = 0; 
                //10�뷢��һ������        
                sprintf(info,"@%d",SCM_state);
                HAL_UART_Transmit(&husart_debug,info,strlen((char *)info),1000); 
                printf("��������\n");
                //����Ƿ���ܰ�׿���ݲ�����
                if(recv_flag_save == Android_Rx_Count){
                    recv_flag_num ++;
                    if(recv_flag_num == 2){
                      Android_Rx_Count = 0;
                      recv_flag_num = 0;
                    }
                }
            }
            if(Delay_Cfg_Mode && minute_timer_count % 914 >= 913)
            {
                Control_flag = 1;
            }
            //1�����ϴ�һ��
             if(minute_timer_count >= 6011)
             {
               // LED3_TOGGLE;
                minute_timer_count = 0;
                    //��������Ϣ�ɼ�
                if(Sensor_Cfg_Mode)
                {
                    UpData_flag = 1;
                    if(frist <= 0)
                    {
                        frist ++;
                    }
                }
             }
        }//if

         for(i=0;i<5;i++)
         {
            //if(delay[i].devices != 0 && delay[i].control >= 3 && delay[i].state == 1)
            if(delay[i].control >= 3 && (time_Control_flag || sys_time_Control_flag) && delay[i].control != 5)
            {
              //��ȡ��ǰʱ�� 
              HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
              sprintf(start_time,"%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes);   
              HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
              if(strcmp(delay[i].start_time,start_time) == 0)
              {
                  printf("��ʱ��ʼ���򿪱�%d\n",i+1);
                  //ʱ�䵽
                  strcpy(delay[i].start_time,"00:00");
                  Open_Delay(delay[i].port);
              }
              else if(delay[i].counter <= 10 )               
              {
                    Close_Delay(delay[i].port);
                    if(delay[i].state == 1){
                      //��������
                      printf("��ʱ�������رձ�%d\n",i+1);
                      if(delay[i].control == 3)
                      {
                          delay[i].counter = delay[i].save_counter;
                          Chang_Start_time(i);
                       }
                       else if(delay[i].control == 6){
                          printf("�̵����رգ�ģʽ6���Ʒ�ʽ������Ҫ%d\n",time_num);
                       }
                       else if(delay[i].control == 4){
                            delay[i].counter = 1000;
                       }
                     }
                     else{
                        delay[i].counter = 1000;
                     }
                     Control();
                }
                else {}
            }
            if(time_num >= 6000 * 30)
            {
                if(com_flag)
                {
                  com_flag = 0;
                }
            else
            {
                printf("��ʱ��δ�յ���׿ͨ����Ϣ���Զ�����\n");
                Reboot_flag = 1;
                error = 1;
            }
               time_num = 0;
               printf("30���Ӽ�ʱ���������¿�ʼģʽ6���Ʒ�ʽ\n");
               index_time_control_flag = 0;
            }
        }
           
        //����̫���������
       if(Rx_Count >= 200){
          Rx_Count = 0;
          FLAG = 0;
        }
        if(Android_Rx_Count >= 900) 
        {
           Android_Rx_Count = 0;
        }
        if(RS485_Rx_Count >= 49) 
        {
           RS485_Rx_Count = 0;
        }
        //�澯��ʱ
        if(warn_timer_count <= 2){
           Close_Beep();
           Open_Light();
           warn_timer_count = 0;
        }
    }//while
}//mian

/**
  * ��������: ���ڽ�����ɻص�����
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ������
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  //��׿�˽���
  if(UartHandle->Instance == DEBUG_USARTx){
    
    if(FLAG){
      printf("��׿���յ����������ݣ�%c\n",aRxBuffer);
      //HAL_UART_Receive_IT(&husart_debug,&aRxBuffer,1);
      Rx_Count++;
      Android_Rx_Count = 0;
      if(aRxBuffer == '{'){
        FLAG = 0;
        aRxBuffer_Android = '{';
      }
    }
    else if(Android_Rx_Count < 1000){
        printf("��׿���ձ������ݣ�%c\n",aRxBuffer_Android);
        if(Android_Rx_Count != 0 || aRxBuffer_Android != 0x20){
          Android_Rx_buf[Android_Rx_Count] = aRxBuffer_Android;
          Android_Rx_Count++;
        }
        recv_flag_save = Android_Rx_Count; 
        switch(aRxBuffer_Android){
          case '#': 
            FLAG = 1;
            RevDevicesData = 1;
            Android_Rx_buf[Android_Rx_Count - 1] = '\0';
            printf("��׿������Ϣ���������\n");
            Android_Rx_Count = 0;
            //HAL_NVIC_EnableIRQ(BASIC_TIM_IRQ);
            break;
          case '!': 
            FLAG = 1;
            RevCommand = 1;
            Android_Rx_buf[Android_Rx_Count - 1] = '\0';
            printf("��׿������Ϣ���������\n");
            Android_Rx_Count = 0;
            break;
         //�������
          case '$':
            if(Android_Rx_Count < 5)
            {
              FLAG = 1;
              Android_Rx_buf[Android_Rx_Count - 1] = '\0';
              Error_flag = 1;
              printf("��׿�����Ϣ���������\n");
              Android_Rx_Count = 0;
            }
            break;
          case ':' :
            if(Android_Rx_Count <= 20 && Android_Rx_Count >= 15 && Android_Rx_Count <= 17)
            {
              FLAG = 1;
              Android_Rx_buf[Android_Rx_Count - 1] = '\0';
              RTC_Config_flag = 1;
              printf("��׿����ʱ����Ϣ���������\n");      
              Android_Rx_Count = 0;
            }
            break;
          case '{':
          //ͨ��
            HAL_UART_Transmit(&husart_debug,"{",1,1000);
            com_flag = 1;
            Android_Rx_buf[Android_Rx_Count - 1] = '\0';       
            printf("��׿ͨ����Ϣ���������\n");
            Android_Rx_Count = 0;
            break;
        case '&':
          //����ϵͳ
          if(Android_Rx_Count < 5)
          {
            FLAG = 1;
            printf("ϵͳ׼�����������³���\n");
            Reboot_flag = 1;
          }
          break;
         case '%':
          //����ϵͳ
          if(Android_Rx_Count < 5)
          {
            FLAG = 1;
            printf("ϵͳ׼������\n");
            error = 1;
            Reboot_flag = 1;
          }
          break;
        case '@':
          if(Android_Rx_Count < 5)
          {
            clear_flag = 1;
            printf("��׿�����Ƭ��FLASH�������\n\n");
            Android_Rx_Count = 0;
          }
          break;
        default:
          break;
       }
     }
    else{
      printf("����������\n");
      Android_Rx_Count = 0;
    }
    HAL_UART_Receive_IT(&husart_debug,&aRxBuffer_Android,1);
  /*
     do{
          if(UartHandle->RxState == HAL_UART_STATE_READY && !FLAG){
            HAL_UART_Receive_IT(&husart_debug,&aRxBuffer_Android,1);
            break;
          }
          error_num++;
          if(error_num >=10){
            error_num = 0;
            printf("232���ڶ������Զ�����\n");
            //HAL_UART_Receive(&husart_debug,(uint8_t *)aRxBuffer_Android,500,1000);
            Reboot_flag = 1;
            error = 1;
            break;
          }
     }while(1);
    */
   }
  //485����
  else if(UartHandle->Instance == RS485_USARTx)
  {
    //printf("\n485:%02x\n",aRxBuffer);
    if(aRxBuffer_485 == saveRS485Adder && RS485_Rx_Count == 1){
      RS485_Rx_buf[RS485_Rx_Count] = aRxBuffer_485;
      RS485_Rx_Count ++;
    }
    else{
      RS485_Rx_Count = 0;
    }
    do{
          if(UartHandle->RxState == HAL_UART_STATE_READY){
            HAL_UART_Receive_IT(&husartx_rs485,&aRxBuffer_485,1);
            break;
          }
          error_num++;
          if(error_num >=10){
            error_num = 0;
            printf("485���ڶ������Զ�����\n");
            Reboot_flag = 1;
            error = 1;
            break;
          }
     }while(1);
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
    //�ɼ���ʱ
    second_timer_count++;
    minute_timer_count++;
    time_num++;
    if(warn_timer_count != 0){
      warn_timer_count --;
    }
    for(int i=0;i<5;i++)
     {
        //if(delay[i].devices != 0 && delay[i].control >= 3 && delay[i].state == 1)
       if(delay[i].control >= 3 &&(time_Control_flag || sys_time_Control_flag))
        {
          
            if( delay[i].state == 1 && delay[i].counter != 0)
              delay[i].counter --;
            else if(delay[i].counter != delay[i].save_counter)
              delay[i].counter = 1000;
            else{}
        }
     }
}

/**
  * ��������: �����ⲿ�жϷ�����
  * �������: GPIO_Pin���ж�����
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  printf("��⵽�����ƽ  %x\n",GPIO_Pin);
 
 // HAL_Delay(20);
  
  if(GPIO_Pin == INPUT11_GPIO_PIN)//��
  {
      LED1_ON;
      if(SCM_state != SCM_RUN){
        SCM_state = SCM_RUN;
        Save_flag = 1;
        HAL_TIM_Base_Start_IT(&htimx);
      }
      //printf("�˿ڣ�11��״̬%d\n",INPUT_StateRead(11));
      
    __HAL_GPIO_EXTI_CLEAR_IT(INPUT11_GPIO_PIN);
  }
  else if(GPIO_Pin == INPUT12_GPIO_PIN)//��
  {
      LED1_OFF;
      if(SCM_state != SCM_STOP){
        SCM_state = SCM_STOP;
        Save_flag = 1;
        HAL_TIM_Base_Stop(&htimx);
      }
     // printf("�˿ڣ�12��״̬%d\n",INPUT_StateRead(12));
    __HAL_GPIO_EXTI_CLEAR_IT(INPUT12_GPIO_PIN);
  }
  else if(GPIO_Pin == INPUT10_GPIO_PIN)//��Һ��
  {
    //keyTimeFlag = 1;
    if(!keyTimeFlag){
      YiYe_pump_control_flag = 1;
      YiYe_pump_flag = YiYe_pump_flag ^ 0xFE;
      keyTimeFlag = 1;
     // printf("�˿ڣ�13��״̬%d,��Һ�ÿ���״̬%d\n",INPUT_StateRead(13),YiYe_pump_flag);
    }
   
    __HAL_GPIO_EXTI_CLEAR_IT(INPUT10_GPIO_PIN);
  }
   else if(GPIO_Pin == INPUT9_GPIO_PIN)//��ͣ
  {
      LED1_OFF;
      if(SCM_state != SCM_STOP){
        SCM_state = SCM_STOP;
        Save_flag = 1;
        HAL_TIM_Base_Stop(&htimx);
      }
     // printf("�˿ڣ�14��״̬%d\n",INPUT_StateRead(14));
    __HAL_GPIO_EXTI_CLEAR_IT(INPUT9_GPIO_PIN);
  }
  
  
}
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


