

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
#define BX_HAL_DATA0_PORT   DAT0_KEY_UP_GPIO_Port
#define BX_HAL_DATA0_PIN    DAT0_KEY_UP_Pin
#define BX_HAL_DATA1_PORT   DAT1_KEY_DOWN_GPIO_Port
#define BX_HAL_DATA1_PIN    DAT1_KEY_DOWN_Pin
#define BX_HAL_DATA2_PORT   DAT2_KEY_LEFT_GPIO_Port
#define BX_HAL_DATA2_PIN    DAT2_KEY_LEFT_Pin
#define BX_HAL_DATA3_PORT   DAT3_KEY_RIGHT_GPIO_Port
#define BX_HAL_DATA3_PIN    DAT3_KEY_RIGHT_Pin
#define BX_HAL_DATA4_PORT   DAT4_KEY_ENTER_GPIO_Port
#define BX_HAL_DATA4_PIN    DAT4_KEY_ENTER_Pin
#define BX_HAL_DATA5_PORT   DAT5_LCD_RS_GPIO_Port
#define BX_HAL_DATA5_PIN    DAT5_LCD_RS_Pin
#define BX_HAL_DATA6_PORT   DAT6_LCD_MOSI_GPIO_Port
#define BX_HAL_DATA6_PIN    DAT6_LCD_MOSI_Pin


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

// define led  used in bxleds.h
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

//define key used in hal_key.h mkey.h
#define BX_HAL_KEY_SWITCH_MODE_PORT   HAL_KEY_GPIO_Port  // 切换手动/自动控制模式
#define BX_HAL_KEY_SWITCH_MODE_PIN    HAL_KEY_Pin

// define input
#define BX_HAL_ALARM_TEST1_PORT         ALARM_TEST1_GPIO_Port           // 加热检测1
#define BX_HAL_ALARM_TEST1_PIN          ALARM_TEST1_Pin
#define BX_HAL_ALARM_TEST2_PORT         ALARM_TEST2_GPIO_Port           // 加热检测2
#define BX_HAL_ALARM_TEST2_PIN          ALARM_TEST2_Pin
#define BX_HAL_IN_ENERGYSTORAGE_PORT    IN_EnergyStorage_GPIO_Port      // 储能
#define BX_HAL_IN_ENERGYSTORAGE_PIN     IN_EnergyStorage_Pin
#define BX_HAL_IN_SWITCHON_PORT         IN_SwitchOn_GPIO_Port           // 断路器合
#define BX_HAL_IN_SWITCHON_PIN          IN_SwitchOn_Pin
#define BX_HAL_IN_SWITCHOFF_PORT        IN_SwitchOff_GPIO_Port          // 断路器分
#define BX_HAL_IN_SWITCHOFF_PIN         IN_SwitchOff_Pin
#define BX_HAL_IN_WORKSTATUS_PORT       IN_WorkStatus_GPIO_Port         // 工作状态
#define BX_HAL_IN_WORKSTATUS_PIN        IN_WorkStatus_Pin
#define BX_HAL_IN_TESTSTATUS_PORT       IN_TestStatus_GPIO_Port         // 试验位置
#define BX_HAL_IN_TESTSTATUS_PIN        IN_TestStatus_Pin
#define BX_HAL_IN_GROUND_PORT           IN_Ground_GPIO_Port             // 接地刀闸
#define BX_HAL_IN_GROUND_PIN            IN_Ground_Pin
#define BX_HAL_LIVEJUDGMENT_PORT        LIVEJUDGMENT_GPIO_Port          // 断线检测
#define BX_HAL_LIVEJUDGMENT_PIN         LIVEJUDGMENT_Pin
#define BX_HAL_IN_SPARE1_PORT           
#define BX_HAL_IN_SPARE1_PIN            
#define BX_HAL_IN_SPARE2_PORT           
#define BX_HAL_IN_SPARE2_PIN      




#endif
