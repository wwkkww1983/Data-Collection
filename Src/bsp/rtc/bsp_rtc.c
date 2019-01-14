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
  * ��������: ��ASCII��ת��Ϊ����,���ַ���ת��ʮ������
  * �������: value �û��ڳ����ն����������ֵ
  * �� �� ֵ: �����ַ���ASCII���Ӧ����ֵ
  * ˵    ����������ר����RTC��ȡʱ��
  */
int StringToArray(char *str, unsigned char *out)
{

    char *p = str;

    char high = 0, low = 0;

    int tmplen = strlen(p);
    int cnt = 0;

    tmplen = strlen(p);

    if(tmplen % 2 != 0) return -1;

    while(cnt < tmplen / 2)

    {

        high = ((*p > '9') && ((*p <= 'F') || (*p <= 'f'))) ? *p - 48 - 7 : *p - 48;

        low = (*(++ p) > '9' && ((*p <= 'F') || (*p <= 'f'))) ? *(p) - 48 - 7 : *(p) - 48;

        out[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));

        p++;

        cnt ++;

    }


    return tmplen / 2;

}
/**
  * ��������: ���õ�ǰʱ�������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void RTC_CalendarConfig(char *time)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef DateToUpdate;
  int outlen = 0;
  unsigned char out[10] = {0};
  
  outlen = StringToArray(time, out);
  Show_Data(out,7); 
  
  if(outlen != 7)
  {
    printf("RTC_CalendarConfig:����ʱ�����,�ֶ�����Ϊ%d�������ط�ʱ��\n",outlen);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  
  /* �������� */
  /* �������ڣ�2018��11��28�� ���ڶ�*/
  if(out[0] < 0x18 || out[0] > 0x40)
  {
    printf("RTC_CalendarConfig:������ݴ���%02x�������ط�\n",out[0]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  
  if(out[1] > 0x12)
  {
    printf("RTC_CalendarConfig:�����·ݴ���%02x�������ط�\n",out[1]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  if(out[2] > 0x31)
  {
    printf("RTC_CalendarConfig:�������ڴ���%02x�������ط�\n",out[2]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  if(out[3] > 0x07)
  {
    printf("RTC_CalendarConfig:�������ڴ���%02x�������ط�\n",out[3]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  if(out[4] > 0x24)
  {
    printf("RTC_CalendarConfig:����Сʱ����%02x�������ط�\n",out[4]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  if(out[5] > 0x60)
  {
    printf("RTC_CalendarConfig:���÷��Ӵ���%02x�������ط�\n",out[5]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
  if(out[6] > 0x60)
  {
    printf("RTC_CalendarConfig:������������%02x�������ط�\n",out[6]);
    HAL_UART_Transmit(&husart_debug,"+",1,1000);
    return;
  }
    /* ����ʱ�� */
  /* ʱ��ʱ�䣺10:15:46 */
  sTime.Hours = out[4];
  sTime.Minutes = out[5];
  sTime.Seconds = out[6];
  
  
  DateToUpdate.Year = out[0];  
  DateToUpdate.Month = out[1];
  DateToUpdate.Date = out[2];
  DateToUpdate.WeekDay = out[3];
  
  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
  HAL_RTC_SetDate(&hrtc,&DateToUpdate,RTC_FORMAT_BCD);
  
  

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

  //char *strCom = "18121704121700";
  /* ��������Ƿ񱣴���RTC���ݼĴ���1������Ѿ�����Ͳ���Ҫ�������ں�ʱ������ */
  /* ��ȡ���ݼĴ���1���� */
  if (HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR1) != 0x32F1)
  {
    /* ����RTC��������ʱ������� */
   // RTC_CalendarConfig(strCom);
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



