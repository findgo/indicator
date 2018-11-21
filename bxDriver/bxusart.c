/**
  ******************************************************************************
  * @file    usart.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "bxusart.h"

/**
  * @brief  init the serial
  * @param  port      which port
  *         ulBaudRate  
  *         ucDataBits  
  *         eParity      
  * @retval TRUE: success FALSE: failed
  */
uint8_t SerialDrvInit(uint8_t port, uint32_t ulBaudRate, uint8_t ucDataBits, DRV_Parity_t eParity)
{
    // see stm32CubeMX
    return Success;
}

/**
  * @brief  send a byte to usart
  * @param  None
  * @retval TRUE: success FALSE: failed
  */
uint8_t SerialDrvPutByte(uint8_t port, char ucByte )
{
    switch (port){
    case COM0:
        LL_USART_TransmitData8(USART1, ucByte);
        break;
    case COM1:
        LL_USART_TransmitData8(USART2, ucByte);
        break;
//    case COM2:
//        LL_USART_TransmitData8(USART3, ucByte);
//        break;
    default:
        return Failed;
    }
  
  return Success;
}

/**
  * @brief  get a byte from usart
  * @param  None
  * @retval TRUE: success FALSE: failed
  */
uint8_t SerialDrvGetByte(uint8_t port, char *pucByte )
{
    switch (port){
    case COM0:    
        *pucByte = LL_USART_ReceiveData8(USART1);
        break;
    case COM1:
        *pucByte = LL_USART_ReceiveData8(USART2);
        break;
//    case COM2:
//        *pucByte = LL_USART_ReceiveData8(USART3);
//        break;
    default:
        return Failed;
    }
    
    return Success;
}

/**
  * @brief  enable Rs485 RTS pin only for 485
  * @param  None
  * @retval None
  */
void SerialDrvEnable(uint8_t port, uint8_t xRxEnable, uint8_t xTxEnable)
{
    switch(port){
    case COM0:
        if(xRxEnable){
            LL_USART_EnableIT_RXNE(USART1);
            //MAX485 rts ping disable enable receive 
            LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_8);
        }
        else{
            LL_USART_DisableIT_RXNE(USART1); 
            //MAX485 rts ping enable enable send 
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_8);
        }

        if(xTxEnable){
            LL_USART_EnableIT_TC(USART1);
        }
        else{
            LL_USART_DisableIT_TC(USART1);
        }
        break;
        
     case COM1:
        if(xRxEnable){
            LL_USART_EnableIT_RXNE(USART2);
            //MAX485 rts ping disable enable receive 
            LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_9);
        }
        else{
            LL_USART_DisableIT_RXNE(USART2); 
            //MAX485 rts pin enable enable send 
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_9);
        }

        if(xTxEnable){
            LL_USART_EnableIT_TC(USART2);
        }
        else{
            LL_USART_DisableIT_TC(USART2);
        }
        break;
 /*       
     case COM2:
        if(xRxEnable){
            LL_USART_EnableIT_RXNE(USART3);
            //MAX485 rts ping disable enable receive 
            LL_GPIO_ResetOutputPin(GPIOD, LL_GPIO_PIN_9);
        }
        else{
            LL_USART_DisableIT_RXNE(USART3); 
            //MAX485 rts pin enable enable send 
            LL_GPIO_SetOutputPin(GPIOD, LL_GPIO_PIN_9);
        }

        if(xTxEnable){
            LL_USART_EnableIT_TC(USART3);
        }
        else{
            LL_USART_DisableIT_TC(USART3);
        }

        break;    
 */       
     default:
        break;
    }
    
}

