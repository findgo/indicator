
/**
  ******************************************************************************
  * @file 
  * @author  
  * @version 
  * @date    
  * @brief  
  ******************************************************************************
  * @attention 	20180920     v1.1   	jgb		
  * @attention 	20181213     v1.2   	jgb		 采用注册方式注册事件任务
  ******************************************************************************
  */


#ifndef __MO_TASK_H__
#define __MO_TASK_H__

#include "common_type.h"
#include "porting_def.h"
#include "msglink.h"
#include "port_atom.h"

// 定义最大任务数
#define TASK_MAX_CAP    	4
// 注册时超出最大任务数返回无id可用
#define TASK_NO_TASKID		0xff  // 超出任务数

// 定义一个系统级别事件, 用于消息的收发
#define SYS_EVENT_MSG               0x8000  // A message is waiting event

typedef uint16_t (*pTaskFn_t)( uint8_t task_id, uint16_t event );

extern void tasksPoll(void);
extern void powerconserve(void);

/* 注册任务,返回任务Id */
uint8_t task_register(pTaskFn_t fn);

/* 设置一个事件 */ 
#define tasks_setEventFromISR(task_id, event_flag)  tasks_setEvent(task_id, event_flag);
#define tasks_clearEventFromISR(task_id, event_flag)  tasks_clearEvent(task_id, event_flag);
uint8_t tasks_setEvent(uint8_t task_id, uint16_t event_flag);
uint8_t tasks_clearEvent(uint8_t task_id, uint16_t event_flag);
// 任务级消息发送
void *tasks_msg_alloc(uint16_t len);
int tasks_msg_dealloc(void *msg_ptr);
int tasks_msg_put(uint8_t dest_taskid, void *msg_ptr);
int tasks_msg_putFront(uint8_t dest_taskid, void *msg_ptr);
void *tasks_msg_accept(uint8_t task_id);
void *tasks_msg_peek(uint8_t task_id);

void tasks_Run_System(void);

#endif

