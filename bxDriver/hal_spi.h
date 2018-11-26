/**
  ******************************************************************************
  * @file   hal_spi.h
  * @author  
  * @version 
  * @date    
  * @brief    
  ******************************************************************************
  * @attention    v1.1      jgb                 20170515 
  ******************************************************************************
  */
#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_

/*
 *  硬件SPI
 *  模式0  默认CLK低，第一上升边沿捕获
 *  模式1  默认CLK低，第二上升边沿捕获
 *  模式2  默认CLK高，第一下降边沿捕获
 *  模式3  默认CLK高，第二下降边沿捕获
*/

#include "app_cfg.h"


//#define HAL_SPI1_THREE_LINE  // 定义则实现三线SPI

//spi1
#define HAL_SPI1_MODE        (0)  // see stm32cubeMX
/*定义硬件IO*/
#define HAL_SPI1_SCK_PORT        GPIOA
#define HAL_SPI1_SCK_PIN         LL_GPIO_PIN_5
#define HAL_SPI1_MISO_PORT       GPIOA
#define HAL_SPI1_MISO_PIN        LL_GPIO_PIN_6
#define HAL_SPI1_MOSI_PORT       GPIOA
#define HAL_SPI1_MOSI_PIN        LL_GPIO_PIN_7
#define HAL_SPI1_CS_PORT         GPIOA
#define HAL_SPI1_CS_PIN          LL_GPIO_PIN_2

#define HAL_SPI1_CS_HIGH()       LL_GPIO_SetOutputPin(HAL_SPI1_CS_PORT, HAL_SPI1_CS_PIN)
#define HAL_SPI1_CS_LOW()        LL_GPIO_ResetOutputPin(HAL_SPI1_CS_PORT, HAL_SPI1_CS_PIN)
#define HAL_SPI1_CS_DEASSERT()   HAL_SPI1_CS_HIGH()
#define HAL_SPI1_CS_ASSERT()     HAL_SPI1_CS_LOW()

#define HAL_SPI2_THREE_LINE  // 定义则实现三线SPI

// spi2
#define HAL_SPI2_MODE        (3) // see stm32cubeMX
/*定义硬件IO*/
#define HAL_SPI2_SCK_PORT        GPIOB
#define HAL_SPI2_SCK_PIN         LL_GPIO_PIN_13
#define HAL_SPI2_MISO_PORT       GPIOB
#define HAL_SPI2_MISO_PIN        LL_GPIO_PIN_14
#define HAL_SPI2_MOSI_PORT       GPIOB
#define HAL_SPI2_MOSI_PIN        LL_GPIO_PIN_15
#define HAL_SPI2_CS_PORT         GPIOB
#define HAL_SPI2_CS_PIN          LL_GPIO_PIN_12


#define HAL_SPI2_CS_HIGH()          LL_GPIO_SetOutputPin(HAL_SPI2_CS_PORT, HAL_SPI2_CS_PIN)
#define HAL_SPI2_CS_LOW()           LL_GPIO_ResetOutputPin(HAL_SPI2_CS_PORT, HAL_SPI2_CS_PIN)
#define HAL_SPI2_CS_DEASSERT()      HAL_SPI2_CS_HIGH()
#define HAL_SPI2_CS_ASSERT()        HAL_SPI2_CS_LOW()


void halSPI1_Init(void);						// SPI1初始化
uint8_t halSPI1_ReadWrite(uint8_t writedat);			// SPI1读写一字节数据
void halSPI2_Init(void);
uint8_t halSPI2_ReadWrite(uint8_t writedat);

#endif

