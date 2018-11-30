#ifndef __HAL_ADC_H_
#define __HAL_ADC_H_

#include "app_cfg.h"


#define ADC_FULL_SCAL	4095	// ¾«¶È12
#define ADC_FULL_VREF	3300	//3300mV


#define ADC_ACTIVE				ADC1
#define ADC_DMA_CHANNEL			DMA1_Channel1	
#define HAL_NTC1_ADC_PORT		GPIOA
#define HAL_NTC1_ADC_PIN		GPIO_Pin_4
#define HAL_NTC1_ADC_CHANNEL	ADC_Channel_4
#define HAL_NTC2_ADC_PORT		GPIOA
#define HAL_NTC2_ADC_PIN		GPIO_Pin_5
#define HAL_NTC2_ADC_CHANNEL	ADC_Channel_5
#define HAL_NTC3_ADC_PORT		GPIOA
#define HAL_NTC3_ADC_PIN		GPIO_Pin_6
#define HAL_NTC3_ADC_CHANNEL	ADC_Channel_6
#define HAL_NTC4_ADC_PORT		GPIOA
#define HAL_NTC4_ADC_PIN		GPIO_Pin_7
#define HAL_NTC4_ADC_CHANNEL	ADC_Channel_7
#define HAL_NTC5_ADC_PORT		GPIOB
#define HAL_NTC5_ADC_PIN		GPIO_Pin_1
#define HAL_NTC5_ADC_CHANNEL	ADC_Channel_9
#define HAL_NTC6_ADC_PORT		GPIOA
#define HAL_NTC6_ADC_PIN		GPIO_Pin_3
#define HAL_NTC6_ADC_CHANNEL	ADC_Channel_3


#define HAL_ADC_PeriphClock_EN() do{RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);}while(0)
#define HAL_ADC_GPIO_PeriphClock_EN() do{RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);}while(0)
#define ADC_DMA_PeriphClock_EN() 	do{RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);}while(0)

typedef enum {
	ADC_NTC1 = 0,
	ADC_NTC2 = 1,
	ADC_NTC3 = 2,
	ADC_NTC4 = 3,
	ADC_NTC5 = 4,
//	ADC_NTC6 = 5,
	ADC_CHANNEL_MAX
} AdcChannel;

void haladcInit(void);
uint16_t adcGetRawValue(uint8_t channel);
int32_t adcInternalTemp(void);
uint32_t adcInternalVref(void);


#endif

