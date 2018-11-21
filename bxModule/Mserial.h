/**
  ******************************************************************************
  * @file   Mserial.h
  * @author  
  * @version 
  * @date    
  * @brief      FIFO非阻塞串口通信，用于数据流传输
  ******************************************************************************
  * @attention      V1.2 jgb        201704023
  * @attention      V1.2.1 jgb        201808003  淇敼bug
  ******************************************************************************
  */

#ifndef __MOD_SERIAL_H_
#define __MOD_SERIAL_H_

#include "app_cfg.h"
#include <string.h>

#include "bxusart.h"


// 宏 控制支持的串口数量
#define COM_USE_NUM     2





//宏 用于控制非阻塞方式下，设置发送，接收最大缓冲区
#define COM0_RX_MAX_SIZE   512
#define COM0_TX_MAX_SIZE   255

#define COM1_RX_MAX_SIZE   128
#define COM1_TX_MAX_SIZE   128

#define COM2_RX_MAX_SIZE   1
#define COM2_TX_MAX_SIZE   255

#define COM3_RX_MAX_SIZE   255
#define COM3_TX_MAX_SIZE   255

#define COM0PutByte(dat)    (LL_USART_TransmitData8(USART_USING1, dat))
#define COM0GetByte()       ((uint8_t)LL_USART_ReceiveData8(USART_USING1))
#define COM0TxIEEnable()    do{ LL_USART_EnableIT_TXE(USART_USING1);}while(0)
#define COM0TxIEDisable()   do{ LL_USART_DisableIT_TXE(USART_USING1); }while(0)

#define COM1PutByte(dat)    (LL_USART_TransmitData8(USART_USING2, dat))
#define COM1GetByte()     	((uint8_t)LL_USART_ReceiveData8(USART_USING2))
#define COM1TxIEEnable()    do{ LL_USART_EnableIT_TXE(USART_USING2); }while(0)
#define COM1TxIEDisable()   do{ LL_USART_DisableIT_TXE(USART_USING2); }while(0)

#define COM2PutByte(dat)    //(USART_SendData(USART_USING3, dat)) 	
#define COM2GetByte()       //((uint8_t)USART_ReceiveData(USART_USING3))	
#define COM2TxIEEnable()    //do{ USART_ITConfig(USART_USING3, USART_IT_TXE, ENABLE); }while(0) 	
#define COM2TxIEDisable()   //do{ USART_ITConfig(USART_USING3, USART_IT_TXE, DISABLE); }while(0)	

#define COM3PutByte(dat)  	
#define COM3GetByte()     	
#define COM3TxIEEnable()    	
#define COM3TxIEDisable()   	

uint16_t Serial_WriteByte(uint8_t COM,uint8_t dat);
uint16_t Serial_WriteBuf(uint8_t COM,uint8_t *buf,uint16_t len);
uint16_t Serial_Read(uint8_t COM,uint8_t *buf,uint16_t len);
uint16_t SerialRxValidAvail(uint8_t COM);
#define Serial_WriteStr(COM,str) Serial_WriteBuf(COM,(uint8_t *)str,strlen(str))


#endif

