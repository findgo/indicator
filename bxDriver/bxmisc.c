
#include "bxmisc.h"
#include "mkey.h"
#include "timers.h"
#include "mcheck.h"
#include "hal_adc.h"

#define misc_logln(format,args...) log_debugln(format,##args )

static void bxMiscCheckInit(void);
static uint8_t halkeySwitchModeIsDown(void);
static void bxMiscScanCB(void *arg);
static void bxMiscCheckHandle(uint16_t cks, uint8_t isHigh);

static void haladcCB(void *arg);;

static TimerHandle_t adctimeHandle;
static TimerStatic_t adctimer;

static TimerHandle_t misctimeHandle;
static TimerStatic_t misctimer;
static mkeycfgStatic_t keycfg;

void bxMiscInit(void)
{  
    mkeyAssign(&keycfg,  halkeySwitchModeIsDown, MKEY_PRESS1_DOWN, MKEY_NULL, MKEY_NULL, 0, 0, 0);
    bxMiscCheckInit();

    misctimeHandle = timerAssign(&misctimer, bxMiscScanCB , (void *)&misctimeHandle);
    timerStart(misctimeHandle, BX_MISC_SCAN_TIME);
    
    adctimeHandle = timerAssign(&adctimer, haladcCB , (void *)&adctimeHandle);
    timerStart(adctimeHandle, HAL_ADC_SCAN_TIME);
}

uint8_t BxGPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    if ((LL_GPIO_ReadInputPort(GPIOx) & GPIO_Pin) != (uint32_t)0){
        return 0x01;
    }
    
    return 0x00;
}

uint8_t bxMiscGetSlaveID(void)
{
    uint8_t value = BxGPIO_ReadInputDataBit(DAT6_GPIO_Port, BX_HAL_DATA6_PIN) << 6;
    value |= BxGPIO_ReadInputDataBit(DAT5_GPIO_Port, BX_HAL_DATA5_PIN) << 5;
    value |= BxGPIO_ReadInputDataBit(DAT4_GPIO_Port, BX_HAL_DATA4_PIN) << 4;
    value |= BxGPIO_ReadInputDataBit(DAT3_GPIO_Port, BX_HAL_DATA3_PIN) << 3;
    value |= BxGPIO_ReadInputDataBit(DAT2_GPIO_Port, BX_HAL_DATA2_PIN) << 2;
    value |= BxGPIO_ReadInputDataBit(DAT1_GPIO_Port, BX_HAL_DATA1_PIN) << 1;
    value |= BxGPIO_ReadInputDataBit(DAT0_GPIO_Port, BX_HAL_DATA0_PIN) << 0;

    return value;
}

static uint8_t halkeySwitchModeIsDown(void)
{
    return !(LL_GPIO_ReadInputPort(BX_HAL_KEY_SWITCH_MODE_PORT) & BX_HAL_KEY_SWITCH_MODE_PIN);
}

static void bxMiscScanCB(void *arg)
{
    mkeydecetor_task();
    mcks_Task();
    timerRestart(*((TimerHandle_t *)arg), BX_MISC_SCAN_TIME);
}


void bxMiscTask(void)
{
    if(mkeygetvalue() == MKEY_PRESS1_DOWN){
        misc_logln("swtich mode  key down!");
    }
}

static void haladcCB(void *arg)
{
    //log_debugln("tmp1: %d", adcGetRawValue(ADC_NTC1));
    //log_debugln("humi1: %d", adcGetRawValue(ADC_NTC2));
    //log_debugln("tmp2: %d", adcGetRawValue(ADC_NTC3));
    //log_debugln("humi2: %d", adcGetRawValue(ADC_NTC4));
    //TODO: your job
    
    timerRestart(*((TimerHandle_t *)arg), HAL_ADC_SCAN_TIME);
}

static void bxMiscCheckInit(void)
{
    uint16_t cks = MCKS_1;
    uint16_t ckss = MCKS_ALL;
    uint16_t value = mcksGetLowStatus();
    
    mcksAssign(MCKS_ALL, bxMiscCheckHandle);
    
    while(ckss)
    {
        if(ckss & cks){
            bxMiscCheckHandle(cks, ((value & cks) > 0) ? TRUE : FALSE ); // 执行初始
        
            ckss ^= cks;
        }
        cks <<= 1;
    }
}

static void bxMiscCheckHandle(uint16_t cks, uint8_t isHigh)
{
     misc_logln("cks: 0x%04x, level: %d", cks, isHigh);
    //TODO: your job
     
}
