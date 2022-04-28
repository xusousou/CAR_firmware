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
    SystemCoreClock 为48 MHz 
       
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
  
  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

  /* 频道1，2，3，4的PWM 模式设置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道1配置
  TIM_OC1Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道2配置
  TIM_OC2Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1 ;//使能频道3配置
  TIM_OC3Init(TIM1, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道4配置
  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
  /* TIM1 计算器使能*/
  TIM_Cmd(TIM1, ENABLE);
	
  /* TIM1 主输出使能 */
  TIM_CtrlPWMOutputs(TIM1, ENABLE);	
	}



void TIM3_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能GPIO时钟 */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB, ENABLE);
	
  /* 配置GPIO管脚复用*/
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
	/* TIM1 的配置 ---------------------------------------------------
   TIM1 输入时钟(TIM1CLK) 设置为 APB2 时钟 (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock 为48 MHz 
   
   产生 4 路PWM 信号在250Hz:
     - TIM1_Period = (SystemCoreClock / 250) - 1
   定时器脉冲的计算方式如下:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	*/  
    /*计算预定表的值，也就是多少个时钟计数为一个周期*/
  TimerPeriod3 = (SystemCoreClock / fpwm ) - 1;			//250hz
//  /*计算CCR1 跳转值 在占空比为50%时*/
//  pwm_M3A = (uint16_t) (((uint32_t) 0 * (TimerPeriod3 - 1)) / 10);
// /*计算CCR2 跳转值 在占空比为37.5%时*/
//  pwm_M3B  = (uint16_t) (((uint32_t) 2 * (TimerPeriod3 - 1)) / 10);
//  /*计算CCR3 跳转值 在占空比为25%时*/
//  pwm_M4A = (uint16_t) (((uint32_t) 0 * (TimerPeriod3 - 1)) / 10);
// /*计算CCR4跳转值 在占空比为12.5%时*/
//  pwm_M4B = (uint16_t) (((uint32_t) 2 * (TimerPeriod3- 1)) / 10);

  /* TIM1 时钟使能 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  
  /* Time 定时基础设置*/
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time 定时设置为上升沿计算模式*/
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod3;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* 频道1，2，3，4的PWM 模式设置 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道1配置
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道2配置
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道3配置
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);

  TIM_OCInitStructure.TIM_Pulse = 1;//使能频道4配置
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	
  /* TIM1 计算器使能*/
  TIM_Cmd(TIM3, ENABLE);
	
  /* TIM1 主输出使能 */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);	
	}

	
	/*******************************************************************************
* 函 数 名         :M1A_pwm(u16 value)
* 函数功能		   		 : 设置pwm占空比函数
* 输    入         : 占空比	
* 输    出         : 无
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
	




