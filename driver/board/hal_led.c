

#include "hal_led.h"




void halledInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	HAL_LED_GPIO_PeriphClock_EN();

//for red led
	GPIO_InitStruct.GPIO_Pin = HAL_REDLED_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(HAL_REDLED_PORT, &GPIO_InitStruct);

// for green led
	GPIO_InitStruct.GPIO_Pin = HAL_GREENLED_PIN;
	
	GPIO_Init(HAL_GREENLED_PORT, &GPIO_InitStruct);
	
}
