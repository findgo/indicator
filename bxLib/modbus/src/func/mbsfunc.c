#include "mbfunc.h"

#if MB_SLAVE_ENABLED > 0

typedef struct
{
    uint8_t ucFunctionCode;
    pMbsFunctionHandler pxHandler;
} MbsFunctionHandler;

/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static MbsFunctionHandler xFuncHandlers[MBS_FUNC_HANDLERS_MAX] = {

#if MBS_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, MbsFuncReportSlaveID},
#endif

#if MBS_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, MbsFuncRdHoldingRegister},
#endif

#if MBS_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, MbsFuncWrHoldingRegister},
#endif

#if MBS_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, MbsFuncWrMulHoldingRegister},
#endif

#if MBS_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, MbsFuncRdWrMulHoldingRegister},
#endif

#if MBS_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, MbsFuncRdInputRegister},
#endif

#if MBS_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, MbsFuncRdCoils},
#endif

#if MBS_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, MbsFuncWrCoil},
#endif

#if MBS_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, MbsFuncWrMulCoils},
#endif

#if MBS_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, MbsFuncRdDiscreteInputs},
#endif
};
/*********************************************************************
 * @brief   register funcction code handle   
 * @param   ucFunctionCode - 功能码
 * @param   pxHandler - 功能码对应的回调函数, NULL: 为注销对应功能码回调
 * @return  
 */
MbErrorCode_t MbsRegisterHandleCB( uint8_t ucFunctionCode, pMbsFunctionHandler pxHandler )
{
    int i;
    MbErrorCode_t eStatus = MB_ENORES;

    if((ucFunctionCode < MB_FUNC_MIN) || (ucFunctionCode > MB_FUNC_MAX))
        return MB_EINVAL;

    for( i = 0; i < MBS_FUNC_HANDLERS_MAX; i++ ){
        if((xFuncHandlers[i].ucFunctionCode == 0) || (xFuncHandlers[i].ucFunctionCode == ucFunctionCode)){ 
            // pxHandler != NULL register,  NULL is unregister
            xFuncHandlers[i].ucFunctionCode = pxHandler ? ucFunctionCode : 0;
            xFuncHandlers[i].pxHandler = pxHandler;
            
            eStatus = MB_ENOERR;
            break;
        }
    }
    if(!pxHandler) // remove can't failed!
        eStatus = MB_ENOERR;
    
    return eStatus;
}

/*
* @brief search function handle with function code  
* @param   ucFunctionCode - 功能码
* @param   pxHandler - 功能码对应的回调函数, NULL: 为注销对应功能码回调
* @return  function handle point, if not exist return NULL
*/
pMbsFunctionHandler MbsFuncHandleSearch(uint8_t ucFunctionCode)
{
    int i;
    pMbsFunctionHandler srch = NULL;

    for( i = 0; i < MBS_FUNC_HANDLERS_MAX; i++){
        /* No more function handlers registered. Abort. */
        if( xFuncHandlers[i].ucFunctionCode == 0 ){
            break;
        }
        else if(xFuncHandlers[i].ucFunctionCode == ucFunctionCode){
            srch = xFuncHandlers[i].pxHandler;
        }
    }

    return srch;  
}
#endif

