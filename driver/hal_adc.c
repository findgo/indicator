







#include "hal_adc.h"


typedef struct adc_config_s {
	uint32_t adcChannel;         // ADC1_INxx channel number
	uint8_t dmaIndex;           // index into DMA buffer in case of sparse channels
	uint8_t sampleTime;
} adc_config_t;

static adc_config_t adcConfig[ADC_CHANNEL_MAX];
static volatile uint16_t adcValues[16][ADC_CHANNEL_MAX];

void haladcInit(void)
{
	uint8_t i;
	uint8_t configuredAdcChannels = 0;
	ADC_InitTypeDef		ADC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;
	
	/* ADC1 DeInit */  
	ADC_DeInit(ADC_ACTIVE);

	RCC_ADCCLKConfig( RCC_ADCCLK_HSI14);
	
	/* GPIOC Periph clock enable */
	HAL_ADC_GPIO_PeriphClock_EN();

	/* ADC1 Periph clock enable */
	HAL_ADC_PeriphClock_EN();

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC1_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HAL_NTC1_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC2_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HAL_NTC2_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC3_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HAL_NTC3_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC4_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HAL_NTC4_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC5_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(HAL_NTC5_ADC_PORT, &GPIO_InitStructure);
	
	/* Configure ADC Channel11 and channel10 as analog input */
//	GPIO_InitStructure.GPIO_Pin = HAL_NTC6_ADC_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//	GPIO_Init(HAL_NTC6_ADC_PORT, &GPIO_InitStructure);

	adcConfig[ADC_NTC1].adcChannel = HAL_NTC1_ADC_CHANNEL;
	adcConfig[ADC_NTC1].dmaIndex = configuredAdcChannels++;
	adcConfig[ADC_NTC1].sampleTime = ADC_SampleTime_239_5Cycles;

	adcConfig[ADC_NTC2].adcChannel = HAL_NTC2_ADC_CHANNEL;
	adcConfig[ADC_NTC2].dmaIndex = configuredAdcChannels++;
	adcConfig[ADC_NTC2].sampleTime = ADC_SampleTime_239_5Cycles;

	adcConfig[ADC_NTC3].adcChannel = HAL_NTC3_ADC_CHANNEL;
	adcConfig[ADC_NTC3].dmaIndex = configuredAdcChannels++;
	adcConfig[ADC_NTC3].sampleTime = ADC_SampleTime_239_5Cycles;

	adcConfig[ADC_NTC4].adcChannel = HAL_NTC4_ADC_CHANNEL;
	adcConfig[ADC_NTC4].dmaIndex = configuredAdcChannels++;
	adcConfig[ADC_NTC4].sampleTime = ADC_SampleTime_239_5Cycles;

	adcConfig[ADC_NTC5].adcChannel = HAL_NTC5_ADC_CHANNEL;
	adcConfig[ADC_NTC5].dmaIndex = configuredAdcChannels++;
	adcConfig[ADC_NTC5].sampleTime = ADC_SampleTime_239_5Cycles;

//	adcConfig[ADC_NTC6].adcChannel = HAL_NTC6_ADC_CHANNEL;
//	adcConfig[ADC_NTC6].dmaIndex = configuredAdcChannels++;
//	adcConfig[ADC_NTC6].sampleTime = ADC_SampleTime_239_5Cycles;
//		
	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;// 从1 - 18方向扫描
	ADC_Init(ADC_ACTIVE, &ADC_InitStructure); 

	for(i = 0; i < ADC_CHANNEL_MAX; i++){
		ADC_ChannelConfig(ADC_ACTIVE, adcConfig[i].adcChannel, adcConfig[i].sampleTime); 
	}	
//	ADC_TempSensorCmd(ENABLE);
//	ADC_VrefintCmd(ENABLE);

	/* DMA1 clock enable */
	ADC_DMA_PeriphClock_EN();

	/* DMA1 Channel1 Config */
	DMA_DeInit(ADC_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC_ACTIVE->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adcValues[0][0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = sizeof(adcValues)/sizeof(uint16_t);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);

	/* DMA1 Channel1 enable */
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC_ACTIVE);

	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(ADC_ACTIVE, ADC_DMAMode_Circular);

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC_ACTIVE, ENABLE);  

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC_ACTIVE, ENABLE);	 

	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC_ACTIVE, ADC_FLAG_ADRDY)); 

	/* ADC1 regular Software Start Conv */ 
	ADC_StartOfConversion(ADC_ACTIVE);
}


/**
  * @brief	读ADC的值，未处理
  * @param	channel 通道
  * @note		
  * @retval  None
  */
uint16_t adcGetRawValue(uint8_t channel)
{
	uint16_t aver_value = 0;
	uint8_t i;

	for(i = 0; i < sizeof(adcValues)/sizeof(uint16_t)/ADC_CHANNEL_MAX; i++){
		aver_value += adcValues[i][adcConfig[channel].dmaIndex];
	}
	
    return aver_value;
}

/*
#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7c2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7b8))

// 未测试
int32_t adcInternalTemp(void)
{
	uint16_t sensor;
	int32_t temperature;
	
	sensor = adcGetRawValue(ADC_SENSOR);
	temperature = ((int32_t) sensor * 300 / 330) - ((int32_t)*TEMP30_CAL_ADDR);
	temperature = temperature *((int32_t)(110 - 30));
	temperature = temperature /((int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR)) + 30;

	return temperature;
}

//未测试
uint32_t adcInternalVref(void)
{
	uint16_t vtemp;

	vtemp = adcGetRawValue(ADC_VREF);

	return (uint32_t)vtemp;
}
*/



