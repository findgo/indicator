
#include "port.h"
#include "mbrtu.h"
#include "modbus.h"

#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0
#include "app_cfg.h"
#include "bxusart.h"

#define MBCOM0_RXEN()  //GPIO_ResetBits(GPIOD,GPIO_Pin_8)
#define MBCOM0_RXDIS() //GPIO_SetBits(GPIOD,GPIO_Pin_8)

extern Mbshandle_t device0;


/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void MbPortSerialEnable(uint8_t port, uint8_t xRxEnable, uint8_t xTxEnable)
{
    (void)port;
    
    if(xRxEnable){
        //使能接收和接收中断
        LL_USART_EnableIT_RXNE(USART2);
        //MAX485操作 低电平为接收模式
        MBCOM0_RXEN();
    }
    else{
        LL_USART_DisableIT_RXNE(USART2); 
        //MAX485操作 高电平为发送模式
        MBCOM0_RXDIS();
    }

    if(xTxEnable){
        //使能发送完成中断
        LL_USART_EnableIT_TC(USART2);
    }
    else{
        //禁止发送完成中断
        LL_USART_DisableIT_TC(USART2);
    }        
}

/**
  * @brief  串口初始化
  * @param  ucPORT      串口号
  *         ulBaudRate  波特率
  *         ucDataBits  数据位
  *         eParity     校验位 
  * @retval None
  */
uint8_t MbPortSerialInit(uint8_t ucPORT, uint32_t ulBaudRate, uint8_t ucDataBits, MbParity_t eParity)
{
    (void) ucPORT;
    
    return SerialDrvInit(COM1, ulBaudRate, ucDataBits, (DRV_Parity_t) eParity);
}

/**
  * @brief  通过串口发送数据
  * @param  None
  * @retval None
  */
uint8_t MbPortSerialPutByte(uint8_t port, char ucByte )
{
    (void)port;
    
    LL_USART_TransmitData8(USART2, ucByte);
    
    return TRUE;
}

/**
  * @brief  从串口获得数据
  * @param  None
  * @retval None
  */
uint8_t MbPortSerialGetByte(uint8_t port, char *pucByte )
{
    (void)port;
    
    *pucByte = LL_USART_ReceiveData8(USART2);
    
    return TRUE;
}

/**
  * @brief  USART2中断服务函数
  * @param  None
  * @retval None
  */
void USART2_IRQHandler(void)
{
  //发生接收中断

    
    if(LL_USART_IsActiveFlag_RXNE(USART2)){
        MbsRTUReceiveFSM(device0);
        //清除中断标志位    
    }
  
  //发生完成中断
    if( LL_USART_IsActiveFlag_TC(USART2) ){
        MbsRTUTransmitFSM(device0);
        LL_USART_ClearFlag_TC(USART2);
    }
  
  //测试看是否可以去除 2012-07-23
  //溢出-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
  /*
  if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
  {
    USART_ClearFlag(USART2,USART_FLAG_ORE); //读SR
    USART_ReceiveData(USART2);              //读DR
  }
  */
    NVIC_ClearPendingIRQ(USART2_IRQn);
}

#endif

