

/* 裁剪工程功能的 */
#ifndef __HAL_BOARD_CFG_H__
#define __HAL_BOARD_CFG_H__



#include "common_def.h"
#include "common_type.h"

//for stm32Cube drivers
#include "gpio.h"
//#include "spi.h"
#include "usart.h"
#include "dma.h"
#include "adc.h"
#include "main.h"
#include "wwdg.h"
#include "tim.h"

// 配置功能
// 支持Modbus
#define configSUPPORT_MODBUS           ( 1 )



// dat0 - dat6 used in misc
#define BX_HAL_DATA0_PORT   DAT0_GPIO_Port
#define BX_HAL_DATA0_PIN    DAT0_Pin
#define BX_HAL_DATA1_PORT   DAT1_GPIO_Port
#define BX_HAL_DATA1_PIN    DAT1_Pin
#define BX_HAL_DATA2_PORT   DAT2_GPIO_Port
#define BX_HAL_DATA2_PIN    DAT2_Pin
#define BX_HAL_DATA3_PORT   DAT3_GPIO_Port
#define BX_HAL_DATA3_PIN    DAT3_Pin
#define BX_HAL_DATA4_PORT   DAT4_GPIO_Port
#define BX_HAL_DATA4_PIN    DAT4_Pin
#define BX_HAL_DATA5_PORT   DAT5_GPIO_Port
#define BX_HAL_DATA5_PIN    DAT5_Pin
#define BX_HAL_DATA6_PORT   DAT6_GPIO_Port
#define BX_HAL_DATA6_PIN    DAT6_Pin


// define coils  used in mcoils.h
#define BX_HAL_RELAY_ALARM1_PORT    RELAY_ALARM1_GPIO_Port
#define BX_HAL_RELAY_ALARM1_PIN     RELAY_ALARM1_Pin
#define BX_HAL_RELAY_ALARM2_PORT    RELAY_ALARM2_GPIO_Port
#define BX_HAL_RELAY_ALARM2_PIN     RELAY_ALARM2_Pin
#define BX_HAL_RELAY_HOT1_PORT      RELAY_HOT1_GPIO_Port
#define BX_HAL_RELAY_HOT1_PIN       RELAY_HOT1_Pin
#define BX_HAL_RELAY_HOT2_PORT      RELAY_HOT2_GPIO_Port
#define BX_HAL_RELAY_HOT2_PIN       RELAY_HOT2_Pin
#define BX_HAL_RELAY_CLOSURE_PORT   RELAY_CLOSURE_GPIO_Port
#define BX_HAL_RELAY_CLOSURE_PIN    RELAY_CLOSURE_Pin

#define BX_HAL_LEDA1_PORT       LED_A1_GPIO_Port
#define BX_HAL_LEDA1_PIN        LED_A1_Pin
#define BX_HAL_LEDA2_PORT       LED_A2_GPIO_Port
#define BX_HAL_LEDA2_PIN        LED_A2_Pin
#define BX_HAL_LEDB1_PORT       LED_B1_GPIO_Port
#define BX_HAL_LEDB1_PIN        LED_B1_Pin
#define BX_HAL_LEDB2_PORT       LED_B2_GPIO_Port
#define BX_HAL_LEDB2_PIN        LED_B2_Pin
#define BX_HAL_LEDC1_PORT       LED_C1_GPIO_Port
#define BX_HAL_LEDC1_PIN        LED_C1_Pin
#define BX_HAL_LEDC2_PORT       LED_C2_GPIO_Port
#define BX_HAL_LEDC2_PIN        LED_C2_Pin
#define BX_HAL_LEDD1_PORT       LED_D1_GPIO_Port
#define BX_HAL_LEDD1_PIN        LED_D1_Pin
#define BX_HAL_LEDD2_PORT       LED_D2_GPIO_Port
#define BX_HAL_LEDD2_PIN        LED_D2_Pin
#define BX_HAL_LEDE1_PORT       LED_E1_GPIO_Port
#define BX_HAL_LEDE1_PIN        LED_E1_Pin
#define BX_HAL_LEDE2_PORT       LED_E2_GPIO_Port
#define BX_HAL_LEDE2_PIN        LED_E2_Pin
#define BX_HAL_LEDES_PORT       LEDEnergyStorage_GPIO_Port
#define BX_HAL_LEDES_PIN        LEDEnergyStorage_Pin


#endif
