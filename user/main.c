

#include "app_cfg.h"


//for extern module
#include "eeprom.h"
//for driver
#include "systick.h"
#include "hal_led.h"
#include "hal_pwm.h"
#include "usart.h"
#include "rf.h"

uint16_t VirtAddVarTab[NB_OF_VAR];


int main(void)
{
    SystemCoreClockUpdate();
    Systick_Configuration();
    Usart_Configuration();
    halledInit();
    
    while(1)
    {   

    }

    
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

