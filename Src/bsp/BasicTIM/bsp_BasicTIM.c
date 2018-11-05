/**
  ******************************************************************************
  * �ļ�����: bsp_BasicTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-03-30
  * ��    ��: ������ʱ��TIM6 & TIM7�ײ���������
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
#include "BasicTIM/bsp_BasicTIM.h" 
    
#include "StepMotor/bsp_STEPMOTOR.h" 
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx;

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void BASIC_TIMx_Init(void)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  htimx.Instance = BASIC_TIMx;
  htimx.Init.Prescaler = BASIC_TIMx_PRESCALER;
  htimx.Init.CounterMode = TIM_COUNTERMODE_UP;
  htimx.Init.Period = BASIC_TIMx_PERIOD;
  HAL_TIM_Base_Init(&htimx);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx, &sMasterConfig);
}

/**
  * ��������: ������ʱ��Ӳ����ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==BASIC_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    BASIC_TIM_RCC_CLK_ENABLE();

    /* �����ж����� */
    HAL_NVIC_SetPriority(BASIC_TIM_IRQ, 1,1);
    HAL_NVIC_EnableIRQ(BASIC_TIM_IRQ);
  }
  else if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ��ʹ�� */
    STEPMOTOR_TIM_RCC_CLK_ENABLE();
  }
  
  
}

/**
  * ��������: ������ʱ��Ӳ������ʼ������
  * �������: htim_base��������ʱ���������ָ��
  * �� �� ֵ: ��
  * ˵    ��: �ú�����HAL���ڲ�����
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{

  if(htim_base->Instance==BASIC_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    BASIC_TIM_RCC_CLK_DISABLE();

    /* �ر������ж� */
    HAL_NVIC_DisableIRQ(BASIC_TIM_IRQ);
  }
  else if(htim_base->Instance==STEPMOTOR_TIMx)
  {
    /* ������ʱ������ʱ�ӽ��� */
    STEPMOTOR_TIM_RCC_CLK_DISABLE();
    
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL1_PORT,STEPMOTOR_TIM_PUL1_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR1_PORT,STEPMOTOR_DIR1_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA1_PORT,STEPMOTOR_ENA1_PIN);
    
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL2_PORT,STEPMOTOR_TIM_PUL2_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR2_PORT,STEPMOTOR_DIR2_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA2_PORT,STEPMOTOR_ENA2_PIN);
    
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL3_PORT,STEPMOTOR_TIM_PUL3_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR3_PORT,STEPMOTOR_DIR3_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA3_PORT,STEPMOTOR_ENA3_PIN);
    
    HAL_GPIO_DeInit(STEPMOTOR_TIM_PUL4_PORT,STEPMOTOR_TIM_PUL4_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_DIR4_PORT,STEPMOTOR_DIR4_PIN);
    HAL_GPIO_DeInit(STEPMOTOR_ENA4_PORT,STEPMOTOR_ENA4_PIN);
    
    HAL_NVIC_DisableIRQ(STEPMOTOR_TIMx_IRQn);
  }
  
} 

/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
