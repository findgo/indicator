#include "port.h"
#include "mbrtu.h"
#include "modbus.h"

#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0
#include "app_cfg.h"

extern Mbshandle_t device0;

/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  定时器初始化函数
  * @param  None
  * @retval None
  */
uint8_t MbPortTimersInit(uint8_t port, uint16_t usTim1Timerout50us)
{
    (void)port;
    
    MX_TIM3_Init();
    LL_TIM_SetAutoReload(TIM3, usTim1Timerout50us);
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_TIM_DisableIT_UPDATE(TIM3);
    
    //定时器3失能
    LL_TIM_DisableCounter(TIM3);
    
    return TRUE;
}


void MbPortTimersEnable(uint8_t port)
{
    (void)port;
    
    /* Enable the timer with the timeout passed to MbPortTimersInit( ) */
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_TIM_EnableIT_UPDATE(TIM3); 
    LL_TIM_SetCounter(TIM3, 0x0000);
    //定时器4启动
    LL_TIM_EnableCounter(TIM3);
}

void MbPortTimersDisable(uint8_t port)
{
    (void)port;
    
    /* Disable any pending timers. */
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_TIM_DisableIT_UPDATE(TIM3);
    LL_TIM_SetCounter(TIM3, 0x0000);
    //关闭定时器4
    LL_TIM_DisableCounter(TIM3);
}
/**
  * @brief  定时器3中断服务函数
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    if (LL_TIM_IsActiveFlag_UPDATE(TIM3)){
        //清除定时器T4溢出中断标志位
        LL_TIM_ClearFlag_UPDATE(TIM3);
        MbsRTUTimerT35Expired(device0);
    }
}

//#if MB_MASTER_ENABLED > 0

//uint32_t sysclocktime = 0;

/*This optional function returns the current time in milliseconds (don't care
  for wraparound, this is only used for time diffs).*/
//uint32_t MbSys_now(void)
//{
//    return sysclocktime;
//}
//#endif

#endif

