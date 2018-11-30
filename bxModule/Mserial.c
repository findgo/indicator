
 
#include "mserial.h"


typedef struct {
    uint8_t *ptxbuf;
    uint16_t txHead;    //·¢ËÍ»º³åÇøÍ·
    uint16_t txTail;    //·¢ËÍ»º³åÇøÎ²
    uint16_t txcount;   //·¢ËÍ»º³åÓÐÐ§¸öÊý
    uint16_t txsize;    //·¢ËÍ»º³åÇø´óÐ¡

    uint8_t *prxbuf;
    uint16_t rxHead;    //½ÓÊÕ»º³åÇøÍ·
    uint16_t rxTail;    //½ÓÊÕ»º³åÇøÎ²
    uint16_t rxcount;   //½ÓÊÕ»º³åÓÐÐ§¸öÊý
    uint16_t rxsize;    //½ÓÊÕ»º³åÇø´óÐ¡
}comcfg_t;

//·¢ËÍ»º³åÇø¿ÕÏÐ×Ö½ÚÊý
#define SERIAL_TX_IDLE_AVAIL(ptr)   (ptr->txsize - ptr->txcount)
//·¢ËÍ»º³åÇøÓÐÐ§½ÚÊý
#define SERIAL_TX_VALID_AVAIL(ptr)  (ptr->txcount)
//·¢ËÍ»º³åÇøÊÇ·ñÓÐ¿É¶ÁÊý¾Ý
#define IS_SERIAL_TX_VALID(ptr) (ptr->txcount > 0)

//½ÓÊÕ»º³åÇø¿ÕÏÐ×Ö½ÚÊý
#define SERIAL_RX_IDLE_AVAIL(ptr)   (ptr->rxsize - ptr->rxcount)
//½ÓÊÕ»º³åÇøÓÐÐ§×Ö½ÚÊý   
#define SERIAL_RX_VALID_AVAIL(ptr)  (ptr->rxcount)
//½ÓÊÕ»º³åÇøÊÇ·ñÓÐ¿É¶ÁÊý¾Ý
#define IS_SERIAL_RX_VALID(ptr) (ptr->rxcount > 0)

//private 
#if COM_USE_NUM >= 1
static uint8_t Com0TxBuf[COM0_TX_MAX_SIZE]; //·¢ËÍ»º³åÇø
static uint8_t Com0RxBuf[COM0_RX_MAX_SIZE]; //½ÓÊÕ»º³åÇø
static comcfg_t comcfg0 = {&Com0TxBuf[0], 0, 0, 0, COM0_TX_MAX_SIZE, &Com0RxBuf[0], 0, 0, 0, COM0_RX_MAX_SIZE}; // serial¶ÔÏóÊµÀý
#endif
#if COM_USE_NUM >= 2
static uint8_t Com1TxBuf[COM1_TX_MAX_SIZE]; //·¢ËÍ»º³åÇø
static uint8_t Com1RxBuf[COM1_RX_MAX_SIZE]; //½ÓÊÕ»º³åÇø
static comcfg_t comcfg1 = {&Com1TxBuf[0], 0, 0, 0, COM1_TX_MAX_SIZE, &Com1RxBuf[0], 0, 0, 0, COM1_RX_MAX_SIZE}; // serial¶ÔÏóÊµÀý
#endif
#if COM_USE_NUM >= 3
static uint8_t Com2TxBuf[COM2_TX_MAX_SIZE]; //·¢ËÍ»º³åÇø
static uint8_t Com2RxBuf[COM2_RX_MAX_SIZE]; //½ÓÊÕ»º³åÇø
static comcfg_t comcfg2 = {&Com2TxBuf[0], 0, 0, 0, COM2_TX_MAX_SIZE, &Com2RxBuf[0], 0, 0, 0, COM2_RX_MAX_SIZE}; // serial¶ÔÏóÊµÀý
#endif
#if COM_USE_NUM >= 4
static uint8_t Com3TxBuf[COM3_TX_MAX_SIZE]; //·¢ËÍ»º³åÇø
static uint8_t Com3RxBuf[COM3_RX_MAX_SIZE]; //½ÓÊÕ»º³åÇø
static comcfg_t comcfg3 = {&Com3TxBuf[0], 0, 0, 0, COM3_TX_MAX_SIZE, &Com3RxBuf[0], 0, 0, 0, COM3_RX_MAX_SIZE}; // serial¶ÔÏóÊµÀý
#endif




static comcfg_t *GetUseCom(uint8_t COM)
{
    switch(COM){
#if COM_USE_NUM >= 1
    case COM0:
        return &comcfg0;
#endif
#if COM_USE_NUM >= 2
    case COM1:
        return &comcfg1;
#endif
#if COM_USE_NUM >= 3
    case COM2:
        return &comcfg2;
#endif
#if COM_USE_NUM >= 4
    case COM3:
        return &comcfg3;
#endif
    default:
        return &comcfg0;
    }
}



/**
  * @brief  Ïò·¢ËÍ»º³åÇø´æ¶à¸öÊý¾Ý
  * @param  buf: Òª´æÈë»º³åÇøµÄÊý¾Ý Len: ´æÈë»º³åÇøµÄÊý¾Ý³¤¶È 
  * @note   
  * @retval number successful put into buf
  */
static uint16_t SerialTxPut( comcfg_t *cfg, uint8_t *buf, uint16_t len )
{
    uint16_t availcount;

    len = MIN( SERIAL_TX_IDLE_AVAIL(cfg), len );    
    availcount = len;
    
    while(availcount){
        cfg->ptxbuf[cfg->txTail] = *buf++;
        cfg->txcount++;
        if (++cfg->txTail >= cfg->txsize){
            cfg->txTail = 0;
        } 
        availcount--;
    }

    return len;
}
/**
  * @brief  ·¢ËÍ»º³åÇøÈ¡³öÒ»¸ö×Ö½ÚÊý¾Ý
  * @param  dat: ÒªÈ¡»º³åÇøµÄÊý¾ÝµØÖ·
  * @note   
  * @retval ·µ»ØÈ¡³öµÄ×Ö½ÚÊý
  * @note       ÖÐ¶Ï·¢ËÍÈ¡Êý¾ÝÊ±ÐèÒªµ÷ÓÃ
  */
uint16_t SerialTxPop( comcfg_t *cfg, uint8_t *dat )
{
    if(IS_SERIAL_TX_VALID(cfg)){//æ˜¯å¦æœ‰æœ‰æ•ˆæ•°æ®
        *dat = cfg->ptxbuf[cfg->txHead];
        cfg->txcount--;
        if(++cfg->txHead >= cfg->txsize){
            cfg->txHead = 0;
        }
        return 1; 
    }

    return 0;
}
/**
  * @brief  ¶Á·¢ËÍ»º³åÇøÓÐÐ§×Ö½ÚÊý
  * @param  
  * @note   
  * @retval ·µ»Ø·¢ËÍ»º³åÇøÓÐÐ§×Ö½ÚÊý
  * @note       
  */
uint16_t SerialTxValidAvail( uint8_t COM )
{
    comcfg_t *cfg;
    
    cfg = GetUseCom(COM);
    return SERIAL_TX_VALID_AVAIL(cfg);
}

/**
  * @brief  Ïò½ÓÊÕ»º³åÇø´æÈëÒ»¸öÊý¾Ý
  * @param  dat:´æÈëµÄÊý¾Ý
  * @note   
  * @retval     ture or false
  * @note       ÖÐ¶Ï½ÓÊÕ´æÊý¾ÝÊ±ÖÐ¶Ïµ÷ÓÃ
  */
bool SerialRxPut( comcfg_t *cfg, uint8_t dat )
{
    if(SERIAL_RX_IDLE_AVAIL(cfg) > 0){
        cfg->prxbuf[cfg->rxTail] = dat;
        cfg->rxcount++;
        if(++cfg->rxTail >= cfg->rxsize){
            cfg->rxTail = 0;
        } 
        return true;
    }

    return false;
}

/**
  * @brief  ´Ó½ÓÊÕ»º³åÇøÈ¡³ölen¸öÊý¾Ý
  * @param  buf:È¡³öÊý¾ÝµÄ»º³åÇø,len: Êý¾Ý³¤¶È
  * @note   
  * @retval  ·µ»ØÈ¡³öÊý¾ÝµÄÊµ¼Ê¸öÊý
  */
static uint16_t SerialRxPop( comcfg_t *cfg, uint8_t *buf, uint16_t len )
{
    uint16_t cnt = 0;
    
    while(IS_SERIAL_RX_VALID(cfg) && (cnt < len))//½ÓÊÕ»º´æÇøÓÐÊý¾Ý
    {
        *buf++ = cfg->prxbuf[cfg->rxHead];//´Ó½ÓÊÕ»º³åÇøÌáÈ¡Êý¾Ý
        cfg->rxcount--;
        if(++cfg->rxHead >= cfg->rxsize){
            cfg->rxHead = 0;
        }
        cnt++;
    }

    return cnt;
}
/**
  * @brief  ´Ó½ÓÊÕ»º³åÇøÈ¡³ölen¸öÊý¾Ý
  * @param  buf:È¡³öÊý¾ÝµÄ»º³åÇø,len: Êý¾Ý³¤¶È
  * @note   
  * @retval  ·µ»ØÈ¡³öÊý¾ÝµÄÊµ¼Ê¸öÊý
  */
uint16_t SerialRxValidAvail( uint8_t COM )
{
    comcfg_t *cfg;
    
    cfg = GetUseCom(COM);
    return SERIAL_RX_VALID_AVAIL(cfg);
}

//Æô¶¯Ò»¸ö·¢ËÍ¿ÕÖÐ¶Ï
static void Start_TXEtransmit( uint8_t COM, comcfg_t *cfg )
{
    //²ÉÓÃ·¢ËÍÍê³É¿ÕÖÐ¶Ï
    if(IS_SERIAL_TX_VALID(cfg)){//ÓÐÊý¾Ý
        switch(COM){
#if COM_USE_NUM >= 1
            case COM0:
                COM0TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 2
            case COM1:
                COM1TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 3
            case COM2:
                COM2TxIEEnable();
                break;
#endif
#if COM_USE_NUM >= 4
            case COM3:
                COM3TxIEEnable();
                break;
#endif
            default:
                COM0TxIEEnable();
                break;
        }
    }
}
//Æô¶¯Ò»¸ö·¢ËÍÍê³ÉÖÐ¶Ï
static void Start_TXCtransmit(uint8_t COM,comcfg_t *cfg)
{
    uint8_t temp;
        
    if(SerialTxPop(cfg,&temp)){
         switch(COM){
#if COM_USE_NUM >= 1
            case COM0:
                COM0PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 2
            case COM1:
                COM1PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 3
            case COM2:
                COM2PutByte(temp);
                break;
#endif
#if COM_USE_NUM >= 4
            case COM3:
                COM3PutByte(temp);
                break;
#endif
            default:
                COM0PutByte(temp);
                break;
        }
    }
   
}
/**
  * @brief  TXÊä³ö³¤¶ÈÎªlenµÄÊý¾Ý
  * @param  buf:Êä³öÊý¾Ý»º³åÇø,len:Êä³öÊý¾Ý³¤¶È
  * @note   
  * @retval  number write
  */
uint16_t Serial_WriteBuf(uint8_t COM,uint8_t *buf,uint16_t len)
{
    uint16_t count;
    comcfg_t *cfg;

    cfg = GetUseCom(COM);
    taskENTER_CRITICAL();
    count = SerialTxPut(cfg,buf, len);
        //²ÉÓÃ·¢ËÍÍê³ÉÖÐ¶Ï has some bug
//    Start_TXCtransmit(COM,cfg);

    //²ÉÓÃ·¢ËÍÍê³É¿ÕÖÐ¶Ï
    Start_TXEtransmit(COM,cfg);
    taskEXIT_CRITICAL();
    
    return count;
}



uint16_t Serial_WriteByte( uint8_t COM, uint8_t dat )
{
    uint16_t count;
    comcfg_t *cfg;

    cfg = GetUseCom(COM);
    taskENTER_CRITICAL();
    count = SerialTxPut( cfg, &dat, 1 );
    
    //²ÉÓÃ·¢ËÍÍê³ÉÖÐ¶Ï has some bug
//    Start_TXCtransmit(COM,cfg);
    Start_TXEtransmit(COM,cfg);

    taskEXIT_CRITICAL();

    return count;
}

/**
  * @brief  ´Ó½ÓÊÕÈ¡³ölen¸öÊý¾Ý
  * @param  buf:¶ÁÈëÊý¾Ý»º³åÇø,len:¶ÁÈëÊý¾Ý³¤¶È
  * @note   
  * @retval  ·µ»ØÈ¡µ½Êý¾ÝµÄÊµ¼Ê¸öÊý
  */
uint16_t Serial_Read( uint8_t COM, uint8_t *buf, uint16_t len )
{
    uint16_t length;
    comcfg_t *cfg;

    cfg = GetUseCom(COM);
    taskENTER_CRITICAL();
    length = SerialRxPop(cfg, buf, len);
    taskEXIT_CRITICAL();
    
    return length;
}
#if COM_USE_NUM > 0
/***********only for Isr callback***********/
/**
  * @brief  ·¢ËÍ¿ÕÖÐ¶Ïµ÷ÓÃº¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM0_TXE_Isr_callback(void)
{
    uint8_t temp;
    isrSaveCriticial_status_Variable;

    isrENTER_CRITICAL();
    if(SerialTxPop(&comcfg0,&temp)){
        COM0PutByte(temp);
    }else{
        COM0TxIEDisable();
    }
    isrEXIT_CRITICAL();
}
/**
  * @brief  ·¢ËÍÍê³ÉÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM0_TXC_Isr_callback(void)
{


}
/**
  * @brief  ½ÓÊÕÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM0_RX_Isr_callback(void)
{
    uint8_t temp = COM0GetByte();
    isrSaveCriticial_status_Variable;

    isrENTER_CRITICAL();    
    SerialRxPut(&comcfg0,temp);
    isrEXIT_CRITICAL();
}
#endif
#if COM_USE_NUM > 1
/**
  * @brief  ·¢ËÍ¿ÕÖÐ¶Ïµ÷ÓÃº¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM1_TXE_Isr_callback(void)
{
    uint8_t temp;
    isrSaveCriticial_status_Variable;

    isrENTER_CRITICAL();
    if(SerialTxPop(&comcfg1,&temp)){
        COM1PutByte(temp);
    }else{
        COM1TxIEDisable();
    }
    isrEXIT_CRITICAL();
}
/**
  * @brief  ·¢ËÍÍê³ÉÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM1_TXC_Isr_callback(void)
{


}
/**
  * @brief  ½ÓÊÕÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM1_RX_Isr_callback(void)
{
    uint8_t temp = COM1GetByte();
    isrSaveCriticial_status_Variable;

    isrENTER_CRITICAL();    
    SerialRxPut(&comcfg1,temp);
    isrEXIT_CRITICAL();
}
#endif
#if COM_USE_NUM > 2
/**
  * @brief  ·¢ËÍ¿ÕÖÐ¶Ïµ÷ÓÃº¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM2_TXE_Isr_callback(void)
{
    uint8_t temp;
    isrSaveCriticial_status_Variable;

    isrENTER_CRITICAL();
    if(SerialTxPop(&comcfg2,&temp)){
        COM2PutByte(temp);
    }else{
        COM2TxIEDisable();
    }
    isrEXIT_CRITICAL();
}
/**
  * @brief  ·¢ËÍÍê³ÉÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM2_TXC_Isr_callback(void)
{


}
/**
  * @brief  ½ÓÊÕÖÐ¶Ï»Øµ÷º¯Êý
  * @param  None
  * @note   
  * @retval  None
  */
void COM2_RX_Isr_callback(void)
{
//    uint8_t temp = COM2GetByte();
//    isrSaveCriticial_status_Variable;
//
//    isrENTER_CRITICAL();    
//    SerialRxPut(&comcfg2,temp);
//    isrEXIT_CRITICAL();
}
#endif

/**
  * @brief  This function handles usart interrupt request.
  * @param  None
  * @retval None
  */
  
#if COM_USE_NUM > 0
void USART1_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_TXE(USART1)){
        COM0_TXE_Isr_callback();
    }
    
    if(LL_USART_IsActiveFlag_RXNE(USART1)){
        COM0_RX_Isr_callback();
    }
    if(LL_USART_IsActiveFlag_TC(USART1)){
        COM0_TXC_Isr_callback();
        LL_USART_ClearFlag_TC(USART1);
    }
    NVIC_ClearPendingIRQ(USART1_IRQn);
}
#endif
#if COM_USE_NUM > 1
void USART2_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_TXE(USART2 ) ){
        COM1_TXE_Isr_callback();
    }
    
    if(LL_USART_IsActiveFlag_RXNE(USART2)){
        COM1_RX_Isr_callback();
    }
    
    if(LL_USART_IsActiveFlag_TC(USART1)){
        COM1_TXC_Isr_callback();
        LL_USART_ClearFlag_TC(USART1);
    }
    NVIC_ClearPendingIRQ(USART2_IRQn);
}
#endif
#if COM_USE_NUM > 2
void USART3_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_TXE(USART3)){
        COM2_TXE_Isr_callback();
    }
    
    if(LL_USART_IsActiveFlag_RXNE(USART3)){
        COM2_RX_Isr_callback();
    }
    
    if(LL_USART_IsActiveFlag_TC(USART3)){
        COM2_TXC_Isr_callback();
        LL_USART_ClearFlag_TC(USART3);
    }
    NVIC_ClearPendingIRQ(USART3_IRQn);
}
#endif
