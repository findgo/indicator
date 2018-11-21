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
  * @brief  config systick 1ms
  * @param  None
  * @note   
  * @note    
  * @note   
  * @retval None
  */
void Systick_Configuration(void)
{
    LL_SYSTICK_SetClkSource( LL_SYSTICK_CLKSOURCE_HCLK);
	if(SysTick_Config(SystemCoreClock / 1000)){
		while(1);
	}
}

