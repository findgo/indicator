
#include "bxleds.h"
#include "timers.h"

typedef struct ledcontrol_s{
    uint8_t     mode;
    uint8_t     todo;       /* 眨现次数，如果设置为0xff，表明无限闪烁*/
    uint8_t     onPct;      /* 占空比*/
    uint32_t    cycle;      /*开/关总周期 */    
    uint32_t    next;       /* 下一次改变的时间/与流逝时间相关 ，与时基绑定*/
}ledcontrol_t;

static uint16_t CurledsOnOffstatus;  // 保存当前灯的状态

#if configBLINK_LED > 0
static uint16_t PreledsBlinkstatus;  // 保存闪烁前的状态

static ledcontrol_t ledseqControl[bxLEDSNUM];
static TimerStatic_t ledtimer;
static TimerHandle_t ledhandle;
#endif
static void ledlowOnOff( uint16_t leds, uint8_t val );

#if configBLINK_LED > 0
static void bxLEDupdateCB(void *arg);
#endif

/**
  * @brief  初始化
  * @param  None
  * @note   
  * @retval  None
  */
void bxLedInit(void)
{
#if configBLINK_LED > 0
    ledhandle = timerAssign(&ledtimer, bxLEDupdateCB , (void *)&ledhandle);
    
    memset(ledseqControl, 0, sizeof(ledcontrol_t) * bxLEDSNUM );
 #endif   
    ledlowOnOff( BXLED_ALL, FALSE);// 所有灯关
}
/**
 * @brief   Tun ON/OFF/TOGGLE given LEDs
 *
 * @param   led - bit mask value of leds to be turned ON/OFF/TOGGLE
 *          mode - BLINK, FLASH, TOGGLE, ON, OFF
 * @return  None
 */

void bxLedset(uint16_t leds, BxLEDMode_t mode)
{
#if configBLINK_LED > 0

    uint16_t led;
    ledcontrol_t *sts;

    switch (mode) {
    case BXLED_MODE_BLINK:
        /* Default blink, 1 time, D% duty cycle */
        bxLedsetblink (leds, BXLED_BLINK_TODO, BXLED_BLINK_DUTY_CYCLE, BXLED_FLASH_CYCLE_TIME);
        break;

    case BXLED_MODE_FLASH:
        /* Default flash, continous , D% duty cycle */
        bxLedsetblink (leds, BXLED_BLINK_CONTINOUS_TODO, BXLED_BLINK_DUTY_CYCLE, BXLED_FLASH_CYCLE_TIME);
        break;

    case BXLED_MODE_OFF:
    case BXLED_MODE_ON:
    case BXLED_MODE_TOGGLE:

        led = BXLED_1;
        leds &= BXLED_ALL;
        sts = &ledseqControl[0];

        while (leds)
        {
            if (leds & led){
                if (mode != BXLED_MODE_TOGGLE) {
                    sts->mode = mode;  /* ON or OFF */
                }
                else{
                    sts->mode ^= BXLED_MODE_ON;  /* Toggle */
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
    uint16_t led;

    // on or off 
    if( mode < BXLED_MODE_TOGGLE ) {
        ledlowOnOff (leds, mode);
        return;
    }

    // only toggle
    led = BXLED_1;
    leds &= BXLED_ALL;
    
    while (leds)
    {
        if (leds & led){
            ledlowOnOff (led, ( CurledsOnOffstatus & led ) ? BXLED_MODE_OFF : BXLED_MODE_ON);
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

void bxLedsetblink(uint16_t leds, uint8_t numBlinks, uint8_t duty, uint32_t period)
{
#if configBLINK_LED > 0

    uint16_t led;
    ledcontrol_t *sts;

    if(leds == 0 || duty == 0 || period == 0){
        bxLEDset( leds, BXLED_MODE_OFF);
    }

    if(duty < 100) {
        led = BXLED_1;           //从第一个灯开始查
        leds &= BXLED_ALL;       //屏蔽不在驱动内的灯
        sts = &ledseqControl[0];
        while (leds)    //遍历找出想要设置的灯
        {
            if (leds & led) {
                if(sts->mode < BXLED_MODE_BLINK ){
                    // 保存闪烁之前的状态,闪烁完后恢复回去
                    PreledsBlinkstatus |= (led & CurledsOnOffstatus);
                }
                sts->mode = BXLED_MODE_OFF;
                sts->todo = numBlinks;
                sts->onPct = duty;
                sts->cycle = period;
                sts->next = mcu_getCurSysctime();
                if(numBlinks == 0) {
                    sts->mode |= BXLED_MODE_FLASH; // Continuous
                }
                sts->mode |= BXLED_MODE_BLINK;// blink it
                
                leds ^= led;//这个参数已设置完,清除对应位掩码
            }
            led <<= 1;  //下一个灯
            sts++;      //下一个灯
        }
        // blink it now
        timerStart(ledhandle, 0);
    }
    else {
        bxLEDset(leds, BXLED_MODE_ON);
    }
    
#else

    // LED is disabled, suppress unused argument warnings
    (void) leds;
    (void) numBlinks;
    (void) duty;
    (void) period;
    
#endif
}

uint16_t bxLedGetstatus( uint16_t which )
{
    return ( CurledsOnOffstatus & which );
}
#if configBLINK_LED > 0
static void bxLedupdateCB(void *arg)
{
    uint8_t pct;
    uint16_t led;
    uint16_t leds;
    ledcontrol_t *sts;
    uint32_t curtime;
    uint32_t wait;
    uint32_t next;
    
    led  = BXLED_1;
    leds = BXLED_ALL;
    sts = &ledseqControl[0];//前端初始化
    while (leds)//遍历所有灯
    {
        if (leds & led){
            if(sts->mode & BXLED_MODE_BLINK){
                curtime = mcu_getCurSysctime();
                if (curtime >= sts->next){
                    if (sts->mode & BXLED_MODE_ON) {
                        pct = 100 - sts->onPct;             /* Percentage of cycle for off */
                        sts->mode &= ~BXLED_MODE_ON;        /* Say it's not on */
                        ledlowOnOff (led, BXLED_MODE_OFF);  /* Turn it off */
                
                        if (!(sts->mode & BXLED_MODE_FLASH)){
                            sts->todo--;                        /* Not continuous, reduce count */
                        }
                  }            
                  else if ( (!sts->todo) && !(sts->mode & BXLED_MODE_FLASH) ) {
                        sts->mode ^= BXLED_MODE_BLINK;      /* No more blinks */  
                  }            
                  else{
                        pct = sts->onPct;                     /* Percentage of cycle for on */
                        sts->mode |= BXLED_MODE_ON;         /* Say it's on */
                        ledlowOnOff (led, BXLED_MODE_ON);   /* Turn it on */
                  }
                  
                  if (sts->mode & BXLED_MODE_BLINK) {
                        wait = (((uint32_t)pct * sts->cycle) / 100);
                        sts->next = curtime + wait;
                  }
                  else{
                        /* no more blink, no more wait */
                        wait = 0;
                        /* After blinking, set the LED back to the state before it blinks */
                        ledlowOnOff (led, ((PreledsBlinkstatus & led) != 0) ? BXLED_MODE_ON : BXLED_MODE_OFF);
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
  * @param  leds: bitmask ,detaial for BXLED_XX ,bool
  * @param  val: TURE: ON, FALSE: OFF
  * @note   
  * @note    
  * @note   
  * @retval 
  */
static void ledlowOnOff( uint16_t leds, uint8_t val )
{
#if (bxLEDSNUM >= 1)
    if (leds & BXLED_1){
        if(val){
            BXTURN_LED1_ON();
        }
        else{
            BXTURN_LED1_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 2)
    if (leds & BXLED_2){
        if(val){
            BXTURN_LED2_ON();
        }
        else{
            BXTURN_LED2_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 3) 
    if (leds & BXLED_3){
        if(val){
            BXTURN_LED3_ON();
        }
        else{
            BXTURN_LED3_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 4) 
    if (leds & BXLED_4){
        if(val){
            BXTURN_LED4_ON();
        }
        else{
            BXTURN_LED4_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 5) 
    if (leds & BXLED_5){
        if(val){
            BXTURN_LED5_ON();
        }
        else{
            BXTURN_LED5_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 6) 
    if (leds & BXLED_6){
        if(val){
            BXTURN_LED6_ON();
        }
        else{
            BXTURN_LED6_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 7) 
    if (leds & BXLED_7){
        if(val){
            BXTURN_LED7_ON();
        }
        else{
            BXTURN_LED7_OFF();
        }
    }
#endif

#if (bxLEDSNUM >= 8)
    if (leds & BXLED_8){
        if(val){
            BXTURN_LED8_ON();
        }
        else{
            BXTURN_LED8_OFF();
        }
   }
#endif

#if (bxLEDSNUM >= 9)
        if (leds & BXLED_9){
            if(val){
                BXTURN_LED9_ON();
            }
            else{
                BXTURN_LED9_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 10)
        if (leds & BXLED_10){
            if(val){
                BXTURN_LED10_ON();
            }
            else{
                BXTURN_LED10_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 11) 
        if (leds & BXLED_11){
            if(val){
                BXTURN_LED11_ON();
            }
            else{
                BXTURN_LED11_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 12) 
        if (leds & BXLED_12){
            if(val){
                BXTURN_LED12_ON();
            }
            else{
                BXTURN_LED12_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 13) 
        if (leds & BXLED_13){
            if(val){
                BXTURN_LED13_ON();
            }
            else{
                BXTURN_LED13_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 14) 
        if (leds & BXLED_14){
            if(val){
                BXTURN_LED14_ON();
            }
            else{
                BXTURN_LED14_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 15) 
        if (leds & BXLED_15){
            if(val){
                BXTURN_LED15_ON();
            }
            else{
                BXTURN_LED15_OFF();
            }
        }
#endif
    
#if (bxLEDSNUM >= 16)
        if (leds & BXLED_16){
            if(val){
                BXTURN_LED16_ON();
            }
            else{
                BXTURN_LED16_OFF();
            }
       }
#endif

    if(val){
        CurledsOnOffstatus |= leds;
    }
    else{
        CurledsOnOffstatus &= (leds ^ 0xFFFF);
    }
}



