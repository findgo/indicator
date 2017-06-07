
#ifndef _HAL_RF_SETTING_H_
#define _HAL_RF_SETTING_H_

#include "rf.h"
/*100kBaud,47kHz,GFSK,325kHz,sensitivity*/
static uint8_t PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm

static HAL_RF_CONFIG rfSettings =
{
#if 1
0x08,	//FSCTRL1
0x00,	//FSCTRL0 
/* data rate: 100K */
0x10,	//FREQ2
0xA7,	//FREQ1
0x62,	//FREQ0
/* 432.999817 MHz */
0x5B,	// MDMCFG4	 Modem configuration.
0xF8,	// MDMCFG3	 Modem configuration.
0x13,	// MDMCFG2	 Modem configuration.
0x22,	// MDMCFG1	 Modem configuration.
0xF8,	// MDMCFG0	 Modem configuration.


0x00,	// CHANNR	 Channel number.
0x47,	// DEVIATN	 Modem deviation setting (when FSK modulation is enabled).
0xB6,	// FREND1	 Front end RX configuration.
0x10,	// FREND0	 Front end RX configuration.

0x18,	// MCSM0	 Main Radio Control State Machine configuration.
0x1D,	// FOCCFG	 Frequency Offset Compensation Configuration.
0x1C,	// BSCFG	 Bit synchronization Configuration.
0xC7,	// AGCCTRL2  AGC control.
0x00,	// AGCCTRL1  AGC control.
0xB2,	// AGCCTRL0  AGC control.


0xEA,	// FSCAL3	 Frequency synthesizer calibration.
0x2A,	// FSCAL2	 Frequency synthesizer calibration.
0x00,	// FSCAL1	 Frequency synthesizer calibration.
0x1F,	// FSCAL0	 Frequency synthesizer calibration.
0x59,	// FSTEST	 Frequency synthesizer calibration.
0x81,	// TEST2	 Various test settings.
0x35,	// TEST1	 Various test settings.
0x09,	// TEST0	 Various test settings.

0x06,	// IOCFG2	 GDO2 output pin configuration.
0x0b,	// IOCFG0D	 GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
0x04,	// PKTCTRL1  Packet automation control.
0x45,	// PKTCTRL0  Packet automation control.
0x00,	// ADDR 	 Device address.
0xff	// PKTLEN	 Packet length.���

#else
0x08,	// FSCTRL1	 Frequency synthesizer control.
0x00,	// FSCTRL0	 Frequency synthesizer control.

0x10,	// FREQ2	 Frequency control word, high byte.
0xA7,	// FREQ1	 Frequency control word, middle byte.
0x62,	// FREQ0	 Frequency control word, low byte.

0x5B,	// MDMCFG4	 Modem configuration.
0xF8,	// MDMCFG3	 Modem configuration.
0x03,	// MDMCFG2	 Modem configuration.          *******
0x22,	// MDMCFG1	 Modem configuration.
0xF8,	// MDMCFG0	 Modem configuration.

0x00,	// CHANNR	 Channel number.
0x47,	// DEVIATN	 Modem deviation setting (when FSK modulation is enabled).
0xB6,	// FREND1	 Front end RX configuration.
0x10,	// FREND0	 Front end RX configuration.

0x18,	// MCSM0	 Main Radio Control State Machine configuration.
0x1D,	// FOCCFG	 Frequency Offset Compensation Configuration.
0x1C,	// BSCFG	 Bit synchronization Configuration.
0xC7,	// AGCCTRL2  AGC control.
0x00,	// AGCCTRL1  AGC control.
0xB2,	// AGCCTRL0  AGC control.

0xEA,	// FSCAL3	 Frequency synthesizer calibration.
0x2A,	// FSCAL2	 Frequency synthesizer calibration.
0x00,	// FSCAL1	 Frequency synthesizer calibration.
0x11,	// FSCAL0	 Frequency synthesizer calibration.   *******
0x59,	// FSTEST	 Frequency synthesizer calibration.
0x81,	// TEST2	 Various test settings.
0x35,	// TEST1	 Various test settings.
0x09,	// TEST0	 Various test settings.

0x06,	// IOCFG2	 GDO2 output pin configuration.
0x0b,	// IOCFG0D	 GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.
0x04,	// PKTCTRL1  Packet automation control.
0x45,	// PKTCTRL0  Packet automation control.
0x00,	// ADDR 	 Device address.
0xff	// PKTLEN	 Packet length.���
#endif
};



#endif
