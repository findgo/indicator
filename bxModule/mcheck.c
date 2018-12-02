
#include "mcheck.h"

#define CKS_STATE_WAIT_CHANGE	    0
#define CKS_STATE_CHANGE_FILTER	    1

typedef struct
{
	uint8_t state; 
	uint8_t filtercount;
	cksCallBackFunc_t pcallfuc;
}mcksCtl_t;

static uint8_t mcksInited = FALSE;
static uint16_t preCksSavestatus;  
static mcksCtl_t cksControl[mCKSNum];

/**
  * @brief	分配回调函数
  * @param	cks:  whick io 
  * @param	pcallback: callback function
  * @note	
  * @retval  None
  */
void mcksAssign(uint16_t ckss, cksCallBackFunc_t pcallback)
{
    uint16_t cks;
    uint8_t idx;
    
    if(pcallback == NULL)
        return;

    if(!mcksInited){
        preCksSavestatus = mcksGetLowStatus();
    }
    
    cks = MCKS_1;
    ckss &= MCKS_ALL;
    idx = 0;

    while (ckss)
    {
        if (ckss & cks){
            cksControl[idx].pcallfuc = pcallback;
            ckss ^= cks;
        }
        cks <<= 1;
    }
}


/**
  * @brief	check 更新
  * @param	ElapseTime 流逝时间
  * @note	下降沿和上升沿都有做滤波，滤波一样的
  * @retval  None
  */
void mcks_Task(void)
{
    uint16_t cks,ckss;
    mcksCtl_t *sts;
    uint16_t newStatus, status;
    uint16_t tmpbit;


    newStatus = mcksGetLowStatus();
    status = preCksSavestatus ^   newStatus; // if bit set, change happen
    
    cks = MCKS_1;
    ckss = MCKS_ALL;
    sts = &cksControl[0];
    while(ckss)
    {
        if (cks & ckss){ 
    		if (sts->state ==  CKS_STATE_WAIT_CHANGE){
                if( status & cks ) { // change happen?
        	        sts->filtercount = 0;
        	        sts->state = CKS_STATE_CHANGE_FILTER;
                }
            }           
    		else{ //CKS_STATE_CHANGE_FILTER
    			sts->filtercount++;
    			if(sts->filtercount >= MCKS_FILTERTIME){
                    if ((status & cks) && sts->pcallfuc) {
                        tmpbit = newStatus & cks;
                        sts->pcallfuc(cks, (tmpbit > 0) ? TRUE : FALSE );
                        preCksSavestatus = (preCksSavestatus &(~cks)) | tmpbit; // 修改对应到新值
                    }
                    sts->state = CKS_STATE_WAIT_CHANGE;
    			}
            }
            ckss ^= cks;//已处理完，对应位清零
        }
        cks <<= 1;  //下一个
        sts++;      //下一个
	 }
}

uint16_t mcksGetLowStatus(void)
{   
    uint16_t status = 0;

#if (mCKSNum >= 1)
    status |= MCKS1_LEVEL() << 0;
#endif

#if (mCKSNum >= 2)
    status |= MCKS2_LEVEL() << 1;
#endif

#if (mCKSNum >= 3) 
    status |= MCKS3_LEVEL() << 2;
#endif

#if (mCKSNum >= 4) 
    status |= MCKS4_LEVEL() << 3;
#endif

#if (mCKSNum >= 5) 
    status |= MCKS5_LEVEL() << 4;
#endif

#if (mCKSNum >= 6) 
    status |= MCKS6_LEVEL() << 5;
#endif

#if (mCKSNum >= 7) 
    status |= MCKS7_LEVEL() << 6;

#endif

#if (mCKSNum >= 8)
    status |= MCKS8_LEVEL() << 7;
#endif

#if (mCKSNum >= 9)
        status |= MCKS9_LEVEL() << 8;
#endif
    
#if (mCKSNum >= 10)
        status |= MCKS10_LEVEL() << 9;
#endif
    
#if (mCKSNum >= 11) 
        status |= MCKS11_LEVEL() << 10;
#endif
    
#if (mCKSNum >= 12) 
        status |= MCKS12_LEVEL() << 11;
#endif
    
#if (mCKSNum >= 13) 
        status |= MCKS13_LEVEL() << 12;
#endif
    
#if (mCKSNum >= 14) 
        status |= MCKS14_LEVEL() << 13;
#endif
    
#if (mCKSNum >= 15) 
        status |= MCKS15_LEVEL() << 14;
#endif
    
#if (mCKSNum >= 16)
        status |= MCKS16_LEVEL() << 15;
#endif

    return status;
}



