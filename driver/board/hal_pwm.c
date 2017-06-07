/**
  ******************************************************************************
  * @file    hal_pwm.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#include "hal_pwm.h"



#define PWM_USING_TIM  			TIM14
#define PWM_ONE_PORT 			GPIOA//GPIOB//GPIOA
#define PWM_ONE_PIN				GPIO_Pin_7//GPIO_Pin_4//GPIO_Pin_7
#define PWM_ONE_AF_PINSOURCE	GPIO_PinSource7//GPIO_PinSource4//GPIO_PinSource7
#define PWM_ONE_AF_SEL			GPIO_AF_4//GPIO_AF_1//GPIO_AF_4
#define PWM_ONE_GPIO_PeriphClock_EN()	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#define PWM_USING_TIM_PeriphClock_EN()	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);		


/**
  * @brief	PWM初始化
  * @param	None
  * @note	
  * @note	 
  * @note	
  * @retval  None
  */
void halpwmInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
//	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;

	TIM_DeInit(PWM_USING_TIM);	
	PWM_ONE_GPIO_PeriphClock_EN();
	PWM_USING_TIM_PeriphClock_EN();
		
	//PA7重映射为TIM14_CH1
	GPIO_InitStruct.GPIO_Pin 	= PWM_ONE_PIN;
	GPIO_InitStruct.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  	= GPIO_PuPd_UP;
	
	GPIO_Init(PWM_ONE_PORT, &GPIO_InitStruct);
	//配置对应复用脚
	GPIO_PinAFConfig(PWM_ONE_PORT,PWM_ONE_AF_PINSOURCE, PWM_ONE_AF_SEL);

	//时钟48分频，TIM时钟为1M ,TIM_Period设置10,周期为1000khz()
	/*
	Prescaler = (TIMX CLK/TIMX Counter Clock) -1
	Period = (TIMX Counter Clock/TIMX Output Clock - 1)
	*/
	TIM_TimeBaseInitStruct.TIM_Prescaler 		 = 48 - 1;
	TIM_TimeBaseInitStruct.TIM_CounterMode 	 	 = TIM_CounterMode_Up;//采用向上计数法
	TIM_TimeBaseInitStruct.TIM_Period 			 = 10 - 1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision 	 = TIM_CKD_DIV1;//时钟不分频
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
	TIM_TimeBaseInit(PWM_USING_TIM, &TIM_TimeBaseInitStruct);
	/*
	配置输出比较，产生占空比为20%的PWM波
	*/
	TIM_OCInitStruct.TIM_OCMode 		= TIM_OCMode_PWM1;		//PWM1为正常波形，PWM2为反极性
	TIM_OCInitStruct.TIM_OutputState	= TIM_OutputState_Enable;//OCx输出使能
	TIM_OCInitStruct.TIM_OutputNState	= TIM_OutputNState_Enable;//OCNx输出使能
	TIM_OCInitStruct.TIM_OCIdleState	= TIM_OCIdleState_Reset; //空闲状态设为0
	TIM_OCInitStruct.TIM_OCNIdleState	= TIM_OCNIdleState_Reset;  //空闲状态设为0
	TIM_OCInitStruct.TIM_OCPolarity		= TIM_OCPolarity_High;     //OCxP极性为低
	TIM_OCInitStruct.TIM_OCNPolarity	= TIM_OCNPolarity_Low;    //OCxNP极性为低
	
	TIM_OCInitStruct.TIM_Pulse	= 5;//比较计数器为3000
	TIM_OC1Init(PWM_USING_TIM, &TIM_OCInitStruct);
	//TIM_OC2Init(PWM_USING_TIM, &TIM_OCInitStruct);

	TIM_OC1PreloadConfig(PWM_USING_TIM, TIM_OCPreload_Enable);
	//TIM_OC2PreloadConfig(PWM_USING_TIM, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(PWM_USING_TIM, ENABLE);//使能ARR寄存器
	TIM_Cmd(PWM_USING_TIM, ENABLE);
	//TIM_CtrlPWMOutputs(PWM_USING_TIM, ENABLE);
	//TIM_GenerateEvent(PWM_USING_TIM,TIM_EventSource_Update);
}

