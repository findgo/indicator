/*
 *  realize four level log module
 * log 等级说明 六个等级 可以设置最大等级,在最大等级以下的log都会打印出来,默认输出trance级别(也就是DEBUG级别)
 *
 * Alert : 用于报告系统级错误, 看情况使用,目前保留
 * Error : 应用级错误,比如内存分配失败等
 * Warn : 应用级警告,不会发现对应用的影响
 * Notice : 应用级 关键系跳转,比如消息发送 标志位变更什么的
 * Info : 应用级 用户信息输出
 * Debug,Trace: 应用级调试信息
 */

#ifndef __MO_LOG_H__
#define __MO_LOG_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "common_type.h"
#include "mserial.h"
#include "usart.h"

enum {
    LOG_LEVEL_ALERT = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARN,
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
};


#define log_alert(format,args...)       log_ll(LOG_LEVEL_ALERT,format,##args)
#define log_error(format,args...)       log_ll(LOG_LEVEL_ERROR,format,##args)
#define log_warn(format,args...)        log_ll(LOG_LEVEL_WARN,format,##args)
#define log_notice(format,args...)      log_ll(LOG_LEVEL_NOTICE,format,##args)
#define log_info(format,args...)        log_ll(LOG_LEVEL_INFO,format,##args)
#define log_debug(format,args...)       log_ll(LOG_LEVEL_DEBUG,format,##args)
#define log_trace(format,args...)       log_ll(LOG_LEVEL_DEBUG,format,##args)

#define log_alertln(format,args...)     log_llln(LOG_LEVEL_ALERT,format,##args)
#define log_errorln(format,args...)     log_llln(LOG_LEVEL_ERROR,format,##args)
#define log_warnln(format,args...)      log_llln(LOG_LEVEL_WARN,format,##args)
#define log_noticeln(format,args...)    log_llln(LOG_LEVEL_NOTICE,format,##args)
#define log_infoln(format,args...)      log_llln(LOG_LEVEL_INFO,format,##args)
#define log_debugln(format,args...)     log_llln(LOG_LEVEL_DEBUG,format,##args)
#define log_traceln(format,args...)     log_llln(LOG_LEVEL_DEBUG,format,##args)

void log_set_max_level(uint8_t level);
void log_Init(void);
void log_llln(uint8_t level,const char *format,...);
void log_ll(uint8_t level,const char *format,...);


// 定义一个外部初始化
#define lowlogInit() do {SerialDrvInit(COM2, 115200, 8, DRV_PAR_NONE); log_set_max_level(LOG_LEVEL_ALERT); }while(0)
// 重定向fput的字符
#define lowlogputChar(c) Serial_WriteByte(COM2,c);


#ifdef __cplusplus
}  /* extern "C */
#endif

#endif

