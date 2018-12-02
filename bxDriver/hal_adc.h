#ifndef __HAL_ADC_H_
#define __HAL_ADC_H_

#include "app_cfg.h"
#include "timers.h"

#define ADC_FULL_SCAL	4095	// ¾«¶È12
#define ADC_FULL_VREF	3300	//3300mV


#define ADC_ACTIVE				ADC1
#define ADC_ACTIVE_SAMPLETIME   LL_ADC_SAMPLINGTIME_239CYCLES_5

#define ADC_DMA_ACTIVE          DMA1
#define ADC_DMA_CHANNEL			LL_DMA_CHANNEL_1

#define HAL_NTC1_ADC_PORT		AD_TEM1_GPIO_Port
#define HAL_NTC1_ADC_PIN		AD_TEM1_Pin
#define HAL_NTC1_ADC_CHANNEL	LL_ADC_CHANNEL_4

#define HAL_NTC2_ADC_PORT		AD_HUM1_GPIO_Port
#define HAL_NTC2_ADC_PIN		AD_HUM1_Pin
#define HAL_NTC2_ADC_CHANNEL	LL_ADC_CHANNEL_5

#define HAL_NTC3_ADC_PORT		AD_TEM2_GPIO_Port
#define HAL_NTC3_ADC_PIN		AD_TEM2_Pin
#define HAL_NTC3_ADC_CHANNEL	LL_ADC_CHANNEL_6

#define HAL_NTC4_ADC_PORT		AD_HUM2_GPIO_Port
#define HAL_NTC4_ADC_PIN		AD_HUM2_Pin
#define HAL_NTC4_ADC_CHANNEL	LL_ADC_CHANNEL_7

#define HAL_NTC5_ADC_PORT		//GPIOB
#define HAL_NTC5_ADC_PIN		//LL_GPIO_PIN_1
#define HAL_NTC5_ADC_CHANNEL	//LL_ADC_CHANNEL_9

#define HAL_NTC6_ADC_PORT		//GPIOA
#define HAL_NTC6_ADC_PIN		//LL_GPIO_PIN_3
#define HAL_NTC6_ADC_CHANNEL	//LL_ADC_CHANNEL_3


typedef enum {
	ADC_NTC1 = 0,
	ADC_NTC2 = 1,
	ADC_NTC3 = 2,
	ADC_NTC4 = 3,
//	ADC_INTER_VREFINT,
//	ADC_INTER_TEMPSENSOR,
	ADC_CHANNEL_MAX
} AdcChannel;

void haladcInit(void);
void adcStart(void);
uint16_t adcGetRawValue(uint8_t channel);
int32_t adcInternalTemp(void);
uint32_t adcInternalVref(void);


#endif

