/* 本方档移植至TI的OSAL     用于单片机的小型事件触发系统 */

#ifndef __MO_TASK_H__
#define __MO_TASK_H__

#include "common_type.h"
#include "porting_def.h"
#include "msglink.h"
#include "port_atom.h"

// 定义一个系统级别事件, 用于消息的收发
#define SYS_EVENT_MSG               0x8000  // A message is waiting event

typedef uint16_t (*pTaskFn_t)( uint8_t task_id, uint16_t event );

extern void tasks_init_System(void);
extern void tasksPoll(void);
extern void powerconserve(void);

#if configSUPPORT_TASKS_EVENT > 0
extern const pTaskFn_t tasksArr[];
extern const uint8_t tasksCnt;
extern uint16_t *tasksEvents;
#endif
/* 设置一个事件 */ 
#define tasks_setEventFromISR(task_id, event_flag)  tasks_setEvent(task_id, event_flag);
#define tasks_clearEventFromISR(task_id, event_flag)  tasks_clearEvent(task_id, event_flag);
uint8_t tasks_setEvent(uint8_t task_id, uint16_t event_flag);
uint8_t tasks_clearEvent(uint8_t task_id, uint16_t event_flag);

void *tasks_msg_alloc(uint16_t len);
int tasks_msg_dealloc(void *msg_ptr);
int tasks_msg_assign(uint8_t task_id);
int tasks_msg_put(uint8_t dest_taskid, void *msg_ptr);
int tasks_msg_putFront(uint8_t dest_taskid, void *msg_ptr);
void *tasks_msg_accept(uint8_t task_id);
void *tasks_msg_peek(uint8_t task_id);

void tasks_Run_System(void);


#endif

