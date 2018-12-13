#include "msglink.h"

#define MSGBOX_CNT(msgbox_ptr)      (((msgbox_t *) (msgbox_ptr))->count)
#define MSGBOX_CAP(msgbox_ptr)      (((msgbox_t *) (msgbox_ptr))->capacity)
#define MSGBOX_QHEAD(msgbox_ptr)      (((msgbox_t *) (msgbox_ptr))->qhead)

#define MSG_HDR_MARK(msg_ptr)      (((msg_hdr_t *) (msg_ptr) - 1)->mark)
#define MSG_HDR_SPARE(msg_ptr)      (((msg_hdr_t *) (msg_ptr) - 1)->spare)
#define MSG_HDR_LEN(msg_ptr)      (((msg_hdr_t *) (msg_ptr) - 1)->len)
#define MSG_HDR_NEXT(msg_ptr)      (((msg_hdr_t *) (msg_ptr) - 1)->next)

// 信息头部
typedef struct
{
    uint8_t mark;
    uint8_t spare;
    uint16_t len;
    void   *next;
} msg_hdr_t;

// 消息队列结构体
typedef struct
{
    uint16_t count; 
    uint16_t capacity;
    msg_q_t qhead;
} msgbox_t;

void *msgalloc(const uint16_t msglen )
{
    msg_hdr_t *hdr;

    if ( msglen == 0 )
        return (void *)( NULL );

    hdr = ( msg_hdr_t *) mo_malloc( (size_t)(sizeof( msg_hdr_t ) + msglen ) );
    if ( hdr ) {
        //init it
        hdr->next = NULL;
        hdr->len = msglen;
        hdr->mark = FALSE; // not on qbox list
        hdr->spare = 0;
        
        return ( (void *) (hdr + 1) ); // point to the data
    }

    return (void *)( NULL );
}

int msgdealloc( void *const msg_ptr )
{
    if ( msg_ptr == NULL )
        return ( MSG_INVALID_POINTER );

    // don't deallocate msg buffer when it on the list
    if ( MSG_HDR_MARK( msg_ptr ) == TRUE )
        return ( MSG_BUFFER_NOT_AVAIL );

    mo_free(  (void *)((uint8_t *)msg_ptr - sizeof( msg_hdr_t )) );

    return ( MSG_SUCCESS );
}

uint16_t msglen( void *const msg_ptr)
{
    if(msg_ptr == NULL)
        return 0;

    return MSG_HDR_LEN(msg_ptr);
}

int msgsetspare( void *const msg_ptr, const uint8_t val)
{
    if ( msg_ptr == NULL )
        return ( MSG_INVALID_POINTER );

    MSG_HDR_SPARE(msg_ptr) = val;
    
    return MSG_SUCCESS;
}

uint8_t msgspare( void *const msg_ptr)
{
    if ( msg_ptr == NULL )
        return 0;

    return MSG_HDR_SPARE(msg_ptr);
}

msgboxhandle_t msgBoxNew(const uint16_t MaxCap)
{
    msgbox_t *pNewmsgbox;

    pNewmsgbox = ( msgbox_t * ) mo_malloc( sizeof( msgbox_t ) );

    if(pNewmsgbox){
        pNewmsgbox->capacity = MaxCap;
        pNewmsgbox->count = 0;
        pNewmsgbox->qhead = NULL;
    }

    return (msgboxhandle_t )pNewmsgbox;
}

msgboxhandle_t msgBoxAssign(msgboxstatic_t *const pmsgboxBuffer, const uint16_t MaxCap)
{
    msgbox_t *pNewmsgbox = ( msgbox_t * )pmsgboxBuffer;

    if( pNewmsgbox ){
        pNewmsgbox->capacity = MaxCap;
        pNewmsgbox->count = 0;
        pNewmsgbox->qhead = NULL;
    }

    return (msgboxhandle_t )pNewmsgbox;
}

uint16_t msgBoxcnt(const msgboxhandle_t msgbox )
{
    if ( msgbox == NULL )
        return 0;

    return MSGBOX_CNT(msgbox);
}

uint16_t msgBoxIdle(const msgboxhandle_t msgbox )
{
    if ( msgbox == NULL )
        return 0;

    
    return (MSGBOX_CAP(msgbox) - MSGBOX_CNT(msgbox));
}

void *msgBoxaccept(const msgboxhandle_t msgbox )
{
    // no message on the list
    if(MSGBOX_CNT(msgbox) == 0)
        return NULL;

    MSGBOX_CNT(msgbox)--;
    
    return msgQpop(&MSGBOX_QHEAD(msgbox));
}

void *msgBoxpeek(const msgboxhandle_t msgbox )
{
    // no message on the list
    if(MSGBOX_CNT(msgbox) == 0)
        return NULL;
    
    return msgQpeek(&MSGBOX_QHEAD(msgbox));
}

int msgBoxGenericpost(const msgboxhandle_t msgbox, void *const msg_ptr,const uint8_t isfront )
{
    if ( msg_ptr == NULL || msgbox == NULL) {
        return ( MSG_INVALID_POINTER );
    }

    if(MSGBOX_CAP(msgbox) != MSGBOX_UNLIMITED_CAP && ((MSGBOX_CAP(msgbox) - MSGBOX_CNT(msgbox)) < 1) )
        return MSG_QBOX_FULL;
    
    // Check the message header ,not init it success, or message on the list
    if ( MSG_HDR_NEXT( msg_ptr ) != NULL || MSG_HDR_MARK( msg_ptr ) != FALSE ) {        
        return ( MSG_INVALID_POINTER );
    }

    MSGBOX_CNT(msgbox)++;
    msgQGenericput(&MSGBOX_QHEAD(msgbox), msg_ptr, isfront);

    return ( MSG_SUCCESS );
}

void msgQGenericput(msg_q_t *const q_ptr, void *const msg_ptr,const uint8_t isfront )
{
    void *list;

    MSG_HDR_MARK( msg_ptr ) = TRUE; // mark on the list
    if(isfront == TRUE){ // put to front
        // Push message to head of queue
        MSG_HDR_NEXT( msg_ptr ) = *q_ptr;
        *q_ptr = msg_ptr;
    }
    else { // put to back
        // set nex to null
        MSG_HDR_NEXT( msg_ptr ) = NULL;
        // If first message in queue
        if ( *q_ptr == NULL ){
            *q_ptr = msg_ptr;
        }
        else {
            // Find end of queue
            for ( list = *q_ptr; MSG_HDR_NEXT( list ) != NULL; list = MSG_HDR_NEXT( list ) );

            // Add message to end of queue
            MSG_HDR_NEXT( list ) = msg_ptr;
        }
    }
}

//ok
void *msgQpop( msg_q_t *const q_ptr )
{
    void *msg_ptr = NULL;

    if ( *q_ptr != NULL ) {
        // Dequeue message
        msg_ptr = *q_ptr;
        *q_ptr = MSG_HDR_NEXT( msg_ptr );
        MSG_HDR_NEXT( msg_ptr ) = NULL;
        MSG_HDR_MARK( msg_ptr ) = FALSE;
    }

  return msg_ptr;
}
//ok
void *msgQpeek( msg_q_t *const q_ptr )
{
    return (void *)(*q_ptr);    
}

//ok
// Take out of the link list
void msgQextract( msg_q_t *const q_ptr, void *const msg_ptr, void *const premsg_ptr )
{
    if ( msg_ptr == *q_ptr ) {
        // remove from first
        *q_ptr = MSG_HDR_NEXT( msg_ptr );
    }
    else {
        // remove from middle
        MSG_HDR_NEXT( premsg_ptr ) = MSG_HDR_NEXT( msg_ptr );
    }
    MSG_HDR_NEXT( msg_ptr ) = NULL;
    MSG_HDR_MARK( msg_ptr ) = FALSE;
}
// get next message
void *msgQnext(void *const msg_ptr)
{
    return MSG_HDR_NEXT( msg_ptr );
}




