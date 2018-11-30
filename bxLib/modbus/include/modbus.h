
#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "mb.h"


#if MB_RTU_ENABLED > 0 ||  MB_ASCII_ENABLED > 0
//public
uint32_t MbRegBufSizeCal(     uint16_t reg_holding_num, uint16_t reg_input_num, uint16_t reg_coils_num, uint16_t reg_discrete_num);

#define MbGetRegsHoldPtr(pReg)       ((uint16_t *)(((MbReg_t *)(pReg))->pReghold))
#define MbGetRegsInputPtr(pReg)      ((uint16_t *)(((MbReg_t *)(pReg))->pReginput))
#define MbGetRegsCoilPtr(pReg)       ((uint8_t *)(((MbReg_t *)(pReg))->pRegCoil))
#define MbGetRegsDiscPtr(pReg)       ((uint8_t *)(((MbReg_t *)(pReg))->pRegDisc))

#define MbGetResgHoldNum(pReg)       (((MbReg_t *)(pReg))->reg_holding_num)
#define MbGetRegsInputNum(pReg)      (((MbReg_t *)(pReg))->reg_input_num)
#define MbGetRegsCoilNum(pReg)       (((MbReg_t *)(pReg))->reg_coils_num)
#define MbGetRegsDiscNum(pReg)       (((MbReg_t *)(pReg))->reg_discrete_num)

#if MB_MASTER_ENABLED
MbErrorCode_t MbmRegisterParseHandleCB( uint8_t ucFunctionCode, pMbmParseRspHandler pxHandler );

/* TODO implement modbus master */
Mbmhandle_t MbmNew(MbMode_t eMode, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity);
void MbmFree(uint8_t ucPort);
MbErrorCode_t MbmSetPara(Mbmhandle_t dev, uint8_t retry,uint32_t replytimeout,
                                    uint32_t delaypolltime, uint32_t broadcastturntime);
MbmNode_t *MbmNodeNew(uint8_t slaveaddr,
                                uint16_t reg_holding_addr_start, uint16_t reg_holding_num,
                                uint16_t reg_input_addr_start, uint16_t reg_input_num,
                                uint16_t reg_coils_addr_start, uint16_t reg_coils_num,
                                uint16_t reg_discrete_addr_start, uint16_t reg_discrete_num);
void MbmNodeFree(MbmNode_t *node);
void MbmNodeCallBackAssign(MbmNode_t *node, pfnReqResultCB cb, void *arg);
MbErrorCode_t MbmAddNode(Mbmhandle_t dev, MbmNode_t *node);
MbErrorCode_t MbmRemoveNode(Mbmhandle_t dev, uint8_t slaveaddr);
MbmNode_t *MbmSearchNode(Mbmhandle_t dev,uint8_t slaveaddr);
MbErrorCode_t MbmStart(Mbmhandle_t dev);
MbErrorCode_t MbmStop(Mbmhandle_t dev);
MbErrorCode_t MbmClose(Mbmhandle_t dev);
void MbmPoll(void);

/* for bits */
/* for request */
MbReqResult_t MbmReqRdCoils(Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Coilcnt, uint16_t scanrate);
MbReqResult_t MbmReqWrCoil(Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegAddr, uint16_t val);
MbReqResult_t MbmReqWrMulCoils(Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Coilcnt,
                                        uint8_t *valbuf, uint16_t valcnt);
MbReqResult_t MbmReqRdDiscreteInputs(Mbmhandle_t dev, uint8_t slaveaddr, 
                                        uint16_t RegStartAddr, uint16_t Discnt, uint16_t scanrate);

/* for register */                                    
/* for request */
MbReqResult_t MbmReqRdHoldingRegister(Mbmhandle_t dev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt, uint16_t scanrate);
MbReqResult_t MbmReqWrHoldingRegister(Mbmhandle_t dev, uint8_t slaveaddr, 
                                                uint16_t RegAddr, uint16_t val);
MbReqResult_t MbmReqWrMulHoldingRegister(Mbmhandle_t dev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt,
                                                uint16_t *valbuf, uint16_t valcnt);
MbReqResult_t MbmReqRdInputRegister(Mbmhandle_t dev, uint8_t slaveaddr, 
                                                uint16_t RegStartAddr, uint16_t Regcnt, uint16_t scanrate);

MbReqResult_t MbmReqRdWrMulHoldingRegister(Mbmhandle_t dev, uint8_t slaveaddr, 
                                                uint16_t RegReadStartAddr, uint16_t RegReadCnt,
                                                uint16_t RegWriteStartAddr, uint16_t RegWriteCnt,
                                                uint16_t *valbuf, uint16_t valcnt);
#define MbmGetReqSlaveID(pReq)  (((MbmReq_t *)(pReq))->slaveaddr)
#define MbmGetReqFunCode(pReq)  (((MbmReq_t *)(pReq))->funcode)
#define MbmGetReqErrorCnt(pReq) (((MbmReq_t *)(pReq))->errcnt)
#define MbmGetReqRegAddr(pReq)  (((MbmReq_t *)(pReq))->regaddr)
#define MbmGetReqRegCnt(pReq)   (((MbmReq_t *)(pReq))->regcnt)
#define MbmGetNodePtr(pReq)     (((MbmReq_t *)(pReq))->node)
#define MbmGetArgPtr(pReq)      (((MbmReq_t *)(pReq))->node->arg)

#define MbmGetSlaveID(pNode)    (((MbmNode_t *)(pNode))->slaveaddr)
#define MbmGetRegsPtr(pNode)    (&(((MbmNode_t *)(pNode))->regs))

#endif

#if MB_SLAVE_ENABLED > 0

#define MbsGetRegsPtr(dev)  (&(((MbsDev_t *)(dev))->regs))
// for slave ,get register start address
#define MbsGetRegsHoldPtr(dev)      ((uint16_t *)(((MbsDev_t *)(dev))->regs.pReghold))
#define MbsGetRegsInputPtr(dev)     ((uint16_t *)(((MbsDev_t *)(dev))->regs.pReginput))
#define MbsGetRegsCoilPtr(dev)      ((uint8_t *)(((MbsDev_t *)(dev))->regs.pRegCoil))
#define MbsGetRegsDiscPtr(dev)      ((uint8_t *)(((MbsDev_t *)(dev))->regs.pRegDisc))
// for slave ,get register number
#define MbsGetRegsHoldNum(dev)      (((MbsDev_t *)(dev))->regs.reg_holding_num)
#define MbsGetRegsInputNum(dev)     (((MbsDev_t *)(dev))->regs.reg_input_num)
#define MbsGetRegsCoilNum(dev)      (((MbsDev_t *)(dev))->regs.reg_coils_num)
#define MbsGetRegsDiscNum(dev)      (((MbsDev_t *)(dev))->regs.reg_discrete_num)


MbErrorCode_t MbsSetSlaveID(MbReg_t *regs, uint8_t ucSlaveID, uint8_t xIsRunning,
                                        uint8_t const *pucAdditional, uint16_t usAdditionalLen );
/*********************************************************************
 * @brief   register funcction code handle   
 * @param   ucFunctionCode - 功能码
 * @param   pxHandler - 功能码对应的回调函数, NULL: 为注销对应功能码回调
 * @return  
 */
MbErrorCode_t MbsRegisterHandleCB(uint8_t ucFunctionCode, pMbsFunctionHandler pxHandler);
/*********************************************************************
 * @brief   create new slave modbus device  
 * @param   eMode - MB_RTU or MB_ASCII
 * @param   ucSlaveAddress - slave id
 * @param   ucPort - use which usart port 
 * @param   ulBaudRate - bandrate
 * @param   eParity - Parity used for characters in serial mode
 * @param   Mbshandle_t - slave device handle ,if failed return NULL 句柄
 * @return  
 */
Mbshandle_t MbsNew(MbMode_t eMode, uint8_t ucSlaveAddress, uint8_t ucPort, uint32_t ulBaudRate, MbParity_t eParity);
/*********************************************************************
 * @brief   free the slave modbus device with port
 * @param   ucPort - use which usart port 
 * @return  
 */
void MbsFree(uint8_t ucPort);
/*********************************************************************
 * @brief   assign buffer for register
 * @return  
 */
MbErrorCode_t MbsRegAssign(Mbshandle_t dev,
                                uint8_t *regstoragebuf,  uint32_t regstoragesize, 
                                uint16_t reg_holding_addr_start, uint16_t reg_holding_num,
                                uint16_t reg_input_addr_start, uint16_t reg_input_num,
                                uint16_t reg_coils_addr_start, uint16_t reg_coils_num,
                                uint16_t reg_discrete_addr_start, uint16_t reg_discrete_num);
MbErrorCode_t MbsRegAssignSingle(Mbshandle_t dev,
                                uint16_t *reg_holdingbuf, uint16_t reg_holding_addr_start, uint16_t reg_holding_num,
                                uint16_t *reg_inputbuf, uint16_t reg_input_addr_start,uint16_t reg_input_num,
                                uint8_t *reg_coilsbuf, uint16_t reg_coils_addr_start,uint16_t reg_coils_num,
                                uint8_t *reg_discretebuf, uint16_t reg_discrete_addr_start, uint16_t reg_discrete_num);
/*********************************************************************
 * @brief   start the slave device
 * @param   dev - the handle of the slave device
 * @return  
 */
MbErrorCode_t MbsStart(Mbshandle_t dev);
/*********************************************************************
 * @brief   stop the slave device
 * @param   dev - the handle of the slave device
 * @return  
 */
MbErrorCode_t MbsStop(Mbshandle_t dev);
/*********************************************************************
 * @brief   close the slave device, before it,muse be use MbsStop first
 * @param   dev - the handle of the slave device
 * @return  
 */
MbErrorCode_t MbsClose(Mbshandle_t dev);
// 
void MbsPoll(void);

#endif

#endif

#endif

