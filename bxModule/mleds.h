/**
  ******************************************************************************
  * @file   mleds.h
  * @author  
  * @version 
  * @date    
  * @brief    leds模块,可用于蜂鸣器,可用于继电器
  ******************************************************************************
  * @attention      v1.1   	jgb		重构                                    20151110
  * @attention      v1.2   	jgb		抽像宏，减少用户介入                            20170429
  * @attention      v1.3   	jgb		修改一些宏,增加对事件添加的抽像层                     20170522
  * @attention      v1.4   	jgb		修改基本时基处理,API更简洁,减少用户理解, 添加配置闪烁宏
  ******************************************************************************
  */
/*
;   定义coils使用的个数mLEDSNUM，
;   实现MTURN_LED1_ON 各个coils的on off功能宏
;   
;   调用API说明，
;   如果只是简单开,关,切换，调用函数mledset
;   如果是闪烁的功能,调用函数mledsetblink
;
*/

#ifndef __MLEDS_H_
#define __MLEDS_H_

#include "app_cfg.h"

//#include "hal_led.h"

// Enable blink led
#define configBLINK_LED         1


/* Defaults */
#define mLEDSNUM   1

/* leds - The leds number is the same as the bit position */
#define MLED_1     	((uint8_t)1 << 0)
#define MLED_2    	((uint8_t)1 << 1)
#define MLED_3     	((uint8_t)1 << 2)
#define MLED_4     	((uint8_t)1 << 3)
#define MLED_5     	((uint8_t)1 << 4)
#define MLED_6    	((uint8_t)1 << 5)
#define MLED_7     	((uint8_t)1 << 6)
#define MLED_8     	((uint8_t)1 << 7)
#define MLED_ALL   	(uint8_t)((1 << mLEDSNUM) - 1)

#define MLED_BLINK_CONTINOUS_TODO	    0    //!< 持续闪烁
#define MLED_BLINK_TODO      		    1    //!< 默认眨现次数
#define MLED_BLINK_DUTY_CYCLE 		    5    //!< 默认眨现占空比
#define MLED_FLASH_CYCLE_TIME           1000 //!< 默认周期，(以时基为准)

/* Modes */
typedef enum 
{
	MLED_MODE_OFF 		    = 0x00,
	MLED_MODE_ON			= 0x01,
	MLED_MODE_TOGGLE  	    = 0x02,
	MLED_MODE_BLINK	        = 0x04,
	MLED_MODE_FLASH         = 0x08
}MledMode_t;


#define MTURN_LED1_ON()		//GPIO_ResetBits(HAL_REDLED_PORT,HAL_REDLED_PIN)
#define MTURN_LED1_OFF()	//GPIO_SetBits(HAL_REDLED_PORT,HAL_REDLED_PIN)
#define MTURN_LED2_ON()	 	
#define MTURN_LED2_OFF()	
#define MTURN_LED3_ON()		
#define MTURN_LED3_OFF()	
#define MTURN_LED4_ON()		
#define MTURN_LED4_OFF()	
#define MTURN_LED5_ON()		
#define MTURN_LED5_OFF()	
#define MTURN_LED6_ON()		 
#define MTURN_LED6_OFF()	
#define MTURN_LED7_ON()		
#define MTURN_LED7_OFF()	
#define MTURN_LED8_ON()	
#define MTURN_LED8_OFF()

void mledInit(void);
void mledset(uint8_t leds, MledMode_t mode);
void mledsetblink(uint8_t leds, uint8_t numBlinks, uint8_t duty, uint32_t period);
uint8_t mledGetstatus(uint8_t which);

#endif


