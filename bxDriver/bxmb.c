
#include "bxmisc.h"
#include "bxmb.h"


#define REG_HOLDING_NREGS 10

Mbshandle_t device0;
__align(2) uint16_t dev0HoldingBuf[REG_HOLDING_NREGS] = {0x1111,0x2222,0x3333};



void bxmbInit(void)
{
    MbErrorCode_t status;
    uint8_t slaveid = 0x01;
    
    //slaveid = bxMiscGetSlaveID();
    
    log_debugln("modbus slave id: 0x%2x", slaveid);
    
    device0 = MbsNew(MB_RTU, slaveid, COM1, 115200, MB_PAR_NONE);
    status = MbsRegAssign(device0, (uint8_t *)dev0HoldingBuf, sizeof(dev0HoldingBuf), 0, REG_HOLDING_NREGS, 0, 0, 0, 0, 0,0);
    if(status == MB_ENOERR){
        MbsStart(device0);
        log_alertln("modbus is started!");
    }
}


