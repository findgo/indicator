
#include "loop.h"
#include "app_cfg.h"

//for module
#include "task.h"
#include "log.h"

//for driver
#include "systick.h"

//#include "memalloc.h"
#include "timers.h"
//#include "event_groups.h"

//#include "nv.h"
#include "bxLeds.h"
#include "mcoils.h"


#include "hmi.h"
#include "bxmb.h"
#include "hal_adc.h"
#include "hal_key.h"

extern void SystemClock_Config(void);



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
    Systick_Configuration();
    log_Init();

    log_alertln("app init!");
//    nvinit(); // 待定

    bxLedInit();  // 初始化灯控制器        test  ok
    mCoilsInit(); // 初始化继电器控制器  tes  t       ok
    halkeyInit();  // 初始化按键      test ok
    haladcInit();
//    hmiInit();
//    haladcInit();
    bxmbInit(); // 初始化modbus ok


// test 
    bxLedset(BXLED_ALL, BXLED_MODE_ON); 
    mCoilsSet(MCOILS_ALL, MCOILS_MODE_ON);

    log_alertln("App start!");
}
void tasksPoll(void)
{
    timerTask();
//    UG_Update();
    MbsPoll();
    keyTask();
}



int main(void)
{
  // for stm32cubeMX init must be init first
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* System interrupt init*/

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
  SystemCoreClockUpdate();
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_DMA_Init();
//  MX_USART2_UART_Init();
//  MX_ADC_Init();
//  MX_USART1_UART_Init();
//  MX_SPI2_Init();
//  MX_WWDG_Init();
 // MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  tasks_init_System();
  /* USER CODE END 2 */
  
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    tasks_Run_System();
  }
  /* USER CODE END 3 */
}

