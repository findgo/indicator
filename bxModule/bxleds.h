/**
  ******************************************************************************
  * @file   bxleds.h
  * @author  
  * @version 
  * @date    
  * @brief    leds模块,可用于蜂鸣器,可用于继电器, 对于少于8个的,使用mleds文件
  ******************************************************************************
  * @attention      v1.0   	jgb		支持到16个灯控制                        20181128
  ******************************************************************************
  */
/*
;   定义led使用的个数bxLEDSNUM，
;   实现MTURN_LED1_ON 各个coils的on off功能宏
;   
;   调用API说明，
;   如果只是简单开,关,切换，调用函数BxLEDset
;   如果是闪烁的功能,调用函数BxLEDsetblink
;
*/

#ifndef __BXLEDS_H_
#define __BXLEDS_H_

#include "app_cfg.h"

//#include "hal_led.h"

// Enable blink led
#define configBLINK_LED     (0)


/* Defaults max 8 */
#define bxLEDSNUM   11

/* leds - The leds number is the same as the bit position */
#define BXLED_1     	((uint16_t)1 << 0)
#define BXLED_2    	    ((uint16_t)1 << 1)
#define BXLED_3     	((uint16_t)1 << 2)
#define BXLED_4     	((uint16_t)1 << 3)
#define BXLED_5     	((uint16_t)1 << 4)
#define BXLED_6    	    ((uint16_t)1 << 5)
#define BXLED_7     	((uint16_t)1 << 6)
#define BXLED_8     	((uint16_t)1 << 7)
#define BXLED_9     	((uint16_t)1 << 8)
#define BXLED_10    	((uint16_t)1 << 9)
#define BXLED_11    	((uint16_t)1 << 10)
#define BXLED_12    	((uint16_t)1 << 11)
#define BXLED_13    	((uint16_t)1 << 12)
#define BXLED_14   	    ((uint16_t)1 << 13)
#define BXLED_15    	((uint16_t)1 << 14)
#define BXLED_16    	((uint16_t)1 << 15)
#define BXLED_ALL   	(uint16_t)((1 << bxLEDSNUM) - 1)

#define BXLED_BLINK_CONTINOUS_TODO	     0    //!< 持续闪烁
#define BXLED_BLINK_TODO      		     1    //!< 默认眨现次数
#define BXLED_BLINK_DUTY_CYCLE 		     5    //!< 默认眨现占空比
#define BXLED_FLASH_CYCLE_TIME           1000 //!< 默认周期，(以时基为准)

/* Modes */
typedef enum 
{
	BXLED_MODE_OFF 		    = 0x00,
	BXLED_MODE_ON			= 0x01,
	BXLED_MODE_TOGGLE  	    = 0x02,
	BXLED_MODE_BLINK	    = 0x04,
	BXLED_MODE_FLASH        = 0x08
}BxLEDMode_t;


#define BXTURN_LED1_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDA1_PORT,BX_HAL_LEDA1_PIN)
#define BXTURN_LED1_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDA1_PORT,BX_HAL_LEDA1_PIN)
#define BXTURN_LED2_ON()	 	LL_GPIO_ResetOutputPin(BX_HAL_LEDA2_PORT,BX_HAL_LEDA2_PIN)
#define BXTURN_LED2_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDA2_PORT,BX_HAL_LEDA2_PIN)
#define BXTURN_LED3_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDB1_PORT,BX_HAL_LEDB1_PIN)
#define BXTURN_LED3_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDB1_PORT,BX_HAL_LEDB1_PIN)
#define BXTURN_LED4_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDB2_PORT,BX_HAL_LEDB2_PIN)
#define BXTURN_LED4_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDB2_PORT,BX_HAL_LEDB2_PIN)
#define BXTURN_LED5_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDC1_PORT,BX_HAL_LEDC1_PIN)
#define BXTURN_LED5_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDC1_PORT,BX_HAL_LEDC1_PIN)
#define BXTURN_LED6_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDC2_PORT,BX_HAL_LEDC2_PIN) 
#define BXTURN_LED6_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDC2_PORT,BX_HAL_LEDC2_PIN)
#define BXTURN_LED7_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDD1_PORT,BX_HAL_LEDD1_PIN)
#define BXTURN_LED7_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDD1_PORT,BX_HAL_LEDD1_PIN)
#define BXTURN_LED8_ON()	    LL_GPIO_ResetOutputPin(BX_HAL_LEDD2_PORT,BX_HAL_LEDD2_PIN)
#define BXTURN_LED8_OFF()       LL_GPIO_SetOutputPin(BX_HAL_LEDD2_PORT,BX_HAL_LEDD2_PIN)
#define BXTURN_LED9_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDE1_PORT,BX_HAL_LEDE1_PIN)
#define BXTURN_LED9_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDE1_PORT,BX_HAL_LEDE1_PIN)
#define BXTURN_LED10_ON()	 	LL_GPIO_ResetOutputPin(BX_HAL_LEDE2_PORT,BX_HAL_LEDE2_PIN)
#define BXTURN_LED10_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDE2_PORT,BX_HAL_LEDE2_PIN)
#define BXTURN_LED11_ON()		LL_GPIO_ResetOutputPin(BX_HAL_LEDES_PORT,BX_HAL_LEDES_PIN)    // 储能指示开
#define BXTURN_LED11_OFF()	    LL_GPIO_SetOutputPin(BX_HAL_LEDES_PORT,BX_HAL_LEDES_PIN)      // 储能指示关
#define BXTURN_LED12_ON()		
#define BXTURN_LED12_OFF()	
#define BXTURN_LED13_ON()		
#define BXTURN_LED13_OFF()	
#define BXTURN_LED14_ON()		 
#define BXTURN_LED14_OFF()	
#define BXTURN_LED15_ON()		
#define BXTURN_LED15_OFF()	
#define BXTURN_LED16_ON()	
#define BXTURN_LED16_OFF()

void bxLedInit(void);
void bxLedset(uint16_t leds, BxLEDMode_t mode);
void bxLedsetblink(uint16_t leds, uint8_t numBlinks, uint8_t duty, uint32_t period);
uint16_t bxLedGetstatus(uint16_t which);

#endif


