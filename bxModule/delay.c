#include "delay.h"

//按48MHZ算，HUNS 0.1/(1/48) ~= 5
#define DELAY_HUN_NS_TICK	5	
#define DELAY_ONE_US_TICK	48




/**
  * @brief	100ns延时
  * @param	None
  * @note		
  * @retval  None
  */	
void delay_hunns(uint32_t hunns)
{
	uint8_t i;
	
	while(hunns--)
	{
		for(i = 0; i < DELAY_HUN_NS_TICK; i++);
	}
}

/**
  * @brief	nus
  * @param	None
  * @note		
  * @retval  None
  */		    								   
void delay_us(uint32_t nus)
{
	uint16_t i;
	
	while(nus--)
	{
		for(i = 0; i < DELAY_ONE_US_TICK; i++);
	}
}
/**
  * @brief	nms
  * @param	None
  * @note		
  * @retval  None
  */	
void delay_ms(uint32_t nms)
{
	while(nms--)
	{
		delay_us(1000);
	}
}



































