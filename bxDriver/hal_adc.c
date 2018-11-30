



#include "hal_adc.h"


typedef struct adc_config_s {
	uint32_t adcChannel;         // ADC1_INxx channel number
	uint8_t dmaIndex;           // index into DMA buffer in case of sparse channels
} adc_config_t;

static uint32_t Hal_ADC_GetCalibrationFactor(ADC_TypeDef* ADCx);

static adc_config_t adcConfig[ADC_CHANNEL_MAX];
static volatile uint16_t adcValues[16][ADC_CHANNEL_MAX];

void haladcInit(void)
{
	uint8_t i;
	uint8_t configuredAdcChannels = 0;
    LL_ADC_InitTypeDef ADC_InitStruct = {0};
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
    
    LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	
	/* ADC1 DeInit */  
    LL_ADC_DeInit(ADC_ACTIVE);

    //LL_RCC_HSI14_EnableADCControl(); // init in stm32cubeMX

    /* Peripheral clock enable */
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_ADC1);
    
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**ADC GPIO Configuration  
    PA4   ------> ADC_IN4
    PA5   ------> ADC_IN5
    PA6   ------> ADC_IN6
    PA7   ------> ADC_IN7 
    */
    GPIO_InitStruct.Pin = HAL_NTC1_ADC_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(HAL_NTC1_ADC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = HAL_NTC2_ADC_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(HAL_NTC2_ADC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = HAL_NTC3_ADC_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(HAL_NTC3_ADC_PORT, &GPIO_InitStruct);
    
    GPIO_InitStruct.Pin = HAL_NTC4_ADC_PIN;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(HAL_NTC4_ADC_PORT, &GPIO_InitStruct);

	adcConfig[ADC_NTC1].adcChannel = HAL_NTC1_ADC_CHANNEL;
	adcConfig[ADC_NTC1].dmaIndex = configuredAdcChannels++;

	adcConfig[ADC_NTC2].adcChannel = HAL_NTC2_ADC_CHANNEL;
	adcConfig[ADC_NTC2].dmaIndex = configuredAdcChannels++;

	adcConfig[ADC_NTC3].adcChannel = HAL_NTC3_ADC_CHANNEL;
	adcConfig[ADC_NTC3].dmaIndex = configuredAdcChannels++;

	adcConfig[ADC_NTC4].adcChannel = HAL_NTC4_ADC_CHANNEL;
	adcConfig[ADC_NTC4].dmaIndex = configuredAdcChannels++;

    // 使能内置传感器
#if defined(ADC_INTER_VREFINT)
	adcConfig[ADC_INTER_VREFINT].adcChannel = LL_ADC_CHANNEL_VREFINT;
	adcConfig[ADC_INTER_VREFINT].dmaIndex = configuredAdcChannels++;
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC_ACTIVE), LL_ADC_PATH_INTERNAL_VREFINT);
#endif
#if defined(ADC_INTER_TEMPSENSOR)
	adcConfig[ADC_INTER_TEMPSENSOR].adcChannel = LL_ADC_CHANNEL_TEMPSENSOR;
	adcConfig[ADC_INTER_TEMPSENSOR].dmaIndex = configuredAdcChannels++;
    LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC_ACTIVE),, LL_ADC_PATH_INTERNAL_TEMPSENSOR);
#endif
	
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
    ADC_InitStruct.Clock = LL_ADC_CLOCK_ASYNC; // 时钟电源 hsi14MHZ
    ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
    ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStruct.LowPowerMode = LL_ADC_LP_MODE_NONE;
    LL_ADC_Init(ADC1, &ADC_InitStruct);
    
	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
    ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
    ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
    ADC_REG_InitStruct.Overrun = LL_ADC_REG_OVR_DATA_PRESERVED;
    LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
    LL_ADC_REG_SetSequencerScanDirection(ADC1, LL_ADC_REG_SEQ_SCAN_DIR_FORWARD);

	for(i = 0; i < ADC_CHANNEL_MAX; i++){
        LL_ADC_REG_SetSequencerChannels(ADC_ACTIVE, adcConfig[i].adcChannel);
    }	
    
    LL_ADC_SetSamplingTimeCommonChannels(ADC_ACTIVE, ADC_ACTIVE_SAMPLETIME);


    /* ADC DMA Init */
	/* DMA1 clock enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
    
    /* ADC Init */
    LL_DMA_SetDataTransferDirection(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); // 方向
    
    LL_DMA_SetChannelPriorityLevel(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_PRIORITY_HIGH); // 优先级
        
    LL_DMA_SetPeriphIncMode(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetPeriphSize(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetPeriphAddress(ADC_DMA_ACTIVE, ADC_DMA_CHANNEL, (uint32_t)&ADC_ACTIVE->DR);
    
    LL_DMA_SetMemoryIncMode(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetMemorySize(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetMemoryAddress(ADC_DMA_ACTIVE, ADC_DMA_CHANNEL, (uint32_t)&adcValues[0][0]);
    
    LL_DMA_SetDataLength(ADC_DMA_ACTIVE, ADC_DMA_CHANNEL, sizeof(adcValues)/sizeof(uint16_t));

	/* DMA1 Channel1 enable */
    LL_DMA_EnableChannel(ADC_DMA_ACTIVE, ADC_DMA_CHANNEL);

	/* ADC Calibration */
	Hal_ADC_GetCalibrationFactor(ADC_ACTIVE);

	/* ADC DMA request in circular mode */
    LL_DMA_SetMode(ADC_DMA_ACTIVE, LL_DMA_CHANNEL_1, LL_DMA_MODE_CIRCULAR); // 循环模式?

	/* Enable ADC_DMA */
    LL_ADC_REG_SetDMATransfer(ADC_ACTIVE, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

	/* Enable the ADC peripheral */
    LL_ADC_Enable(ADC_ACTIVE);

	/* Wait the ADRDY flag */
	while(!LL_ADC_IsActiveFlag_ADRDY(ADC_ACTIVE)){}; 

	/* ADC1 regular Software Start Conv */
    LL_ADC_REG_StartConversion(ADC_ACTIVE);
}


static uint32_t Hal_ADC_GetCalibrationFactor(ADC_TypeDef* ADCx)
{
  uint32_t calibrationcounter = 0;
  
  /* Set the ADC calibartion */
  LL_ADC_StartCalibration(ADCx);
  /* Wait until no ADC calibration is completed */
  
  do
  {
    calibrationcounter++;  
  } while((calibrationcounter != ((uint32_t)0x0000F000)) && LL_ADC_IsCalibrationOnGoing(ADCx));
    
  if(LL_ADC_IsCalibrationOnGoing(ADCx))
  {
  
    /* Error factor */
    return 0x00000000;
  }
  
   /*Get the calibration factor from the ADC data register */
   return LL_ADC_REG_ReadConversionData32(ADCx);
}


/**
  * @brief	¶ÁADCµÄÖµ£¬Î´´¦Àí
  * @param	channel Í¨µÀ
  * @note		
  * @retval  None
  */
uint16_t adcGetRawValue(uint8_t channel)
{
	uint16_t aver_value = 0;
	uint8_t i;

	for(i = 0; i < sizeof(adcValues) / sizeof(uint16_t) / ADC_CHANNEL_MAX; i++){
		aver_value += adcValues[i][adcConfig[channel].dmaIndex];
	}
	
    return aver_value;
}

/*
#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7c2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7b8))

// Î´²âÊÔ
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

//Î´²âÊÔ
uint32_t adcInternalVref(void)
{
	uint16_t vtemp;

	vtemp = adcGetRawValue(ADC_VREF);

	return (uint32_t)vtemp;
}
*/




