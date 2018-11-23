/**
  ******************************************************************************
  * �ļ�����: bsp_rtc.c 
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: RTCʵʱʱ��
  ******************************************************************************
  */

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "rtc/bsp_rtc.h"
#include "usart/bsp_debug_usart.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
//ʱ��
RTC_HandleTypeDef hrtc;
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ���õ�ǰʱ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void RTC_CalendarConfig(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;
#if 1           //�ô�ѡ���Ƿ��Լ�����ʱ�ӣ�������Լ����ã���ô���ó�0 
  /* �������� */
  /* �������ڣ�2018��09��05�� ������ */
  DateToUpdate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  DateToUpdate.Month = RTC_MONTH_SEPTEMBER;
  DateToUpdate.Date = 0x05;
  DateToUpdate.Year = 0x18;  
  HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BCD);
  
  /* ����ʱ�� */
  /* ʱ��ʱ�䣺10:15:46 */
  sTime.Hours = 0x19;
  sTime.Minutes = 0x16;
  sTime.Seconds = 0x00;
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
#else
  __IO uint32_t Tmp_YY = 0xFF, Tmp_MM = 0xFF, Tmp_DD = 0xFF, Tmp_XX = 0xFF, Tmp_HH = 0xFF, Tmp_MI = 0xFF, Tmp_SS = 0xFF;

  printf("=========================ʱ������==================\n");
  printf("���������: 20\n");
  while (Tmp_YY == 0xFF)
  {
    Tmp_YY = USART_Scanf(99);
  }
  printf("��ݱ�����Ϊ:  20%0.2d\n", Tmp_YY);

  DateToUpdate.Year = Tmp_YY;
  
  printf("�������·�:  \n");
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(12);
  }
  printf("�·ݱ�����Ϊ:  %d\n", Tmp_MM);

  DateToUpdate.Month= Tmp_MM;

  printf("����������:  \n");
  while (Tmp_DD == 0xFF)
  {
    Tmp_DD = USART_Scanf(31);
  }
  printf("���ڱ�����Ϊ:  %d\n", Tmp_DD);
  DateToUpdate.Date= Tmp_DD;
  
  printf("����������:  \n");
  while (Tmp_XX == 0xFF)
  {
    Tmp_XX = USART_Scanf(36);
  }
  printf("���ڱ�����Ϊ:  %d\n", Tmp_XX);
  DateToUpdate.WeekDay= Tmp_XX;
    
  HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BIN);
  
  printf("������ʱ��:  \n");
  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
  }
  printf("ʱ�ӱ�����Ϊ:  %d\n", Tmp_HH );
  sTime.Hours= Tmp_HH;


  printf("���������:  \n");
  while (Tmp_MI == 0xFF)
  {
    Tmp_MI = USART_Scanf(59);
  }
  printf("���ӱ�����Ϊ:  %d\n", Tmp_MI);
  sTime.Minutes= Tmp_MI;

  printf("����������:  \n");
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
  }
  printf("���ӱ�����Ϊ:  %d\n", Tmp_SS);
  sTime.Seconds= Tmp_SS;
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
#endif

  /* д��һ����ֵ��0x32F1��RTC�������ݼĴ���1 */
  HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR1, 0x32F1);
}

/**
  * ��������: RTCʵʱʱ�ӳ�ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void MX_RTC_Init(void)
{
  /* ��ʼ��RTCʵʱʱ�Ӳ�����ʱ������� */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;  //24Сʱ��
  hrtc.Init.AsynchPrediv = 127;              //�첽����
  hrtc.Init.SynchPrediv = 255;               //ͬ������
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;     //��ʹ��RTC_ALARM����ı�־  
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;  //RTC_ALARM�������Ϊ��
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;     //RTC_ALARM�������Ϊ��©���
  HAL_RTC_Init(&hrtc);

#if 0
  /* ����RTC��������ʱ������� */
  RTC_CalendarConfig();
#else
  /* ��������Ƿ񱣴���RTC���ݼĴ���1������Ѿ�����Ͳ���Ҫ�������ں�ʱ������ */
  /* ��ȡ���ݼĴ���1���� */
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F1)
  {
    /* ����RTC��������ʱ������� */
    RTC_CalendarConfig();
  }
  else
  {
    /* ����ϵ縴λ��־λ�Ƿ�Ϊ��SET */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      printf("�����ϵ縴λ������\n");
    }
    /* ������Ÿ�λ��־λ�Ƿ�Ϊ��SET */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      printf("�����ⲿ���Ÿ�λ������\n");
    }
    /* �����λԴ��־λ */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
#endif
}

/**
  * ��������: RTCʵʱʱ�ӳ�ʼ��
  * �������: hrtc��RTC������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ���������
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  if(hrtc->Instance==RTC)
  {
    /* To change the source clock of the RTC feature (LSE, LSI), You have to:
       - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
       - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
         configure the RTC clock source (to be done once after reset).
       - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
         __HAL_RCC_BACKUPRESET_RELEASE().
       - Configure the needed RTc clock source */

    HAL_PWR_EnableBkUpAccess();
    
    /* ʹ�ܱ���ʱ�ӣ����ݼĴ��� */
  //  __HAL_RCC_BKP_CLK_ENABLE();
    
    /* �����ⲿ����ʱ��ΪRTCʱ��Դ */
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;             //RTCʹ��LSE
    HAL_RCC_OscConfig(&RCC_OscInitStruct);
    
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC; //����ΪRTC
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE; //RTCʱ��ԴΪLSE
    HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);
    
    /* ʹ��RTC����ʱ�� */
    __HAL_RCC_RTC_ENABLE();
  }
  
}

/**
  * ��������: RTCʵʱʱ�ӷ���ʼ��
  * �������: hrtc��RTC������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ���������
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{

  if(hrtc->Instance==RTC)
  {
    /* ����RTCʱ�� */
    __HAL_RCC_RTC_DISABLE();

    /* ����PWRʱ�ӺͶ�ȡ������ */
    HAL_PWR_DisableBkUpAccess();
    __HAL_RCC_PWR_CLK_DISABLE();
    
    /* ���ñ���ʱ�ӣ����ݼĴ��� */
    //__HAL_RCC_BKP_CLK_DISABLE();
  }
} 



