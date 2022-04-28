#include "timer.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
uint16_t TimerPeriod1 = 0,TimerPeriod3 = 0;
uint16_t pwm_M1A = 0, pwm_M1B = 0, pwm_M2A = 0, pwm_M2B = 0;
uint16_t pwm_M3A = 0, pwm_M3B = 0, pwm_M4A = 0, pwm_M4B = 0;

void TIM1_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* Configuring GPIO pin multiplexing */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_2);

}

void TIM1_PWM_Config(uint16_t fpwm)
{
    /* TIM1 configuration ---------------------------------------------------
    TIM1 configuration    
     => TIM1CLK = PCLK2 = SystemCoreClock
    TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
    SystemCoreClock Ϊ48 MHz 
       
    Our goal is to generate 4 PWM signals at 250Hz:
     - TIM1_Period = (SystemCoreClock / 250) - 1
    The duty cycle set for channels 1, 2, 3, and 4 is 50%. 
    The timer pulse is calculated as follows:
         - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	*/  
    /*Calculate the value of the predetermined table, that is, how many clock counts are one cycle*/
  TimerPeriod1 = (SystemCoreClock / fpwm ) - 1;			//250hz

  /* Enable TIM1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
  
  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* Ƶ��1��2��3��4��PWM ģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��1����
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��2����
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1 ;//ʹ��Ƶ��3����
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��4����
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
  /* TIM1 ������ʹ��*/
  TIM_Cmd(TIM1, ENABLE);
	
  /* TIM1 �����ʹ�� */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	}



void TIM3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ��GPIOʱ�� */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
  /* ����GPIO�ܽŸ���*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);

}

void TIM3_PWM_Config(uint16_t fpwm)
{
	/* TIM1 ������ ---------------------------------------------------
   TIM1 ����ʱ��(TIM1CLK) ����Ϊ APB2 ʱ�� (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock Ϊ48 MHz 
   
   ���� 4 ·PWM �ź���250Hz:
     - TIM1_Period = (SystemCoreClock / 250) - 1
   ��ʱ������ļ��㷽ʽ����:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	*/  
    /*����Ԥ�����ֵ��Ҳ���Ƕ��ٸ�ʱ�Ӽ���Ϊһ������*/
  TimerPeriod3 = (SystemCoreClock / fpwm ) - 1;			//250hz
//  /*����CCR1 ��תֵ ��ռ�ձ�Ϊ50%ʱ*/
//  pwm_M3A = (uint16_t) (((uint32_t) 0 * (TimerPeriod3 - 1)) / 10);
// /*����CCR2 ��תֵ ��ռ�ձ�Ϊ37.5%ʱ*/
//  pwm_M3B  = (uint16_t) (((uint32_t) 2 * (TimerPeriod3 - 1)) / 10);
//  /*����CCR3 ��תֵ ��ռ�ձ�Ϊ25%ʱ*/
//  pwm_M4A = (uint16_t) (((uint32_t) 0 * (TimerPeriod3 - 1)) / 10);
// /*����CCR4��תֵ ��ռ�ձ�Ϊ12.5%ʱ*/
//  pwm_M4B = (uint16_t) (((uint32_t) 2 * (TimerPeriod3- 1)) / 10);

  /* TIM1 ʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  
  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod3;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* Ƶ��1��2��3��4��PWM ģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��1����
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��2����
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��3����
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//ʹ��Ƶ��4����
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
  /* TIM1 ������ʹ��*/
  TIM_Cmd(TIM3, ENABLE);
	
  /* TIM1 �����ʹ�� */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);	
	}

	
	/*******************************************************************************
* �� �� ��         :M1A_pwm(u16 value)
* ��������		   		 : ����pwmռ�ձȺ���
* ��    ��         : ռ�ձ�	
* ��    ��         : ��
*******************************************************************************/
void M1A_pwm(uint16_t value)
{
	TIM1->CCR1=TimerPeriod1-(value*TimerPeriod1/100);
}

void M1B_pwm(uint16_t value)
{
	TIM1->CCR2=TimerPeriod1-(value*TimerPeriod1/100);
}

void M2A_pwm(uint16_t value)
{
	TIM1->CCR3=TimerPeriod1-(value*TimerPeriod1/100);
}

void M2B_pwm(uint16_t value)
{
	TIM1->CCR4=TimerPeriod1-(value*TimerPeriod1/100);	
}



void M3A_pwm(uint16_t value)
{
	TIM3->CCR1=TimerPeriod3-(value*TimerPeriod3/100);
}

void M3B_pwm(uint16_t value)
{
	TIM3->CCR2=TimerPeriod3-(value*TimerPeriod3/100);
}

void M4A_pwm(uint16_t value)
{
		TIM3->CCR3=TimerPeriod3-(value*TimerPeriod3/100);
}

void M4B_pwm(uint16_t value)
{
		TIM3->CCR4=TimerPeriod3-(value*TimerPeriod3/100);
}
	




