
#include "bxmisc.h"

static uint8_t BxGPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin)
{
    if ((LL_GPIO_ReadInputPort(GPIOx) & GPIO_Pin) != (uint32_t)0){
        return 0x01;
    }
    
    return 0x00;
}

uint8_t bxMiscGetSlaveID(void)
{
    uint8_t value = BxGPIO_ReadInputDataBit(DAT6_GPIO_Port, BX_HAL_DATA6_PIN) << 6;
    value |= BxGPIO_ReadInputDataBit(DAT5_GPIO_Port, BX_HAL_DATA5_PIN) << 5;
    value |= BxGPIO_ReadInputDataBit(DAT4_GPIO_Port, BX_HAL_DATA4_PIN) << 4;
    value |= BxGPIO_ReadInputDataBit(DAT3_GPIO_Port, BX_HAL_DATA3_PIN) << 3;
    value |= BxGPIO_ReadInputDataBit(DAT2_GPIO_Port, BX_HAL_DATA2_PIN) << 2;
    value |= BxGPIO_ReadInputDataBit(DAT1_GPIO_Port, BX_HAL_DATA1_PIN) << 1;
    value |= BxGPIO_ReadInputDataBit(DAT0_GPIO_Port, BX_HAL_DATA0_PIN) << 0;

    return value;
}
