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
// for usart 2
#define USART_USING2   					USART2
// for usart 3
#define USART_USING3   					USART3

uint8_t SerialDrvInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, DRV_Parity_t eParity);
uint8_t SerialDrvPutByte(uint8_t port, char ucByte );
uint8_t SerialDrvGetByte(uint8_t port, char *pucByte );

void SerialDrvEnable(uint8_t port, uint8_t xRxEnable, uint8_t xTxEnalbe);


#endif

