#ifndef __LOOP_H__
#define __LOOP_H__

#include "common_type.h"
#include "common_def.h"

//使能事件触发
#define LOOP_TASKS_EVENT_ENABLE  (0)

// 定义一个系统级别事件
#define SYS_EVENT_MSG               0x8000  // A message is waiting event

typedef uint16_t (*pTaskFn_t)( uint8_t task_id, uint16_t event );
typedef void (*pPollFn_t)(void);

#define tasks_setEventFromISR(task_id, event_flag)  tasks_setEvent(task_id, event_flag);
#define tasks_clearEventFromISR(task_id, event_flag)  tasks_clearEvent(task_id, event_flag);

uint8_t tasks_setEvent(uint8_t task_id, uint16_t event_flag);
uint8_t tasks_clearEvent(uint8_t task_id, uint16_t event_flag);


void loop_init_System(void);
void loop_Run_System(void);

#endif

