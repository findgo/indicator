#include "task.h"

void tasks_Run_System(void)
{
    uint8_t idx = 0;
    uint16_t events;

    tasksPoll();

#if configSUPPORT_TASKS_EVENT > 0
    // 移植的TI OSAL
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
#endif

}

__weak void tasks_init_System(void)
{

}
__weak void tasksPoll(void)
{

}
#if configSUPPORT_TASKS_EVENT > 0
// task id and qmsg map message
typedef struct {
    uint8_t task_id;
    msg_q_t q_taskmsghead;
}taskmsg_mapInfo_t; 

static taskmsg_mapInfo_t *tasksmap_search(uint8_t task_id);

static msg_q_t taskmsgInfoq_head = NULL; // 存放消息队列与任务的映射表
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

int tasks_msg_assign(uint8_t task_id)
{
    taskmsg_mapInfo_t *mapinfo;

    mapinfo = tasksmap_search(task_id);

    if(mapinfo == NULL){
        if ((mapinfo = msgalloc(sizeof(taskmsg_mapInfo_t))) == NULL)
            return MSG_INVALID_POINTER;

        mapinfo->task_id = task_id;
        mapinfo->q_taskmsghead = NULL;

        msgQputFront(&taskmsgInfoq_head, mapinfo);
    }
    
    return MSG_SUCCESS;
}

// not find it : NULL,else find it 
static taskmsg_mapInfo_t *tasksmap_search(uint8_t task_id)
{
    taskmsg_mapInfo_t *srch;

    msgQ_for_each_msg(&taskmsgInfoq_head, srch){
        if(srch->task_id == task_id){ // find it 
            break;
        }
    }

    return srch;
}

int tasks_msg_Genericput(uint8_t dest_taskid, void *msg_ptr, uint8_t isfront)
{
    taskmsg_mapInfo_t *mapinfo;

    if(dest_taskid >= tasksCnt){
        return MSG_INVALID_TASK;
    }

    if(msg_ptr == NULL)
        return MSG_INVALID_POINTER;
    
    mapinfo = tasksmap_search(dest_taskid);
    if(mapinfo){
        msgQGenericput(&(mapinfo->q_taskmsghead), msg_ptr, isfront);
    }
    else{
        msgsetspare(msg_ptr, dest_taskid);
        msgQGenericput(&taskmsgq_head, msg_ptr, isfront);
    }
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
    taskmsg_mapInfo_t *mapinfo;
    void *srcmsg;
    void *premsg;
    
    if(task_id >= tasksCnt){
        return NULL;
    }

    mapinfo = tasksmap_search(task_id);
    if(mapinfo){
        return msgQpop(&(mapinfo->q_taskmsghead));
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
    taskmsg_mapInfo_t *mapinfo;
    void *srcmsg;

    if(task_id >= tasksCnt){
        return NULL;
    }

    mapinfo = tasksmap_search(task_id);
    if(mapinfo){
        return msgQpop(&(mapinfo->q_taskmsghead));
    }
    
    msgQ_for_each_msg(&taskmsgq_head, srcmsg){
        if(msgspare(srcmsg) == task_id)
            break;
    }
    
    return srcmsg;
}


#endif

