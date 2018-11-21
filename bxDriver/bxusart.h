#ifndef _USER_DIRVER_UART_H_
#define _USER_DIRVER_UART_H_

#include "app_cfg.h"

enum{
    COM0,
    COM1,
    COM2,
    COM3
};
    
typedef enum
{
    DRV_PAR_NONE,                /*!< No parity. */
    DRV_PAR_ODD,                 /*!< Odd parity. */
    DRV_PAR_EVEN                 /*!< Even parity. */
}DRV_Parity_t;

// for usart 1
#define USART_USING1   					USART1
#define USART_USING1_IRQ				USART1_IRQn

#define USART_USING1_TX_PORT			GPIOA
#define USART_USING1_TX_PIN				LL_GPIO_PIN_9

#define USART_USING1_RX_PORT			GPIOA
#define USART_USING1_RX_PIN				LL_GPIO_PIN_10

// for usart 2
#define USART_USING2   					USART2
#define USART_USING2_IRQ				USART2_IRQn

#define USART_USING2_TX_PORT			GPIOA
#define USART_USING2_TX_PIN				LL_GPIO_PIN_2

#define USART_USING2_RX_PORT			GPIOA
#define USART_USING2_RX_PIN				LL_GPIO_PIN_3

// for usart 3
//#define USART_USING3   					USART3
//#define USART_USING3_IRQ				USART3_IRQn

//#define USART_USING3_TX_PORT			GPIOB
//#define USART_USING3_TX_PIN				GPIO_Pin_10

//#define USART_USING3_RX_PORT			GPIOB
//#define USART_USING3_RX_PIN				GPIO_Pin_11

uint8_t SerialDrvInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, DRV_Parity_t eParity);
uint8_t SerialDrvPutByte(uint8_t port, char ucByte );
uint8_t SerialDrvGetByte(uint8_t port, char *pucByte );

void SerialDrvEnable(uint8_t port, uint8_t xRxEnable, uint8_t xTxEnalbe);


#endif

