
#include "loop.h"

//for driver
//#include "mleds.h"
//#include "systick.h"
//#include "mserial.h"
//#include "memalloc.h"
//#include "timers.h"
//#include "event_groups.h"
//#include "log.h"

//#include "nv.h"

//#include "hmi.h"

//#include "SHT.h"

#if LOOP_TASKS_EVENT_ENABLE > 0
// 事件触发,为未来低功耗节能
static const pTaskFn_t tasksArr[] =
{
};
static const uint8_t tasksCnt = sizeof(tasksArr) / sizeof(tasksArr[0]);
static uint16_t *tasksEvents;
#endif
void tasks_init_System(void)
{
#if LOOP_TASKS_EVENT_ENABLE > 0
    uint8_t taskID = 0;
    
    tasksEvents = (uint16_t *)mo_malloc( sizeof( uint16_t ) * tasksCnt);
    memset( tasksEvents, 0, (sizeof( uint16_t ) * tasksCnt));
#endif

    
//    Systick_Configuration();
//    log_Init();
//    
//    log_infoln("loop_init_System init begin");
//    hmiInit();
//    //nvinit();

//    delay_ms(200);
//    //halledInit();
//    //mledInit();
//    bsp_InitSHT();

//    //mledset(MLED_1, MLED_MODE_FLASH);
//    log_infoln("loop_init_System init end, and then start system");
}
void tasksPoll(void)
{
//    timerTask();
//    UG_Update();
//    SHT_PeriodicHandle();
}
