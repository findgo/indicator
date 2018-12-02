#include "hal_key.h"

#define HAL_KEY_SCAN_TIME   10

#define key_log(format,args...) log_debugln(format,##args )

static uint8_t halkeySwitchModeIsDown(void);
static void halkeyCB(void *arg);


static TimerHandle_t keytimeHandle;
static TimerStatic_t keytimer;
static mkeycfgStatic_t keycfg;


void halkeyInit(void)
{
    mkeyAssign(&keycfg,  halkeySwitchModeIsDown, MKEY_PRESS1_DOWN, MKEY_NULL, MKEY_NULL, 0, 0, 0);
    keytimeHandle = timerAssign(&keytimer, halkeyCB , (void *)&keytimeHandle);
    timerStart(keytimeHandle, HAL_KEY_SCAN_TIME);
}

void keyTask(void)
{
    if(mkeygetvalue() == MKEY_PRESS1_DOWN){
        key_log("swtich mode  key down!");
    }
}

static uint8_t halkeySwitchModeIsDown(void)
{
    return !(LL_GPIO_ReadInputPort(BX_HAL_KEY_SWITCH_MODE_PORT) & BX_HAL_KEY_SWITCH_MODE_PIN);
}

static void halkeyCB(void *arg)
{
    mkeydecetor_task();
    timerRestart(*((TimerHandle_t *)arg), HAL_KEY_SCAN_TIME);
}

