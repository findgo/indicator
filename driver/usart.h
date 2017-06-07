#ifndef _USER_DIRVER_UART_H_
#define _USER_DIRVER_UART_H_

#include "app_cfg.h"


#define USART_USING1   					USART1
#define USART_USING1_IRQ				USART1_IRQn	
#define USART_USING1_TX_PORT			GPIOB
#define USART_USING1_TX_PIN				GPIO_Pin_6
#define USART_USING1_TX_AF_PINSOURCE	GPIO_PinSource6
#define USART_USING1_TX_AF_SEL			GPIO_AF_0

#define USART_USING1_RX_PORT			GPIOB
#define USART_USING1_RX_PIN				GPIO_Pin_7
#define USART_USING1_RX_AF_PINSOURCE	GPIO_PinSource7
#define USART_USING1_RX_AF_SEL			GPIO_AF_0

#define USART_USING1_GPIO_PeriphClock_EN() do{ RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE); }while(0)
#define USART_USING1_PeriphClock_EN()	do{ RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);}while(0)


void Usart_Configuration(void);

#endif

