#include "task.h"


static pTaskFn_t tasksArr[TASK_MAX_CAP];
static uint16_t tasksEvents[TASK_MAX_CAP];;
static uint8_t tasksCnt = 0;

/* 注册任务,返回任务Id */
uint8_t task_register(pTaskFn_t fn)
{
	if(tasksCnt < TASK_MAX_CAP){
		tasksArr[tasksCnt] = fn;
		return tasksCnt++;
	}

	return TASK_NO_TASKID;
}


void tasks_Run_System(void)
{
    uint8_t idx = 0;
    uint16_t events;

    tasksPoll();

    for(idx = 0;idx < tasksCnt; ++idx)
    {
        if (tasksEvents[idx]){ // Task is highest priority that is ready.
            break;
        }
    }
    
    if (idx < tasksCnt) {
        taskENTER_CRITICAL();
        events = tasksEvents[idx];
        tasksEvents[idx] = 0;  // Clear the Events for this task.
        taskEXIT_CRITICAL();
    
        events = (tasksArr[idx])( idx, events );
    
        taskENTER_CRITICAL();
        tasksEvents[idx] |= events;  // Add back unprocessed events to the current task.
        taskEXIT_CRITICAL();
    }
#if defined( configSUPPORT_POWER_SAVING )
    else { // Complete pass through all task events with no activity?
        powerconserve();  // Put the processor/system into sleep
    }
#endif
}

__weak void tasksPoll(void)
{

}

static msg_q_t taskmsgq_head = NULL; //存放在Info找不到映射表的消息 

uint8_t tasks_setEvent(uint8_t task_id, uint16_t event_flag)
{
    isrSaveCriticial_status_Variable;

    if ( task_id < tasksCnt ){
        isrENTER_CRITICAL();
        tasksEvents[task_id] |= event_flag; 
        isrEXIT_CRITICAL();
        return TRUE;
    }
    
    return FALSE;
}

uint8_t tasks_clearEvent(uint8_t task_id, uint16_t event_flag)
{
    isrSaveCriticial_status_Variable;

    if ( task_id < tasksCnt ){
        isrENTER_CRITICAL();
        tasksEvents[task_id] &= ~event_flag; 
        isrEXIT_CRITICAL();
        return TRUE;
    }
    
    return FALSE;
}

void *tasks_msg_alloc(uint16_t len)
{
	return msgalloc(len);
} 

int tasks_msg_dealloc(void *msg_ptr)
{
	return msgdealloc(msg_ptr);
}

int tasks_msg_Genericput(uint8_t dest_taskid, void *msg_ptr, uint8_t isfront)
{
    if(dest_taskid >= tasksCnt){
        return MSG_INVALID_TASK;
    }

    if(msg_ptr == NULL)
        return MSG_INVALID_POINTER;
    

    msgsetspare(msg_ptr, dest_taskid);
    msgQGenericput(&taskmsgq_head, msg_ptr, isfront);
    
    tasks_setEvent(dest_taskid, SYS_EVENT_MSG);
    
    return MSG_SUCCESS;
}

int tasks_msg_put(uint8_t dest_taskid, void *msg_ptr)
{
    return tasks_msg_Genericput(dest_taskid, msg_ptr, FALSE);
}

int tasks_msg_putFront(uint8_t dest_taskid, void *msg_ptr)
{
    return tasks_msg_Genericput(dest_taskid, msg_ptr, TRUE);
}

void *tasks_msg_accept(uint8_t task_id)
{
    void *srcmsg;
    void *premsg;
    
    if(task_id >= tasksCnt){
        return NULL;
    }
    
    msgQ_for_each_msg(&taskmsgq_head, srcmsg){
        if(msgspare(srcmsg) == task_id)
            break;
       premsg = srcmsg;
    }

    if(srcmsg){ // take out list
        msgQextract(&taskmsgq_head, srcmsg, premsg);
    }
    
    return srcmsg;
}

void *tasks_msg_peek(uint8_t task_id)
{
    void *srcmsg;

    if(task_id >= tasksCnt){
        return NULL;
    }
    
    msgQ_for_each_msg(&taskmsgq_head, srcmsg){
        if(msgspare(srcmsg) == task_id)
            break;
    }
    
    return srcmsg;
}
