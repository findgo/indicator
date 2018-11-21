/**
  ******************************************************************************
  * @file   mkey.h
  * @author   jgb
  * @version 
  * @date    
  * @brief    key模块  
  * @ NOTE: 本模块暂不支持组合键
  ******************************************************************************
  * @attention      v1.1   	jgb		20151117  重构 		
  * @attention      v1.2        		20170306  修改框架，直接返回键值
  * @attention      v1.3              20170429  再次重构，避免更多用户修改介入 
  * @attention      v1.4              20181024  减少对用户的数据暴露
  ******************************************************************************
  */
/*
; 关于按键功能说明
; 支持短按长按功能时，触发长按时，不会触发短按效果
; 支持短按连发，触发连发时，是不会触发短按状态，虽然连发和短按键值一样，另外这种情况，连发前有个默认长延时
;
*/
/*  使用说明
 *  根据mkeydecetor_task 周期调用时间定义宏
 *  定义要使用的键值  短按 长按  抬键
 *  调用mkeyAssign创建按键功能
 *  调mkey_getvalue 获得键值
 *
 */

#ifndef __MKEY_H_
#define __MKEY_H_

#include "app_cfg.h"

//缓冲区大小
#define KEY_QUEUE_MAX_SIZE		3
//默认滤波时间,根据mkeydecetor_task
#define KEY_DEFAULT_FILTER_TIME     	2   // 滤波时间
//如果不支持长按，但支持连击，有个长延时到连击的默认时间
//如果长按和连击都不支持，直接到抬键
#define KEY_DEFAULT_LONGTOREPEAT_TIME     50

typedef uint8_t (*mKeyDownFunc_t)(void);

//静态结构体,用于屏蔽用户对结构体的可见
typedef struct {   
    uint8_t (*xDummy0)(void);
    void *xDummy1;
    uint8_t xDummy2;
    uint8_t xDummy3;
    uint16_t xDummy4;
    uint16_t xDummy5;
    uint8_t xDummy6;
    uint8_t xDummy7;
    uint8_t xDummy8;
    uint8_t xDummy9;
    uint8_t xDummy10;
}mkeycfgStatic_t;

/*按键值 用户修改*/
enum
{
	MKEY_NULL = 0,			/* 0 表示无按键事件 */	

	MKEY_PRESS1_DOWN,			
	MKEY_PRESS1_LONG, 		
	MKEY_PRESS1_UP
};

void mkeyAssign(mkeycfgStatic_t *cfg,
                mKeyDownFunc_t IsmKeyDownFunc_t,    // 定义该函数 按键下 返回1
                uint8_t KeyCodeDown,                // 短按编码, 为MKEY_NULL将忽略短按
                uint8_t KeyCodeLong,                // 长按编码
                uint8_t KeyCodeUp,                  // 为 MKEY_NULL表示不支持抬键,不会触发KeyCodeUp
                uint8_t filterTime,                 // 为 0 将使用默认滤波时间
                uint16_t longTime,                  // 为 0 表示不支持长按, 此时不会触发长击KeyCodeLong
                uint8_t repeatSpeed);               // 为 0 表示不支持连击, 此时不会触发连击KeyCodeDown

//每隔一定周期做一次键扫 一般为10ms
void mkeydecetor_task(void);
// 获取键值,由上面枚举定义的
uint8_t mkeygetvalue(void);

#endif


