
#include "mleds.h"
#include "timers.h"

typedef struct ledcontrol_s{
    uint8_t     mode;
    uint8_t     todo;       /* 眨现次数，如果设置为0xff，表明无限闪烁*/
    uint8_t     onPct;      /* 占空比*/
    uint32_t    cycle;      /*开/关总周期 */    
    uint32_t    next;       /* 下一次改变的时间/与流逝时间相关 ，与时基绑定*/
}ledcontrol_t;

static uint8_t CurledsOnOffstatus;  // 保存当前灯的状态

#if configBLINK_LED > 0
static uint8_t PreledsBlinkstatus;  // 保存闪烁前的状态

static ledcontrol_t ledseqControl[mLEDSNUM];
static TimerStatic_t ledtimer;
static TimerHandle_t ledhandle;
#endif
static void ledlowOnOff( uint8_t leds, uint8_t val );

#if configBLINK_LED > 0
static void mledupdateCB(void *arg);
#endif

/**
  * @brief  初始化
  * @param  None
  * @note   
  * @retval  None
  */
void mledInit(void)
{
#if configBLINK_LED > 0
    ledhandle = timerAssign(&ledtimer, mledupdateCB , (void *)&ledhandle);
    
    memset(ledseqControl, 0, sizeof(ledcontrol_t) * mLEDSNUM );
 #endif   
    ledlowOnOff( MLED_ALL, FALSE);// 所有灯关
}
/**
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - BLINK, FLASH, TOGGLE, ON, OFF
 * @return  None
 */

void mledset(uint8_t leds, MledMode_t mode)
{
#if configBLINK_LED > 0

    uint8_t led;
    ledcontrol_t *sts;

    switch (mode) {
    case MLED_MODE_BLINK:
        /* Default blink, 1 time, D% duty cycle */
        mledsetblink (leds, MLED_BLINK_TODO, MLED_BLINK_DUTY_CYCLE, MLED_FLASH_CYCLE_TIME);
        break;

    case MLED_MODE_FLASH:
        /* Default flash, continous , D% duty cycle */
        mledsetblink (leds, MLED_BLINK_CONTINOUS_TODO, MLED_BLINK_DUTY_CYCLE, MLED_FLASH_CYCLE_TIME);
        break;

    case MLED_MODE_OFF:
    case MLED_MODE_ON:
    case MLED_MODE_TOGGLE:

        led = MLED_1;
        leds &= MLED_ALL;
        sts = &ledseqControl[0];

        while (leds)
        {
            if (leds & led){
                if (mode != MLED_MODE_TOGGLE) {
                    sts->mode = mode;  /* ON or OFF */
                }
                else{
                    sts->mode ^= MLED_MODE_ON;  /* Toggle */
                }
                ledlowOnOff (led, sts->mode);
                leds ^= led;
            }
            led <<= 1;
            sts++;
        }
        break;

    default:
        break;
    }

#else
    uint8_t led;

    // no blink, do nothing
    if( mode > MLED_MODE_TOGGLE)
        return;

    // on or off 
    if( mode < MLED_MODE_TOGGLE ) {
        ledlowOnOff (leds, mode);
        return;
    }

    // only toggle
    led = MLED_1;
    leds &= MLED_ALL;
    
    while (leds)
    {
        if (leds & led){
            ledlowOnOff (led, ( CurledsOnOffstatus & led ) ? MLED_MODE_OFF : MLED_MODE_ON);
            leds ^= led;
        }
        led <<= 1;
    }
    
#endif
}

/**
  * @brief  
  * @param  leds : whick led
  * @param  numBlinks count you want to blink
  * @param  duty: on duty
  * @param  period:
  * @note      
  * @retval  None
  */

void mledsetblink(uint8_t leds, uint8_t numBlinks, uint8_t duty, uint32_t period)
{
#if configBLINK_LED > 0

    uint8_t led;
    ledcontrol_t *sts;

    if(leds == 0 || duty == 0 || period == 0){
        mledset( leds, MLED_MODE_OFF);
    }

    if(duty < 100) {
        led = MLED_1;           //从第一个灯开始查
        leds &= MLED_ALL;       //屏蔽不在驱动内的灯
        sts = &ledseqControl[0];
        while (leds)    //遍历找出想要设置的灯
        {
            if (leds & led) {
                if(sts->mode < MLED_MODE_BLINK ){
                    // 保存闪烁之前的状态,闪烁完后恢复回去
                    PreledsBlinkstatus |= (led & CurledsOnOffstatus);
                }
                sts->mode = MLED_MODE_OFF;
                sts->todo = numBlinks;
                sts->onPct = duty;
                sts->cycle = period;
                sts->next = mcu_getCurSysctime();
                if(numBlinks == 0) {
                    sts->mode |= MLED_MODE_FLASH; // Continuous
                }
                sts->mode |= MLED_MODE_BLINK;// blink it
                
                leds ^= led;//这个参数已设置完,清除对应位掩码
            }
            led <<= 1;  //下一个灯
            sts++;      //下一个灯
        }
        // blink it now
        timerStart(ledhandle, 0);
    }
    else {
        mledset(leds, MLED_MODE_ON);
    }
    
#else

    // LED is disabled, suppress unused argument warnings
    (void) leds;
    (void) numBlinks;
    (void) duty;
    (void) period;
    
#endif
}

uint8_t mledGetstatus( uint8_t which )
{
    return ( CurledsOnOffstatus & which );
}
#if configBLINK_LED > 0
static void mledupdateCB(void *arg)
{
    uint8_t pct;
    uint8_t led;
    uint8_t leds;
    ledcontrol_t *sts;
    uint32_t curtime;
    uint32_t wait;
    uint32_t next;
    
    led  = MLED_1;
    leds = MLED_ALL;
    sts = &ledseqControl[0];//前端初始化
    while (leds)//遍历所有灯
    {
        if (leds & led){
            if(sts->mode & MLED_MODE_BLINK){
                curtime = mcu_getCurSysctime();
                if (curtime >= sts->next){
                    if (sts->mode & MLED_MODE_ON) {
                        pct = 100 - sts->onPct;             /* Percentage of cycle for off */
                        sts->mode &= ~MLED_MODE_ON;        /* Say it's not on */
                        ledlowOnOff (led, MLED_MODE_OFF);  /* Turn it off */
                
                        if (!(sts->mode & MLED_MODE_FLASH)){
                            sts->todo--;                        /* Not continuous, reduce count */
                        }
                  }            
                  else if ( (!sts->todo) && !(sts->mode & MLED_MODE_FLASH) ) {
                        sts->mode ^= MLED_MODE_BLINK;      /* No more blinks */  
                  }            
                  else{
                        pct = sts->onPct;                     /* Percentage of cycle for on */
                        sts->mode |= MLED_MODE_ON;         /* Say it's on */
                        ledlowOnOff (led, MLED_MODE_ON);   /* Turn it on */
                  }
                  
                  if (sts->mode & MLED_MODE_BLINK) {
                        wait = (((uint32_t)pct * sts->cycle) / 100);
                        sts->next = curtime + wait;
                  }
                  else{
                        /* no more blink, no more wait */
                        wait = 0;
                        /* After blinking, set the LED back to the state before it blinks */
                        ledlowOnOff (led, ((PreledsBlinkstatus & led) != 0) ? MLED_MODE_ON : MLED_MODE_OFF);
                        /* Clear the saved bit */
                        PreledsBlinkstatus &= (led ^ 0xFF);
                  }
                }
                else{
                    wait = sts->next - curtime;  /* Time left */
                }
                if(!next || ( wait && (wait < next))){
                    next = wait;
                }
            }
            leds ^= led;//已处理完，对应位清零
        }
        led <<= 1;  //下一个灯
        sts++;      //下一个灯
    }

    if(next){
        timerRestart(*((TimerHandle_t *)arg), next);
    }
}
#endif
/**
  * @brief  leds开关控制 抽象低层
  * @param  leds: bitmask ,detaial for MLED_XX ,bool
  * @param  val: TURE: ON, FALSE: OFF
  * @note   
  * @note    
  * @note   
  * @retval 
  */
static void ledlowOnOff( uint8_t leds, uint8_t val )
{
#if (mLEDSNUM >= 1)
    if (leds & MLED_1){
        val ? MTURN_LED1_ON() : MTURN_LED1_OFF();
    }
#endif

#if (mLEDSNUM >= 2)
    if (leds & MLED_2){
        val ? MTURN_LED2_ON() : MTURN_LED2_OFF();
    }
#endif

#if (mLEDSNUM >= 3) 
    if (leds & MLED_3){
        val ? MTURN_LED3_ON() : MTURN_LED3_OFF();
    }
#endif

#if (mLEDSNUM >= 4) 
    if (leds & MLED_4){
        val ? MTURN_LED4_ON() : MTURN_LED4_OFF();
    }
#endif

#if (mLEDSNUM >= 5) 
    if (leds & MLED_5){
        val ? MTURN_LED5_ON() : MTURN_LED5_OFF();
    }
#endif

#if (mLEDSNUM >= 6) 
    if (leds & MLED_6){
        val ? MTURN_LED6_ON() : MTURN_LED6_OFF();
    }
#endif

#if (mLEDSNUM >= 7) 
    if (leds & MLED_7){
        val ? MTURN_LED7_ON() : MTURN_LED7_OFF();
    }
#endif

#if (mLEDSNUM >= 8)
    if (leds & MLED_8){
        val ? MTURN_LED8_ON() : MTURN_LED8_OFF();
   }
#endif

    if(val){
        CurledsOnOffstatus |= leds;
    }
    else{
        CurledsOnOffstatus &= (leds ^ 0xFF);
    }
}


