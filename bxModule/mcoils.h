/**
  ******************************************************************************
  * @file   mcoils.h
  * @author  
  * @version 
  * @date    
  * @brief    coils模块，只有开和关两状态都可以归于这一类别
  ******************************************************************************
  * @attention    v1.1      jgb     重构                                20151113 
  * @attention    v1.2      jgb     简化功能修改更少的宏就可使用                  20170428
  * @attention    v1.3      jgb     优化底层                            20181013
  * @attention    v1.4      jgb     修正mCoilsSet错误,只要大于等于宏MCOILS_MODE_TOGGLE,都认为是切换                   20181128
  ******************************************************************************
  */
/* 使用说明
;   定义coils使用的个数mCOILSNUM，
;   实现MTURN_COILS1_ON 各个coils的功能宏
;   调用API
*/
#ifndef __MCOILS_H_
#define __MCOILS_H_

#include "app_cfg.h"


//define Coils Numbers you want  Max 8
#define mCOILSNUM   5

/* COLIS - The COLIS number is the same as the bit position */
#define MCOILS_1        ((uint8_t)1 << 0)
#define MCOILS_2        ((uint8_t)1 << 1)
#define MCOILS_3        ((uint8_t)1 << 2)
#define MCOILS_4        ((uint8_t)1 << 3)
#define MCOILS_5        ((uint8_t)1 << 4)
#define MCOILS_6        ((uint8_t)1 << 5)
#define MCOILS_7        ((uint8_t)1 << 6)
#define MCOILS_8        ((uint8_t)1 << 7)
#define MCOILS_ALL      (uint8_t)((1 << mCOILSNUM) - 1)

// user defined used in app
#define MCOILS_ALARM1   MCOILS_1
#define MCOILS_ALARM2   MCOILS_2
#define MCOILS_HOT1     MCOILS_3
#define MCOILS_HOT2     MCOILS_4
#define MCOILS_CLOSURE  MCOILS_5

/* Modes */
typedef enum 
{
    MCOILS_MODE_OFF     = 0x00,
    MCOILS_MODE_ON      = 0x01,
    MCOILS_MODE_TOGGLE  = 0x02,
}mCoils_Mode_t;

// 抽像继电器底层
#define MTURN_COILS1_ON()       LL_GPIO_SetOutputPin(BX_HAL_RELAY_ALARM1_PORT, BX_HAL_RELAY_ALARM1_PIN) 
#define MTURN_COILS1_OFF()      LL_GPIO_ResetOutputPin(BX_HAL_RELAY_ALARM1_PORT, BX_HAL_RELAY_ALARM1_PIN)
#define MTURN_COILS2_ON()       LL_GPIO_SetOutputPin(BX_HAL_RELAY_ALARM2_PORT, BX_HAL_RELAY_ALARM2_PIN) 
#define MTURN_COILS2_OFF()      LL_GPIO_ResetOutputPin(BX_HAL_RELAY_ALARM2_PORT, BX_HAL_RELAY_ALARM2_PIN)
#define MTURN_COILS3_ON()       LL_GPIO_SetOutputPin(BX_HAL_RELAY_HOT1_PORT, BX_HAL_RELAY_HOT1_PIN) 
#define MTURN_COILS3_OFF()      LL_GPIO_ResetOutputPin(BX_HAL_RELAY_HOT1_PORT, BX_HAL_RELAY_HOT1_PIN)
#define MTURN_COILS4_ON()       LL_GPIO_SetOutputPin(BX_HAL_RELAY_HOT2_PORT, BX_HAL_RELAY_HOT2_PIN) 
#define MTURN_COILS4_OFF()      LL_GPIO_ResetOutputPin(BX_HAL_RELAY_HOT2_PORT, BX_HAL_RELAY_HOT2_PIN)
#define MTURN_COILS5_ON()       LL_GPIO_SetOutputPin(BX_HAL_RELAY_CLOSURE_PORT, BX_HAL_RELAY_CLOSURE_PIN)    
#define MTURN_COILS5_OFF()      LL_GPIO_ResetOutputPin(BX_HAL_RELAY_CLOSURE_PORT, BX_HAL_RELAY_CLOSURE_PIN)
#define MTURN_COILS6_ON()         
#define MTURN_COILS6_OFF()   
#define MTURN_COILS7_ON()
#define MTURN_COILS7_OFF()
#define MTURN_COILS8_ON()
#define MTURN_COILS8_OFF()

void mCoilsInit(void);
void mCoilsSet (uint8_t Coils, mCoils_Mode_t mode);
uint8_t mCoilsGetStasus(uint8_t which);

#endif

