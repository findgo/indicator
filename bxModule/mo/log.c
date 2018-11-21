

#include <stdio.h>
#include <stdarg.h>
#include "log.h"

#if defined(GLOBAL_DEBUG)

//默认只输出debug等级
static uint8_t default_level = LOG_LEVEL_DEBUG;


static void lowlevelPrefix_log(uint8_t level)
{
    switch (level){
    case LOG_LEVEL_ALERT:
        printf("[A]:");
        break;
    case LOG_LEVEL_ERROR:
        printf("[E]:");
        break;
    case LOG_LEVEL_WARN:
        printf("[W]:");
        break;
    case LOG_LEVEL_NOTICE:
        printf("[N]:");
        break;
    case LOG_LEVEL_INFO:
        printf("[I]:");
        break;
    case LOG_LEVEL_DEBUG:
        printf("[D]:");
        break;
    default:
        break;
    }
}

// 默认输出到stdout
void log_ll(uint8_t level,const char *format,...)
{
    va_list ap;

    if( level <= default_level ){ 
        lowlevelPrefix_log(level);
        va_start(ap,format);
        vprintf(format,ap);
        va_end(ap);
    }   
}

void log_llln(uint8_t level,const char *format,...)
{
    va_list ap;

    if( level <= default_level ){ 
        lowlevelPrefix_log(level);
        va_start(ap,format);
        vprintf(format,ap);
        va_end(ap);
        printf("\r\n");
    }   
}

void log_set_max_level(uint8_t level) 
{
    default_level = MIN(level, LOG_LEVEL_DEBUG);
}
void log_Init(void)
{
    lowlogInit();
}

/* 重定向fputc 到输出，单片机一般为串口*/ 
int fputc(int ch, FILE *f)
{
    /* e.g. write a character to the USART */
    lowlogputChar(ch);
    
//    (void)Serial_WriteByte(COM2,ch);
//    USART_SendData(USART_USING2,ch);
    /* Loop until the end of transmission */
//    while(USART_GetFlagStatus(USART_USING2, USART_FLAG_TC) == RESET);
    
    return ch;
}

#else
void log_ll(uint8_t level,const char *format,...)
{
    (void)level;
}
void log_llln(uint8_t level,const char *format,...)
{
    (void)level;
}

void log_set_max_level(uint8_t level) 
{
    (void)level;
}
void log_Init(void)
{

}
#endif
