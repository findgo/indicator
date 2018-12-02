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

#define mCKSNum    8
#define MCKS_FILTERTIME    50

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

/* cks 指明哪个IO 发生变化,用于同一个回调识别不同IO变化
 * 电平变化时,触发回调,  TRUE指明上降沿触发,当前高电平. FALSE: 下升沿触发,当前低电平 */
typedef void (*cksCallBackFunc_t)(uint16_t cks, uint8_t isHigh); 

void mcksAssign(uint16_t ckss, cksCallBackFunc_t pcallback);
void mcks_Task(void);
uint16_t mcksGetLowStatus(void);


/* IO电平,高平电返回1,低电平返回0 */
#define MCKS1_LEVEL()    1
#define MCKS2_LEVEL()    1
#define MCKS3_LEVEL()    1
#define MCKS4_LEVEL()    1
#define MCKS5_LEVEL()    1
#define MCKS6_LEVEL()    1
#define MCKS7_LEVEL()    1
#define MCKS8_LEVEL()    1
#define MCKS9_LEVEL()    
#define MCKS10_LEVEL()    
#define MCKS11_LEVEL()    
#define MCKS12_LEVEL()    
#define MCKS13_LEVEL()    
#define MCKS14_LEVEL()    
#define MCKS15_LEVEL()    
#define MCKS16_LEVEL()

#endif



