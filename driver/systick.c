/**
  ******************************************************************************
  * @file    systick.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "systick.h"

/**
  * @brief  ³õÊ¼»¯systick Îª1ms
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval None
  */
void Systick_Configuration(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	if(SysTick_Config(SystemCoreClock/1000)){
		while(1);
	}
}
