
/*
 * The general format of an FLOW message is:
 * <preamble1><preamble2>,<data length><command>,<data>,<CRC/FCS/SUM>
 * 
 * pdu = <command>,<data>
 *
 * Where:
 * preamble     = the ASCII characters '$M'
 * data length  = number of data bytes, binary. Can be zero as in the case of a data request to the master
 * data         = as per the table below. UINT8
 * CRC          = CRC:
*/

#ifndef __FLOW_DEF_H_
#define __FLOW_DEF_H_

#include "verify.h" 


#define FLOW_DF_HEAD_LEN               2  // double frame head len
#define FLOW_SF_HEAD_LEN               1  // single frame head len
#define FLOW_F_DATALEN_LEN            1  // frame data length len
#define FLOW_F_XOR_LEN                1  // XOR
#define FLOW_F_CRC_LEN                2  // CRC
                                  //ascii
#define FLOW_DF_PREAMBLE1       '$'  // 0x24
#define FLOW_DF_PREAMBLE2       'M'  // 0x4d

#define FLOW_SF_PREAMBLE        0xf0
#endif

