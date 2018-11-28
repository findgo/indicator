
#include "mcoils.h"

/* 用于记录coil当前的工作状态 */
static uint8_t CurCoilOnOffStatus;
static void mCoilsOnOff(uint8_t Coils, uint8_t val);
/**
  * @brief  初始化
  * @param  none
  * @note   
  * @retval None
  */
void mCoilsInit(void)
{
    mCoilsOnOff(MCOILS_ALL, MCOILS_MODE_OFF);
}

/**
  * @brief     
  * @param  Coils --which coils, mode---模式
  * @note   
  * @retval None
  */
void mCoilsSet (uint8_t Coils, mCoils_Mode_t mode)
{
    uint8_t coil;

    // on or off 
    if( mode < MCOILS_MODE_TOGGLE ) {
        mCoilsOnOff (Coils, mode);
        return;
    }

    // only toggle
    coil = MCOILS_1;
    Coils &= MCOILS_ALL;
    
    while (Coils)
    {
        if (Coils & coil){
            mCoilsOnOff (coil, ( CurCoilOnOffStatus & coil ) ? MCOILS_MODE_OFF : MCOILS_MODE_ON);
            Coils ^= coil;
        }
        coil <<= 1;
    }
}
/**
  * @brief  
  * @param  which -- which coils
  * @note   
  * @retval None
  */
uint8_t mCoilsGetStasus(uint8_t which)
{
    return (CurCoilOnOffStatus & which);
}
/**
  * @brief      Coils开关控制
  * @param  Coils --哪个coils, val--- on or off
  * @note   
  * @retval None
  */
static void mCoilsOnOff(uint8_t Coils, uint8_t val)
{
#if (mCOILSNUM >= 1)
    if (Coils & MCOILS_1){
        if(val){
            MTURN_COILS1_ON();
        }
        else{
            MTURN_COILS1_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 2)
    if (Coils & MCOILS_2){
        if(val){
            MTURN_COILS2_ON();
        }
        else{
            MTURN_COILS2_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 3)
    if (Coils & MCOILS_3){
        if(val){
            MTURN_COILS3_ON();
        }
        else{
            MTURN_COILS3_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 4)
    if (Coils & MCOILS_4){
        if(val){
            MTURN_COILS4_ON();
        }
        else{
            MTURN_COILS4_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 5)
    if (Coils & MCOILS_5){
        if(val){
            MTURN_COILS5_ON();
        }
        else{
            MTURN_COILS5_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 6)
    if (Coils & MCOILS_6){
        if(val){
            MTURN_COILS6_ON();
        }
        else{
            MTURN_COILS6_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 7)
    if (Coils & MCOILS_7){
        if(val){
            MTURN_COILS7_ON();
        }
        else{
            MTURN_COILS7_OFF();
        }
    }
#endif
#if (mCOILSNUM >= 8)    
    if (Coils & MCOILS_8){
        if(val){
            MTURN_COILS8_ON();
        }
        else{
            MTURN_COILS8_OFF();
        }
    }
#endif

    if(val){
        CurCoilOnOffStatus |= Coils;
    }
    else{
        CurCoilOnOffStatus &= (Coils ^ 0xFF);
    }
}


