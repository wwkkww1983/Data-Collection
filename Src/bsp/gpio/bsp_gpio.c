/**
  ******************************************************************************
  * �ļ�����: bsp_gpio.c 
  * ��    ��: 
  * ��    ��: V1.0
  * ��д����: 2017-3-30
  * ��    ��: GPIO-������
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "gpio/bsp_gpio.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ���ذ���IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_key.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
    
    9,10,11��12���ж�ģʽ���ڰ�������������ѯģʽ
  */
void INPUT_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)INPUT���Ŷ�ӦIO�˿�ʱ�� */  
  INPUT1_RCC_CLK_ENABLE();
  INPUT2_RCC_CLK_ENABLE();
  INPUT3_RCC_CLK_ENABLE();
  INPUT4_RCC_CLK_ENABLE();
  INPUT5_RCC_CLK_ENABLE();
  INPUT6_RCC_CLK_ENABLE();
  INPUT7_RCC_CLK_ENABLE();
  INPUT8_RCC_CLK_ENABLE();
  INPUT9_RCC_CLK_ENABLE();
  INPUT10_RCC_CLK_ENABLE();
  INPUT11_RCC_CLK_ENABLE();
  INPUT12_RCC_CLK_ENABLE();

  
  /* ����INPUT GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = INPUT1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT1_GPIO, &GPIO_InitStruct);   
  
  GPIO_InitStruct.Pin = INPUT2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT2_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT3_GPIO_PIN;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT3_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT4_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT5_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT5_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT6_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT6_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT7_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT7_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT8_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT8_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT9_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT9_GPIO, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin = INPUT10_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT10_GPIO, &GPIO_InitStruct);  
  
  
  /* ����INPUT GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = INPUT11_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT11_GPIO, &GPIO_InitStruct);   
  
  /* ����INPUT GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = INPUT12_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(INPUT12_GPIO, &GPIO_InitStruct);   
  
    /* �����ж����ȼ� */
  HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,1);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    /* �����ж����ȼ� */
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  
}
/**
  * ��������: ��ȡ����״̬
  * �����������Ӧ�˿�
  * �� �� ֵ: High������ߵ�ƽ��
  *           Low  ������͵�ƽ
  * ˵    ������
  */
Input_State INPUT_StateRead(int port)
{
  GPIO_TypeDef* INPUT_GPIO;
  uint16_t INPUT_GPIO_PIN;
  switch (port){
    case 1:
      INPUT_GPIO = INPUT1_GPIO;
      INPUT_GPIO_PIN = INPUT2_GPIO_PIN;
      break;
    case 2:
      INPUT_GPIO = INPUT2_GPIO;
      INPUT_GPIO_PIN = INPUT2_GPIO_PIN;
      break;     
    case 3:
      INPUT_GPIO = INPUT3_GPIO;
      INPUT_GPIO_PIN = INPUT3_GPIO_PIN;
      break;  
    case 4:
      INPUT_GPIO = INPUT4_GPIO;
      INPUT_GPIO_PIN = INPUT4_GPIO_PIN;
      break;  
    case 5:
      INPUT_GPIO = INPUT5_GPIO;
      INPUT_GPIO_PIN = INPUT5_GPIO_PIN;
      break;  
    case 6:
      INPUT_GPIO = INPUT6_GPIO;
      INPUT_GPIO_PIN = INPUT6_GPIO_PIN;
      break;       
    case 7:
      INPUT_GPIO = INPUT7_GPIO;
      INPUT_GPIO_PIN = INPUT7_GPIO_PIN;
      break;       
    case 8:
      INPUT_GPIO = INPUT8_GPIO;
      INPUT_GPIO_PIN = INPUT8_GPIO_PIN;
      break;      
    case 9:
      INPUT_GPIO = INPUT9_GPIO;
      INPUT_GPIO_PIN = INPUT9_GPIO_PIN;
      break;        
    case 10:
      INPUT_GPIO = INPUT10_GPIO;
      INPUT_GPIO_PIN = INPUT10_GPIO_PIN;
      break;      
    case 11:
      INPUT_GPIO = INPUT11_GPIO;
      INPUT_GPIO_PIN = INPUT11_GPIO_PIN;
      break;   
    case 12:
      INPUT_GPIO = INPUT12_GPIO;
      INPUT_GPIO_PIN = INPUT12_GPIO_PIN;
      break;  
    default:
      break;
  }
  /* ��ȡ��ʱ����ֵ���ж��Ƿ��Ǳ�����״̬������Ǳ�����״̬���뺯���� */
  if(HAL_GPIO_ReadPin(INPUT_GPIO,INPUT_GPIO_PIN)==INPUT_DOWN_LEVEL)
  {
    /* ��ʱһС��ʱ�䣬�������� */
    HAL_Delay(20);
    /* ��ʱʱ��������жϰ���״̬��������ǰ���״̬˵������ȷʵ������ */
    if(HAL_GPIO_ReadPin(INPUT_GPIO,INPUT_GPIO_PIN)==INPUT_DOWN_LEVEL)
    {
      /* �ȴ������������˳�����ɨ�躯�� */
      //while(HAL_GPIO_ReadPin(INPUT_GPIO,INPUT_GPIO_PIN)==INPUT_DOWN_LEVEL);      
       /* ����ɨ����ϣ�ȷ�����������£����ذ���������״̬ */
      return High;
    }
  }
  /* ����û�����£�����û������״̬ */
  return Low;
}



/**
  * ��������: ���ذ���IO���ų�ʼ��.
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ����ʹ�ú궨�巽������������źţ����������ֲ��ֻҪ���޸�bsp_gpio.h
  *           �ļ���غ궨��Ϳ��Է����޸����š�
  */
void OUTPUT_GPIO_Init(void)
{
   /* ����IOӲ����ʼ���ṹ����� */
  GPIO_InitTypeDef GPIO_InitStruct;
	
	/* ʹ��(����)KEY���Ŷ�ӦIO�˿�ʱ�� */  
  OUTPUT1_RCC_CLK_ENABLE();  
  OUTPUT2_RCC_CLK_ENABLE(); 
  OUTPUT3_RCC_CLK_ENABLE(); 
  OUTPUT4_RCC_CLK_ENABLE(); 
  
  /* ����OUTPUT GPIO:��������ģʽ */
  GPIO_InitStruct.Pin = OUTPUT1_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT1_GPIO, &GPIO_InitStruct);   
  
  GPIO_InitStruct.Pin = OUTPUT2_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT2_GPIO, &GPIO_InitStruct);   
  
  GPIO_InitStruct.Pin = OUTPUT3_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT3_GPIO, &GPIO_InitStruct);   
  
  
  GPIO_InitStruct.Pin = OUTPUT4_GPIO_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(OUTPUT4_GPIO, &GPIO_InitStruct);   
  
  
}


