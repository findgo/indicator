
#include "mbfunc.h"

#if MB_MASTER_ENABLED > 0

typedef struct
{
    uint8_t ucFunctionCode;
    pMbmParseRspHandler pxHandler;
} MbmParseRspHandler;


static MbmParseRspHandler parseRspHandlers[MBM_PARSE_RSP_HANDLERS_MAX] = {
#if MBM_PARSE_RSP_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, NULL},
#endif
#if MBM_PARSE_RSP_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, MbmParseRspRdHoldingRegister},
#endif
#if MBM_PARSE_RSP_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, MbmParseRspWrHoldingRegister},
#endif
#if MBM_PARSE_RSP_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, MbmParseRspWrMulHoldingRegister},
#endif
#if MBM_PARSE_RSP_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, MbmParseRspRdWrMulHoldingRegister},
#endif
#if MBM_PARSE_RSP_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, MbmParseRdInputRegister},
#endif
#if MBM_PARSE_RSP_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, MbmParseRspRdCoils},
#endif
#if MBM_PARSE_RSP_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, MbmParseRspWrCoil},
#endif
#if MBM_PARSE_RSP_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, MbmParseRspWrMulCoils},
#endif
#if MBM_PARSE_RSP_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, MbmParseRspRdDiscreteInputs},
#endif
};

/*********************************************************************
 * @brief 注册功能码回调函数  
 *
 * @param   ucFunctionCode - 功能码
 * @param   pxHandler - 功能码对应的回调函数, NULL: 为注销对应功能码回调
 *
 * @return  
 */
MbErrorCode_t MbmRegisterParseHandleCB( uint8_t ucFunctionCode, pMbmParseRspHandler pxHandler )
{
    int i;
    MbErrorCode_t eStatus = MB_ENORES;

    if((ucFunctionCode < MB_FUNC_MIN) || (ucFunctionCode > MB_FUNC_MAX))
        return MB_EINVAL;

    for( i = 0; i < MBS_FUNC_HANDLERS_MAX; i++ ){
        if((parseRspHandlers[i].ucFunctionCode == 0) || (parseRspHandlers[i].ucFunctionCode == ucFunctionCode)){ 
            // pxHandler != NULL register,  NULL is unregister
            parseRspHandlers[i].ucFunctionCode = pxHandler ? ucFunctionCode : 0;
            parseRspHandlers[i].pxHandler = pxHandler;
            
            eStatus = MB_ENOERR;
            break;
        }
    }
    if(!pxHandler) // remove can't failed!
        eStatus = MB_ENOERR;
    
    return eStatus;
}



// search function code handle
pMbmParseRspHandler MbmFuncHandleSearch(uint8_t ucFunctionCode)
{
    uint8_t i;

    for( i = 0; i < MBM_PARSE_RSP_HANDLERS_MAX; i++ ){
        /* No more function handlers registered. Abort. */
        if( parseRspHandlers[i].ucFunctionCode == 0 ){
            return NULL;
        }
        else if(parseRspHandlers[i].ucFunctionCode == ucFunctionCode){
            return (parseRspHandlers[i].pxHandler);
        }                
    }

    return NULL;
}

#endif

