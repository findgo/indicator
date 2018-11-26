
/* 实现模拟SPI时序 
 *  模式0  默认CLK低，第一上升边沿捕获
 *  模式1  默认CLK低，第二上升边沿捕获
 *  模式2  默认CLK高，第一下降边沿捕获
 *  模式3  默认CLK高，第二下降边沿捕获
 */
#ifndef _DRIVER_HAL_SIMSPI_H_
#define _DRIVER_HAL_SIMSPI_H_

#include "app_cfg.h"
#include "delay.h"

#define HAL_SIMSPI_MODE        (3)
#define HAL_SIMSPI_THREE_LINE  // 定义则实现三线SPI

/*定义硬件IO*/
#define HAL_SIMSPI_SCK_PORT        GPIOA
#define HAL_SIMSPI_SCK_PIN         LL_GPIO_PIN_13
#define HAL_SIMSPI_MISO_PORT       GPIOA
#define HAL_SIMSPI_MISO_PIN        LL_GPIO_PIN_14
#define HAL_SIMSPI_MOSI_PORT       GPIOA
#define HAL_SIMSPI_MOSI_PIN        LL_GPIO_PIN_15
#define HAL_SIMSPI_CS_PORT         GPIOA
#define HAL_SIMSPI_CS_PIN          LL_GPIO_PIN_12

//#define HAL_SIMSPI_GPIO_CLOCK_EN() do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA ,ENABLE );}while(0)
    

/*SPI宏*/
#define HAL_SIMSPI_TIME_CLOCK_WIDE()           delay_hunns(2)//延时间隔100ns以上

#define HAL_SIMSPI_SCK_OUTPUT_MODE()           /* 设置为output模式 */ 
#define HAL_SIMSPI_SCK_LOW()                  LL_GPIO_ResetOutputPin(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)              
#define HAL_SIMSPI_SCK_HIGH()                  LL_GPIO_SetOutputPin(HAL_SIMSPI_SCK_PORT, HAL_SIMSPI_SCK_PIN)               

#define HAL_SIMSPI_MISO_INPUT_MODE()            /* 设置为input模式 */ 
#define HAL_SIMSPI_MISO_READ                   //GPIO_ReadInputDataBit(HAL_SIMSPI_MISO_PORT,HAL_SIMSPI_MISO_PIN)

#define HAL_SIMSPI_MOSI_OUTPUT_MODE()           /* 设置为output模式 */ 
#define HAL_SIMSPI_MOSI_LOW()                  LL_GPIO_ResetOutputPin(HAL_SIMSPI_MOSI_PORT,HAL_SIMSPI_MOSI_PIN)    
#define HAL_SIMSPI_MOSI_HIGH()                 LL_GPIO_SetOutputPin(HAL_SIMSPI_MOSI_PORT,HAL_SIMSPI_MOSI_PIN)      

#define HAL_SIMSPI_CS_OUTPUT_MODE()            /* 设置为output模式 */ 
#define HAL_SIMSPI_CS_DEASSERT()               LL_GPIO_SetOutputPin(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)
#define HAL_SIMSPI_CS_ASSERT()                 LL_GPIO_ResetOutputPin(HAL_SIMSPI_CS_PORT,HAL_SIMSPI_CS_PIN)

void halsimspiInit(void);
uint8_t halsimSpi_transmit_byte(uint8_t dat);

#endif


