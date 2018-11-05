/**
  ******************************************************************************
  * �ļ�����: bsp_STEPMOTOR.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2017-05-31
  * ��    ��: �����������������ʵ��
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F1Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "StepMotor/bsp_STEPMOTOR.h" 
#include "variable.h"
/* ˽�����Ͷ��� --------------------------------------------------------------*/
/* ˽�к궨�� ----------------------------------------------------------------*/
/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_STEPMOTOR;
//__IO uint16_t Toggle_Pulse = 450;         // �Ƚ�������ڣ�ֵԽС���Ƶ��Խ�죬Ĭ�����

/* ��չ���� ------------------------------------------------------------------*/
/* ˽�к���ԭ�� --------------------------------------------------------------*/
/* ������ --------------------------------------------------------------------*/
/**
  * ��������: ���������GPIO��ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
static void STEPMOTOR_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct; 
  
  /* ���Ŷ˿�ʱ��ʹ�� */
  STEPMOTOR_TIM_PUL1_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR1_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA1_GPIO_CLK_ENABLE();
  
  STEPMOTOR_TIM_PUL2_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR2_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA2_GPIO_CLK_ENABLE();
  
  STEPMOTOR_TIM_PUL3_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR3_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA3_GPIO_CLK_ENABLE();
  
  STEPMOTOR_TIM_PUL4_GPIO_CLK_ENABLE();
  STEPMOTOR_DIR4_GPIO_CLK_ENABLE();
  STEPMOTOR_ENA4_GPIO_CLK_ENABLE();
  
  /* ��1��*/
  /* �����������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;        // GPIO��������TIM���ù���
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL1_PORT, &GPIO_InitStruct);
  
  /* �����������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_DIR1_PORT, &GPIO_InitStruct);
  
  /* �������ѻ�ʹ�ܿ�������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_ENA1_PORT, &GPIO_InitStruct);
  
  /* ��2��*/
  /* �����������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;        // GPIO��������TIM���ù���
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL2_PORT, &GPIO_InitStruct);
  
  /* ��������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_DIR2_PORT, &GPIO_InitStruct);
  
  /* ���������ʹ�ܿ�������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_ENA2_PORT, &GPIO_InitStruct);
  
  /* ��3��*/
  /* �����������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;        // GPIO��������TIM���ù���
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL3_PORT, &GPIO_InitStruct);
  
  /* ��������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_DIR3_PORT, &GPIO_InitStruct);
  
  /* ���������ʹ�ܿ�������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA3_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_ENA3_PORT, &GPIO_InitStruct);
  
  /* ��4��*/
  /* �����������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_TIM_PUL4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF3_TIM8;        // GPIO��������TIM���ù���
  HAL_GPIO_Init(STEPMOTOR_TIM_PUL4_PORT, &GPIO_InitStruct);
  
  /* ��������������������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_DIR4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_DIR4_PORT, &GPIO_InitStruct);
  
  /* ���������ʹ�ܿ�������IO��ʼ�� */
  GPIO_InitStruct.Pin = STEPMOTOR_ENA4_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF0_TRACE;       // GPIO��������ϵͳĬ�Ϲ���
  HAL_GPIO_Init(STEPMOTOR_ENA4_PORT, &GPIO_InitStruct);
  
  STEPMOTOR_DIR1_FORWARD();
  STEPMOTOR_OUTPUT1_ENABLE();
  
  STEPMOTOR_DIR2_FORWARD();
  STEPMOTOR_OUTPUT2_ENABLE();
  
  STEPMOTOR_DIR3_FORWARD();
  STEPMOTOR_OUTPUT3_ENABLE();
  
  STEPMOTOR_DIR4_FORWARD();
  STEPMOTOR_OUTPUT4_ENABLE();
}

/**
  * ��������: ��������ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void STEPMOTOR_TIMx_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;             // ��ʱ��ʱ��
  TIM_MasterConfigTypeDef sMasterConfig;                 // ��ʱ����ģʽ����
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;   // ɲ��������ʱ������
  TIM_OC_InitTypeDef sConfigOC;                          // ��ʱ��ͨ���Ƚ����
  
  /* ��ʱ�������������� */
  htimx_STEPMOTOR.Instance = STEPMOTOR_TIMx;                                 // ��ʱ�����
  htimx_STEPMOTOR.Init.Prescaler = STEPMOTOR_TIM_PRESCALER;                  // ��ʱ��Ԥ��Ƶ��
  htimx_STEPMOTOR.Init.CounterMode = TIM_COUNTERMODE_UP;                     // �����������ϼ���
  htimx_STEPMOTOR.Init.Period = STEPMOTOR_TIM_PERIOD;                        // ��ʱ������
  htimx_STEPMOTOR.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;                 // ʱ�ӷ�Ƶ
  htimx_STEPMOTOR.Init.RepetitionCounter = STEPMOTOR_TIM_REPETITIONCOUNTER;  // �ظ�������
  HAL_TIM_Base_Init(&htimx_STEPMOTOR);

  /* ��ʱ��ʱ��Դ���� */
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;       // ʹ���ڲ�ʱ��Դ
  HAL_TIM_ConfigClockSource(&htimx_STEPMOTOR, &sClockSourceConfig);

  /* ��ʼ����ʱ���Ƚ�������� */
  HAL_TIM_OC_Init(&htimx_STEPMOTOR);
  
  /* ��ʱ�������ģʽ */
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htimx_STEPMOTOR, &sMasterConfig);
  
  /* ɲ��������ʱ������ */
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_STEPMOTOR, &sBreakDeadTimeConfig);

  Step_Motor_Init(5);
  
  /* ��ʱ���Ƚ�������� */
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;                // �Ƚ����ģʽ����ת���
  sConfigOC.Pulse = Stepper_Motor[0].Toggle_Pulse;                   // ������
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;          // �������
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         // ����ͨ���������
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;           // ����ģʽ
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;       // ���е�ƽ
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;     // ����ͨ�����е�ƽ
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_NO1_TIM_CHANNEL_x);

  sConfigOC.Pulse = Stepper_Motor[1].Toggle_Pulse;                   // ������
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_NO2_TIM_CHANNEL_x);
  
  sConfigOC.Pulse = Stepper_Motor[2].Toggle_Pulse;                   // ������
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_NO3_TIM_CHANNEL_x);
  
  sConfigOC.Pulse = Stepper_Motor[3].Toggle_Pulse;                  // ������
  HAL_TIM_OC_ConfigChannel(&htimx_STEPMOTOR, &sConfigOC, STEPMOTOR_NO4_TIM_CHANNEL_x);
  
  /* STEPMOTOR���GPIO��ʼ������ */
  STEPMOTOR_GPIO_Init();
  
  /* ���ö�ʱ���ж����ȼ���ʹ�� */
  HAL_NVIC_SetPriority(STEPMOTOR_TIMx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(STEPMOTOR_TIMx_IRQn);

}
/**
  * ��������: ���������ʼ��
  * �������: ��Ӧ id
  * �� �� ֵ: ��
  * ˵    ��: id == 5 ȫ����ʼ��
typedef struct {
  __IO uint8_t  run_state ;  // �����ת״̬
  __IO uint8_t  dir ;        // �����ת����
  __IO int32_t  step_delay;  // �¸��������ڣ�ʱ������������ʱΪ���ٶ�
  __IO uint32_t decel_start; // ��������λ��
  __IO int32_t  decel_val;   // ���ٽ׶β���
  __IO int32_t  min_delay;   // ��С��������(����ٶȣ������ٶ��ٶ�)
  __IO int32_t  accel_count; // �Ӽ��ٽ׶μ���ֵ
  __IO int32_t  speed; // �����ٶȣ�rpm��
  __IO int32_t  step; //����
  __IO int32_t  min_speed; //��С�ٶ�  �����ݵ����ͬ������ͬ��
  __IO int32_t  max_speed; //����ٶ�
  __IO uint16_t Toggle_Pulse;//����Ƶ��
}speedRampData;
  */
void Step_Motor_Init(int id){
  int i;
  if(id == 5){
    for(i=0;i<4;i++){
      Stepper_Motor[i].run_state = STOP;
      Stepper_Motor[i].dir = CW;
      Stepper_Motor[i].step_delay = 0;
      Stepper_Motor[i].decel_start = 0;
      Stepper_Motor[i].decel_val = 0;
      Stepper_Motor[i].min_delay = 0;
      Stepper_Motor[i].accel_count = 0;
      Stepper_Motor[i].speed = 0;
      Stepper_Motor[i].min_speed = 0;
      Stepper_Motor[i].max_speed = 0;
      Stepper_Motor[i].Toggle_Pulse = 1000;
      Stepper_Motor[i].step = 0;
    }
  }
  else{
      Stepper_Motor[id-1].run_state = STOP;
      Stepper_Motor[id-1].dir = CW;
      Stepper_Motor[id-1].step_delay = 0;
      Stepper_Motor[id-1].decel_start = 0;
      Stepper_Motor[id-1].decel_val = 0;
      Stepper_Motor[id-1].min_delay = 0;
      Stepper_Motor[id-1].accel_count = 0;
      Stepper_Motor[id-1].speed = 0;
      Stepper_Motor[id-1].min_speed = 0;
      Stepper_Motor[id-1].max_speed = 0;
      Stepper_Motor[id-1].Toggle_Pulse = 800;
      Stepper_Motor[id-1].step = 0;
  
  }

}



/******************* (C) COPYRIGHT 2015-2020 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
