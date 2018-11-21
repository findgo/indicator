
#include "hal_spi.h"

void halSPI1_Init(uint16_t baud)
{
    // see stm32CubeMX
    MX_SPI2_Init();
}

uint8_t halSPI1_ReadWrite(uint8_t writedat)
{
	/* Loop while DR register in not emplty */
	while(!LL_SPI_IsActiveFlag_TXE(SPI1));

	/* Send byte through the SPI1 peripheral */
    LL_SPI_TransmitData8(SPI1, writedat);
    
#if !defined(HAL_SPI1_THREE_LINE)	
	/* Wait to receive a byte */
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1));
	
	/* Return the byte read from the SPI bus */
	return LL_SPI_ReceiveData8(SPI1);
#else
    return 0;
#endif
}

void halSPI2_Init(uint16_t baud)
{
    // see stm32CubeMX
}
uint8_t halSPI2_ReadWrite(uint8_t writedat)
{
	/* Loop while DR register in not emplty */
	while(!LL_SPI_IsActiveFlag_TXE(SPI2));

	/* Send byte through the SPI1 peripheral */
    LL_SPI_TransmitData8((SPI2), writedat);
    
#if !defined(HAL_SPI2_THREE_LINE)	
	/* Wait to receive a byte */
	while(!LL_SPI_IsActiveFlag_RXNE((SPI2)));
	
	/* Return the byte read from the SPI bus */
	return LL_SPI_ReceiveData8((SPI2));
#else
    return 0;
#endif
}	

/************************************************** end of file ***********************************************************/

