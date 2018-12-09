/**
  ******************************************************************************
  * @file   mcheck.h
  * @author  
  * @version 
  * @date    
  * @brief    moduble check模块  用于边缘电平变化触发，一次性，支持下降沿和上升沿触发回调，双边缘滤波
  ******************************************************************************
  * @attention     v1.1   	jgb		重构  20161113
  * @attention     v1.2   	jgb		优化结构,初始化电平,隐藏数据暴露     20181202
  ******************************************************************************
  */

#ifndef __MCHECK_H_
#define __MCHECK_H_

#include "app_cfg.h"
#include "bxmisc.h"

#define mCKSNum             9
#define MCKS_FILTERTIME     5

/* leds - The leds number is the same as the bit position */
#define MCKS_1     	((uint16_t)1 << 0)
#define MCKS_2    	((uint16_t)1 << 1)
#define MCKS_3     	((uint16_t)1 << 2)
#define MCKS_4     	((uint16_t)1 << 3)
#define MCKS_5     	((uint16_t)1 << 4)
#define MCKS_6    	((uint16_t)1 << 5)
#define MCKS_7     	((uint16_t)1 << 6)
#define MCKS_8     	((uint16_t)1 << 7)
#define MCKS_9     	((uint16_t)1 << 8)
#define MCKS_10    	((uint16_t)1 << 9)
#define MCKS_11    	((uint16_t)1 << 10)
#define MCKS_12    	((uint16_t)1 << 11)
#define MCKS_13    	((uint16_t)1 << 12)
#define MCKS_14   	((uint16_t)1 << 13)
#define MCKS_15    	((uint16_t)1 << 14)
#define MCKS_16    	((uint16_t)1 << 15)
#define MCKS_ALL   	(uint16_t)((1 << mCKSNum) - 1)

/* cks 指明哪个IO 发生变化,可用于同一个回调识别不同IO变化
 * 电平变化时,触发回调,  TRUE指明上降沿触发,当前高电平. FALSE: 下升沿触发,当前低电平 */
typedef void (*cksCallBackFunc_t)(uint16_t cks, uint8_t isHigh); 

void mcksAssign(uint16_t ckss, cksCallBackFunc_t pcallback);
void mcks_Task(void);
uint16_t mcksGetLowStatus(void);

// user defined 
#define MCKS_ALARM_TEST1        MCKS_1
#define MCKS_ALARM_TEST2        MCKS_2
#define MCKS_IN_ENERGYSTORAGE   MCKS_3
#define MCKS_IN_SWITCHON        MCKS_4
#define MCKS_IN_SWITCHOFF       MCKS_5
#define MCKS_IN_WORKSTATUS      MCKS_6
#define MCKS_IN_TESTSTATUS      MCKS_7
#define MCKS_IN_GROUND          MCKS_8
#define MCKS_LIVEJUDGMENT       MCKS_9


/* IO电平,高平电返回1,低电平返回0 */
#define MCKS1_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_ALARM_TEST1_PORT, BX_HAL_ALARM_TEST1_PIN)
#define MCKS2_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_ALARM_TEST2_PORT, BX_HAL_ALARM_TEST2_PIN)
#define MCKS3_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_ENERGYSTORAGE_PORT, BX_HAL_IN_ENERGYSTORAGE_PIN)
#define MCKS4_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_SWITCHON_PORT, BX_HAL_IN_SWITCHON_PIN)
#define MCKS5_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_SWITCHOFF_PORT, BX_HAL_IN_SWITCHOFF_PIN)
#define MCKS6_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_WORKSTATUS_PORT, BX_HAL_IN_WORKSTATUS_PIN)
#define MCKS7_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_TESTSTATUS_PORT, BX_HAL_IN_TESTSTATUS_PIN)
#define MCKS8_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_IN_GROUND_PORT, BX_HAL_IN_GROUND_PIN)
#define MCKS9_LEVEL()    LL_GPIO_IsInputPinSet(BX_HAL_LIVEJUDGMENT_PORT, BX_HAL_LIVEJUDGMENT_PIN)
#define MCKS10_LEVEL()   //LL_GPIO_IsInputPinSet(BX_HAL_IN_SPARE1_PORT, BX_HAL_IN_SPARE1_PIN)
#define MCKS11_LEVEL()   //LL_GPIO_IsInputPinSet(BX_HAL_IN_SPARE2_PORT, BX_HAL_IN_SPARE2_PIN)
#define MCKS12_LEVEL()    
#define MCKS13_LEVEL()    
#define MCKS14_LEVEL()    
#define MCKS15_LEVEL()    
#define MCKS16_LEVEL()

#endif



