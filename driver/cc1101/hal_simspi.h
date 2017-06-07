


#ifndef _DRIVER_HAL_SIMSPI_H_
#define _DRIVER_HAL_SIMSPI_H_

#include "app_cfg.h"

#define HAL_SIMSPI_MODE        (0)

/*定义硬件IO*/
#define HAL_SIMSPI_SCK_PORT        GPIOA
#define HAL_SIMSPI_SCK_PIN         GPIO_Pin_5
#define HAL_SIMSPI_MISO_PORT       GPIOA
#define HAL_SIMSPI_MISO_PIN        GPIO_Pin_6
#define HAL_SIMSPI_MOSI_PORT       GPIOA
#define HAL_SIMSPI_MOSI_PIN        GPIO_Pin_7
#define HAL_SIMSPI_CS_PORT         GPIOA
#define HAL_SIMSPI_CS_PIN          GPIO_Pin_4

#define HAL_SIMSPI_GPIO_CLOCK_EN() do{RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);}while(0)
    

/*SPI宏*/
#define HAL_SIMSPI_TIME_CLOCK_WIDE()           //delay_us(1)//延时间隔100ns以上

#define HAL_SIMSPI_SCK_OUTPUT_MODE()           /* 设置为output模式 */ 
#define HAL_SIMSPI_SCK_LOW()                   GPIO_ResetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)              
#define HAL_SIMSPI_SCK_HIGH()                  GPIO_SetBits(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)               

#define HAL_SIMSPI_MISO_INPUT_MODE()            /* 设置为input模式 */ 
#define HAL_SIMSPI_MISO_READ                   GPIO_ReadInputDataBit(HAL_SIMSPI_MISO_PORT,HAL_SIMSPI_MISO_PIN)

#define HAL_SIMSPI_MOSI_OUTPUT_MODE()           /* 设置为output模式 */ 
#define HAL_SIMSPI_MOSI_LOW()                  GPIO_ResetBits(HAL_SIMSPI_MOSI_PORT,HAL_SIMSPI_MOSI_PIN)    
#define HAL_SIMSPI_MOSI_HIGH()                 GPIO_SetBits(HAL_SIMSPI_MOSI_PORT,HAL_SIMSPI_MOSI_PIN)      

#define HAL_SIMSPI_CS_OUTPUT_MODE()            /* 设置为output模式 */ 
#define HAL_SIMSPI_CS_DEASSERT()               GPIO_SetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)
#define HAL_SIMSPI_CS_ASSERT()                 GPIO_ResetBits(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)

void halsimspiInit(void);
uint8_t halsimSpi_transmit_byte(uint8_t dat);

#endif


