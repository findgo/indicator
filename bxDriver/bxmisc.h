/* 杂项驱动 */
#ifndef __BXMISC_H__
#define __BXMISC_H__

#include "app_cfg.h" 


#define BX_MISC_SCAN_TIME   10
#define HAL_ADC_SCAN_TIME   1000

uint8_t bxMiscGetSlaveID(void);
uint8_t BxGPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin);

void bxMiscInit(void);
void bxMiscTask(void);


#endif

