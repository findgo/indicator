#include "mbfunc.h"
#include "port.h"
#include "mb.h"

#if MB_RTU_ENABLED > 0
#include "mbrtu.h"
#endif
#if MB_ASCII_ENABLED > 0
#include "mbascii.h"
#endif
#if MB_TCP_ENABLED > 0
#include "mbtcp.h"
#endif
#include "mbutils.h"

/* TODO implement modbus master */
#if (MB_RTU_ENABLED > 0 || MB_ASCII_ENABLED > 0) && MB_MASTER_ENABLED > 0
#include "mbmem.h"
#include "mbmbuf.h"
static MbErrorCode_t __MbmHandle(MbmDev_t *dev,uint32_t timediff);
static void __MbmReqPendlistScan(MbmDev_t *dev, uint32_t diff);
static MbmDev_t *__MbmdevSearch(uint8_t port);

static msg_q_t mbm_dev_head = NULL;

//static msgboxstatic_t msgboxHandlebuf = MSGBOX_STATIC_INIT(MSGBOX_UNLIMITED_CAP);

Mbmhandle_t MbmNew(MbMode_t eMode, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity)
{
    MbErrorCode_t eStatus = MB_ENOERR;
    MbmDev_t *dev;

    if((dev = (MbmDev_t *)msgalloc(sizeof(MbmDev_t))) == NULL)
        return NULL;
    
    memset(dev, 0, sizeof(MbmDev_t));    
    
    switch (eMode){
#if MB_RTU_ENABLED > 0
    case MB_RTU:
        dev->pMbStartCur = MbmRTUStart;
        dev->pMbStopCur = MbmRTUStop;
        dev->pMbCloseCur = MbmRTUClose;
        dev->pMbSendCur = MbmRTUSend;
        dev->pMbReceivedCur = MbmRTUReceive;

        eStatus = MbmRTUInit(dev, ucPort, ulBaudRate, eParity);
        break;
#endif

#if MB_ASCII_ENABLED > 0
    case MB_ASCII:
        dev->pMbStartCur = MbmASCIIStart;
        dev->pMbStopCur = MbmASCIIStop;
        dev->pMbCloseCur = MbmASCIIClose;
        dev->pMbSendCur = MbmASCIISend;
        dev->pMbReceivedCur = MbmASCIIReceive;
        
        eStatus = MbmASCIIInit(dev, ucPort, ulBaudRate, eParity);
        break;
#endif

    default:
        eStatus = MB_EINVAL;
        break;
        
    }

    if(eStatus != MB_ENOERR){
        msgdealloc(dev);
        return NULL;
    }

    // init parameter
    dev->port         = ucPort;
    dev->mode         = eMode;        
    dev->T50PerCharater = (ulBaudRate > 19200) ? 10 : (220000 / ulBaudRate);
    
    dev->devstate     = DEV_STATE_DISABLED;

    dev->nodehead     = NULL;

    dev->Reqreadyhead = NULL;
    dev->Reqpendinghead  = NULL;

    dev->Pollstate    = MBM_IDLE;
    
    dev->retry        = MBM_DEFAULT_RETRY_COUNT;
    dev->retrycnt     = 0;

    dev->XmitingTime        = 0;
    dev->Replytimeout       = MBM_DEFAULT_REPLYTIMEOUT;
    dev->Replytimeoutcnt    = 0;
    dev->Delaypolltime      = MBM_DEFAULT_DELAYPOLLTIME;
    dev->Delaypolltimecnt   = 0;
    dev->Broadcastturntime  = MBM_DEFAULT_BROADTURNTIME;
    dev->Broadcastturntimecnt = 0;
   
    // check the device on the list ?
    if(__MbmdevSearch(dev->port)){
        msgdealloc(dev);

        return NULL;
    }
    msgQputFront(&mbm_dev_head, dev);
    
    return ( Mbmhandle_t )dev;
}

// must be delete all the request and node
//不实现
void MbmFree(uint8_t ucPort)
{
/*
    MbmDev_t *srh = NULL;
    MbmDev_t *pre = NULL;

    msgQ_for_each_msg(&mbm_dev_head, srh){
        if(srh->port == ucPort)
            break;

        pre = srh; // save previous msg      
    }
    
    if(srh){
        msgQextract(&mbm_dev_head, srh, pre);
        msgdealloc(srh);
    }
    */
}

MbErrorCode_t MbmSetPara(Mbmhandle_t dev, uint8_t retry,uint32_t replytimeout,
                                    uint32_t delaypolltime, uint32_t broadcastturntime)
{
    MbmDev_t *pdev = ( MbmDev_t * )dev;
    
    if(pdev == NULL)
        return MB_EINVAL;
    
    pdev->retry = (retry > MBM_RETRY_COUNT_MAX) ? MBM_RETRY_COUNT_MAX : retry;
    
    if(replytimeout < MBM_REPLYTIMEOUT_MIN)
        pdev->Replytimeout = MBM_REPLYTIMEOUT_MIN;
    else if(replytimeout > MBM_REPLYTIMEOUT_MAX)
        pdev->Replytimeout = MBM_REPLYTIMEOUT_MAX;
    else
        pdev->Replytimeout = replytimeout;

    if(delaypolltime < MBM_DELAYPOLLTIME_MIN)
        pdev->Delaypolltime = MBM_DELAYPOLLTIME_MIN;
    else if(delaypolltime > MBM_DELAYPOLLTIME_MAX)
        pdev->Delaypolltime = MBM_DELAYPOLLTIME_MAX;
    else
        pdev->Delaypolltime = delaypolltime;

    if(broadcastturntime < MBM_DELAYPOLLTIME_MIN)
        pdev->Broadcastturntime = MBM_BROADTURNTIME_MIN;
    else if(broadcastturntime > MBM_BROADTURNTIME_MAX)
        pdev->Broadcastturntime = MBM_BROADTURNTIME_MAX;
    else
        pdev->Broadcastturntime = broadcastturntime;

    return MB_ENOERR;
}
/* 创建一个独立节点和寄存器列表 */
MbmNode_t *MbmNodeNew(uint8_t slaveaddr,
                                uint16_t reg_holding_addr_start, uint16_t reg_holding_num,
                                uint16_t reg_input_addr_start, uint16_t reg_input_num,
                                uint16_t reg_coils_addr_start, uint16_t reg_coils_num,
                                uint16_t reg_discrete_addr_start, uint16_t reg_discrete_num)
{
    uint32_t lens;
    uint8_t *regbuf;
    MbReg_t *reg;
    MbmNode_t *node;

    /* check slave address valid */
    if(slaveaddr < MB_ADDRESS_MIN || slaveaddr > MB_ADDRESS_MAX)
        return NULL;
    
    if((node = (MbmNode_t *)msgalloc(sizeof(MbmNode_t))) == NULL){
        return NULL;
    }
    memset(node, 0, sizeof(MbmNode_t));
    
    lens = MbRegBufSizeCal(reg_holding_num, reg_input_num, reg_coils_num, reg_discrete_num);
    regbuf = (uint8_t *)mo_malloc(lens);
    if(regbuf == NULL){
        msgdealloc(node);
        return NULL;
    }
    memset(regbuf, 0, lens);
    
    reg = (MbReg_t *)&node->regs;
    
    reg->reg_holding_addr_start = reg_holding_addr_start;
    reg->reg_holding_num = reg_holding_num;
    
    reg->reg_input_addr_start = reg_input_addr_start;    
    reg->reg_input_num = reg_input_num;
        
    reg->reg_coils_addr_start = reg_coils_addr_start;
    reg->reg_coils_num = reg_coils_num;
    
    reg->reg_discrete_addr_start = reg_discrete_addr_start;
    reg->reg_discrete_num = reg_discrete_num;

    reg->pReghold = (uint16_t *)&regbuf[0];    
    lens = reg_holding_num * sizeof(uint16_t);
    
    reg->pReginput = (uint16_t *)&regbuf[lens];
    lens +=  reg_input_num * sizeof(uint16_t);
    
    reg->pRegCoil = (uint8_t *)&regbuf[lens];   
    lens += (reg_coils_num >> 3) + (((reg_coils_num & 0x07) > 0) ? 1 : 0);
    
    reg->pRegDisc = (uint8_t *)&regbuf[lens];
    
    node->slaveaddr = slaveaddr;

    return node;
}
// 给节点安排一个回调函数    
void MbmNodeCallBackAssign(MbmNode_t *node, pfnReqResultCB cb, void *arg)
{
    if(node){
        node->cb = cb;
        node->arg = arg;
    }
}

/* 释放节点，释放由MbmNodeNew创建的节点内存 */
void MbmNodeFree(MbmNode_t *node)
{
    if(node){
        if(node->regs.pReghold)
            mo_free(node->regs.pReghold);
        msgdealloc(node);
    }
}

/* 将节点加入到主机，由MbmNodeNew创建的节点 */
MbErrorCode_t MbmAddNode(Mbmhandle_t dev, MbmNode_t *node)
{
    MbmNode_t *srhnode;
    
    if(dev == NULL || node == NULL)
        return MB_EINVAL;
    
    /* check slave address valid */
    if(node->slaveaddr < MB_ADDRESS_MIN || node->slaveaddr > MB_ADDRESS_MAX)
        return MB_EILLNODEADDR;

    // check node on the list?
    srhnode = MbmSearchNode(dev, node->slaveaddr);
    if(srhnode)
        return MB_ENODEEXIST;

    msgQputFront(&(((MbmDev_t *)dev)->nodehead), node);

    return MB_ENOERR;
}

/* 将节点从主机删除 */
MbErrorCode_t MbmRemoveNode(Mbmhandle_t dev, uint8_t slaveaddr)
{
    MbmNode_t *srchnode;
    MbmNode_t *prenode = NULL;
    
    if( dev == NULL )
        return MB_EINVAL;

    msgQ_for_each_msg(&(((MbmDev_t *)dev)->nodehead), srchnode){
        if(srchnode->slaveaddr == slaveaddr) // find it
            break;
    
        prenode = srchnode;
    }

    if(srchnode){
        //first remove from node list
        msgQextract(&(((MbmDev_t *)dev)->nodehead), srchnode, prenode);
    }
    // init
    srchnode->slaveaddr = 0;
    
    return MB_ENOERR;
}

/* search node on the host list ? */
MbmNode_t *MbmSearchNode(Mbmhandle_t dev, uint8_t slaveaddr)
{
    MbmNode_t *srh;

    if(dev == NULL)
        return NULL;

    msgQ_for_each_msg(&(((MbmDev_t *)dev)->nodehead), srh){
        if(srh->slaveaddr == slaveaddr)
            break;
    }

    return srh;
}


MbErrorCode_t MbmStart(Mbmhandle_t dev)
{
    MbmDev_t *pdev = (MbmDev_t *)dev;

    if( pdev->devstate == DEV_STATE_NOT_INITIALIZED )
        return MB_EILLSTATE;

    if( pdev->devstate == DEV_STATE_DISABLED ){
        /* Activate the protocol stack. */
        pdev->pMbStartCur(dev);
        pdev->devstate = DEV_STATE_ENABLED;
    }

    return MB_ENOERR;
}

MbErrorCode_t MbmStop(Mbmhandle_t dev)
{
    MbmDev_t *pdev = (MbmDev_t *)dev;

    if( pdev->devstate == DEV_STATE_NOT_INITIALIZED )
        return MB_EILLSTATE;

    if( pdev->devstate == DEV_STATE_ENABLED ){
        pdev->pMbStopCur(dev);
        pdev->devstate = DEV_STATE_DISABLED;
    }

    return MB_ENOERR;
}

MbErrorCode_t MbmClose(Mbmhandle_t dev)
{    
    MbmDev_t *pdev = (MbmDev_t *)dev;
    
    // must be stop first then it can close
    if( pdev->devstate == DEV_STATE_DISABLED ){
        if( pdev->pMbCloseCur != NULL ){
            pdev->pMbCloseCur(dev);
        }

        return MB_ENOERR;
    }
    
    return MB_EILLSTATE;
}

 MbReqResult_t MbmSend(Mbmhandle_t dev, MbmReq_t *req)
{
    uint16_t crc_lrc;
    MbmDev_t *pdev = (MbmDev_t *)dev;
    
    if(pdev->mode == MB_RTU) {
#if MB_RTU_ENABLED > 0
        crc_lrc = MbCRC16(req->adu, req->adulength);
        req->adu[req->adulength++] = crc_lrc & 0xff;
        req->adu[req->adulength++] = (crc_lrc >> 8) & 0xff;
#endif
    }
    else if(pdev->mode == MB_ASCII) {
#if MB_ASCII_ENABLED > 0
        crc_lrc = MbLRC(req->adu,req->adulength);
        req->adu[req->adulength++] = crc_lrc & 0xff;
#endif
    }
    else {
        /* tcp no check sum */
    }
    
    if(req->scanrate){ 
        msgQput(&(pdev->Reqpendinghead) , req);
    }
    else {
        // if zero add ready list immediately but only once
        msgQput(&(pdev->Reqreadyhead) , req);
    }
    
    return MBR_ENOERR;
}

// TODO:  retry cnt 
void MbmPoll(void)
{
    static uint32_t HistimerCounter = 0;
    MbmDev_t *curdev;    
    uint32_t diff = 0;
    uint32_t tms;

    tms = MbSys_now();

    if(tms != HistimerCounter){
        diff = (uint32_t)( ( MbSys_now() - HistimerCounter ) & 0xffffffffu );
        HistimerCounter = tms;
    }

    msgQ_for_each_msg(&mbm_dev_head, curdev){
        __MbmHandle( curdev, diff );
    }
}

static MbErrorCode_t __MbmHandle(MbmDev_t *dev, uint32_t timediff)
{
    uint8_t *pRemainFrame; // remain fram
    uint8_t ucFunctionCode;
    uint16_t usLength;
    MbException_t exception;
    MbReqResult_t result;
    MbHeader_t header;
    MbmReq_t *req;

    pMbmParseRspHandler handle;

    /* Check if the protocol stack is ready. */
    if( dev->devstate != DEV_STATE_ENABLED ){
        return MB_EILLSTATE;
    }
      
    switch (dev->Pollstate){
    case MBM_IDLE:
        dev->Delaypolltimecnt = 0;
        dev->Pollstate = MBM_DELYPOLL;
        break;
    case MBM_XMIT: 
        req = msgQpeek(&(dev->Reqreadyhead)); // peek ready list ,any request on the list?
        if(req && (dev->pMbSendCur(dev,req->adu,req->adulength) == MB_ENOERR)){
            if(dev->mode == MB_RTU){
                dev->XmitingTime = dev->T50PerCharater * 50 * req->adulength / 1000 + 1;
            }
            else {
                dev->XmitingTime = dev->T50PerCharater * 50 * (req->adulength + 1) * 2 / 1000 ;
            }
            dev->Replytimeoutcnt = 0;
            dev->Pollstate = MBM_WAITRSP;
        }
        else{ /* nothing want to send or send error, wait a moment to try*/
            dev->Pollstate = MBM_DELYPOLL;
        }
        break;

    case MBM_RSPEXCUTE:  // response excute  
        req = msgQpop(&(dev->Reqreadyhead));// pop from ready list
        if(req == NULL) { /* some err happen ,then no request in list*/
            dev->Pollstate = MBM_DELYPOLL;
            break;
        }
    
        /* parser a adu fram */
        result = dev->pMbReceivedCur(dev, &header, &ucFunctionCode, &pRemainFrame, &usLength);
        if(result == MBR_ENOERR){            
            /* not for us ,continue to wait response */
            if((req->funcode != (ucFunctionCode & 0x7f)) || (req->slaveaddr != header.introute.slaveid)){
                dev->Pollstate = MBM_WAITRSP;
                break;
            }
            
            /* funcode and slaveid same, this frame for us and then excute it*/
            if(ucFunctionCode & 0x80){ // 异常
                result = MBR_ERSPEXCEPTOIN;
                exception = (MbException_t)pRemainFrame[0]; //异常码
            }
            else{
                result = MBR_EINFUNCTION;
                handle = MbmFuncHandleSearch(ucFunctionCode);
                if(handle)
                    result = handle(&req->node->regs, req->regaddr, req->regcnt, pRemainFrame, usLength);  
            }
        }

        // response exception is not a error
        if(result != MBR_ENOERR && result != MBR_ERSPEXCEPTOIN){
            req->errcnt++;
        }
        
        if(req->node->cb)  
            req->node->cb(result, exception, req); //执行回调
        
        if(result == MBR_EINFUNCTION){ // 无此功能码
            MbmReqMsgDelete(req); // delete request
        }
        else {
            if((req->slaveaddr == MB_ADDRESS_BROADCAST) || (req->scanrate == 0)){// only once
                MbmReqMsgDelete(req); 
            }
            else{ // move to pend list
                msgQput(&(dev->Reqpendinghead), req);
            }                
        } 
        dev->Pollstate = MBM_DELYPOLL;   
        break;
    case MBM_RSPTIMEOUT:
        req = msgQpop(&(dev->Reqreadyhead));
        if(req == NULL) { /* some err happen ,then no request in list*/
            dev->Pollstate = MBM_DELYPOLL;
        }
        else{         
            result = MBR_ETIMEOUT;
            req->errcnt++;
            if(req->node->cb)
                req->node->cb(result, MB_EX_NONE, req); //执行回调
            
            if((req->slaveaddr == MB_ADDRESS_BROADCAST) || (req->scanrate == 0))// only once
                MbmReqMsgDelete(req);
            else{// move to pend list
                msgQput(&(dev->Reqpendinghead), req);
            }
            dev->Pollstate = MBM_XMIT;
        }
        break;
    case MBM_BROADCASTTURN:
    case MBM_DELYPOLL:
        break;
    case MBM_WAITRSP:  // send ok ? wait for server response
        req = msgQpeek(&(dev->Reqreadyhead));
        if(req == NULL) { /* some err happen ,then no request in list*/
            dev->Pollstate = MBM_DELYPOLL;
        }
        else if(req->slaveaddr == MB_ADDRESS_BROADCAST){ // broadcast ,remove from pend list soon  
            msgQpop(&(dev->Reqreadyhead));
            MbmReqMsgDelete(req);
            dev->Pollstate = MBM_BROADCASTTURN;
        }
        else{
            /* keep wait for responese */
        }
        break;        
    default:
        dev->Pollstate = MBM_IDLE;
    }
    
    if(timediff){
        
        switch (dev->Pollstate){
        case MBM_BROADCASTTURN: /* 广播转换延迟时间 ,发出广播后给节点处理的时间*/
            dev->Broadcastturntimecnt += timediff;
            if(dev->Broadcastturntimecnt >= dev->Broadcastturntime){
                dev->Broadcastturntimecnt = 0;                
                dev->Pollstate = MBM_XMIT;
            }
            break;
            
        case MBM_DELYPOLL: /* 两个请求之间的延迟时间, 请求失败或重试时的延迟*/
            dev->Delaypolltimecnt += timediff;
            if(dev->Delaypolltimecnt >= dev->Delaypolltime){
                dev->Delaypolltimecnt = 0;                
                dev->Pollstate = MBM_XMIT;
            }
            break;
            
        case MBM_WAITRSP: /* 等待应答超时时间 */
            dev->Replytimeoutcnt += timediff;
            if(dev->Replytimeoutcnt >= (dev->Replytimeout + dev->XmitingTime)){
                dev->Replytimeoutcnt = 0;
                dev->Pollstate = MBM_RSPTIMEOUT;
            }
            break;
            
        default:
            break;
        }

        __MbmReqPendlistScan(dev, timediff);// scan pend list 
    }
    
    return MB_ENOERR;
}


/*扫描挂起列表         发现有准备好的请求 移入就绪列表      */
static void __MbmReqPendlistScan(MbmDev_t *dev, uint32_t diff)
{
    MbmReq_t *prevReq = NULL;
    MbmReq_t *srchReq;

    msgQ_for_each_msg(&(dev->Reqpendinghead), srchReq){
        srchReq->scancnt += diff;
        if(srchReq->scancnt >= srchReq->scanrate){// scan timeout move to ready list

            srchReq->scancnt = 0; // clear scan count
            //first remove from pend list
            msgQextract(&(dev->Reqpendinghead), srchReq, prevReq);
            // then add to read list tail
            msgQput(&(dev->Reqreadyhead) , srchReq);
        }
        else{
            prevReq = srchReq;
        }        
    } 
}

/*ok */
static MbmDev_t *__MbmdevSearch(uint8_t port)
{
    MbmDev_t *srh = NULL;
    
    msgQ_for_each_msg(&mbm_dev_head, srh){
        if(srh->port == port)
            break;
    }
    
    return srh;
}


#endif

