




#ifndef __HAL_LED_H_
#define __HAL_LED_H_

#include "app_cfg.h"


#define HAL_REDLED_PORT		GPIOB
#define HAL_REDLED_PIN		GPIO_Pin_4
#define HAL_GREENLED_PORT	GPIOB
#define HAL_GREENLED_PIN	GPIO_Pin_3
#define HAL_LED_GPIO_PeriphClock_EN() do{ RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); }while(0)


void halledInit(void);

#endif
