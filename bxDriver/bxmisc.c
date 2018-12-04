
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

uint8_t bxMiscGetSlaveID(void)
{
    uint8_t value = LL_GPIO_IsInputPinSet(DAT6_GPIO_Port, BX_HAL_DATA6_PIN) << 6;
    value |= LL_GPIO_IsInputPinSet(DAT5_GPIO_Port, BX_HAL_DATA5_PIN) << 5;
    value |= LL_GPIO_IsInputPinSet(DAT4_GPIO_Port, BX_HAL_DATA4_PIN) << 4;
    value |= LL_GPIO_IsInputPinSet(DAT3_GPIO_Port, BX_HAL_DATA3_PIN) << 3;
    value |= LL_GPIO_IsInputPinSet(DAT2_GPIO_Port, BX_HAL_DATA2_PIN) << 2;
    value |= LL_GPIO_IsInputPinSet(DAT1_GPIO_Port, BX_HAL_DATA1_PIN) << 1;
    value |= LL_GPIO_IsInputPinSet(DAT0_GPIO_Port, BX_HAL_DATA0_PIN) << 0;

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

    mcksAssign(MCKS_ALL, bxMiscCheckHandle); // 分配同一个回调
    
    while(ckss)
    {
        if(ckss & cks){
            bxMiscCheckHandle(cks, ((value & cks) > 0) ? TRUE : FALSE ); // 执行初始
        
            ckss ^= cks;
        }
        cks <<= 1;
    }
}

// 有io电平变化,触发对应IO的回调
static void bxMiscCheckHandle(uint16_t cks, uint8_t isHigh)
{
     misc_logln("cks: 0x%04x, level: %d", cks, isHigh);
    //TODO: your job
    switch (cks){
    case MCKS_ALARM_TEST1:
        break;
    case MCKS_ALARM_TEST2:
        break;
    case MCKS_IN_WORKSTATUS:
        if(isHigh){ // 工作位置触点断开
            // 指示触点断开,灯灭
        }
        else{ 
            // 指示触点闭合,灯亮
        }
        // 修改工作位置触点 标志位
        break;
    case MCKS_IN_TESTSTATUS:
        if(isHigh){ // 试验位置触点断开
            // 指示触点断开, 灯灭
        }
        else{ 
            // 指示触点闭合,灯亮
        }
        // 修改试验位置触点 标志位
        break;
    case MCKS_IN_GROUND:
        if(isHigh){ // 接地合闸断开
            // 指示触点闭合灯灭
            // 指示触点断开灯亮
        }
        else{ 
            // 指示触点闭合灯亮
            // 指示触点断开灯灭
        }
        // 修改接地合闸标志位
        break;
    case MCKS_IN_ENERGYSTORAGE: 
        if(isHigh){ // 储能分开
            // 指示储能灯灭
        }
        else{ 
            // 指示储能灯亮
        }
        // 修改储能标志位
        break;
    case MCKS_IN_SWITCHON:
        if(isHigh){ // 断路器合闸位置触点 断开
            // 指示触点断开, 灯灭
        }
        else{ 
            // 指示触点闭合, 灯亮
        }
        // 修改断路器合位置触点 标志位
        break;
    case MCKS_IN_SWITCHOFF:
        if(isHigh){ // 断路器分闸位置触点 断开
            // 指示触点断开, 灯灭
        }
        else{ 
            // 指示触点闭合, 灯亮
        }
        // 修改断路器分位置触点 标志位
        break;
    case MCKS_LIVEJUDGMENT:
        if(isHigh){ // 正常
            // 指示正常, 灯灭
        }
        else{ 
            // 指示断线报警, 灯亮
        }
        // 修改断线报触 标志位
        break;
    default:
        break;
    }

    // process all flag 
    

    
}
