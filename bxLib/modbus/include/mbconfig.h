
#ifndef __MB_CONFIG_H
#define __MB_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/********************* common defined  ************************/
#define MB_MASTER_ENABLED        ( 0 )
#define MB_SLAVE_ENABLED         ( 1 )

/*! \defgroup modbus_cfg Modbus Configuration
 */
/*! \brief If Modbus RTU support is enabled. */
#define MB_RTU_ENABLED                          (  1 )
/*! \brief If Modbus ASCII support is enabled. */
#define MB_ASCII_ENABLED                        (  0 )
/*! \brief If Modbus TCP support is enabled. */
#define MB_TCP_ENABLED                          (  0 )

/********************for master defined***************************************/
/*! \brief Maximum number of Modbus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported Modbus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
#define MBM_PARSE_RSP_HANDLERS_MAX               ( 10 )

/*! \brief Number of bytes which should be allocated for the <em>Report Slave ID
 *    </em>command.
 *
 * This number limits the maximum size of the additional segment in the
 * report slave id function. See MbsSetSlaveID(  ) for more information on
 * how to set this value. It is only used if MBS_FUNC_OTHER_REP_SLAVEID_ENABLED
 * is set to <code>1</code>.
 */
#define MBM_PARSE_RSP_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MBM_PARSE_RSP_OTHER_REP_SLAVEID_ENABLED       (  1 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define MBM_PARSE_RSP_READ_HOLDING_ENABLED            (  1 )
/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define MBM_PARSE_RSP_WRITE_HOLDING_ENABLED           (  1 )
/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define MBM_PARSE_RSP_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MBM_PARSE_RSP_READWRITE_HOLDING_ENABLED       (  1 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define MBM_PARSE_RSP_READ_INPUT_ENABLED              (  1 )
/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define MBM_PARSE_RSP_READ_COILS_ENABLED              (  1 )
/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define MBM_PARSE_RSP_WRITE_COIL_ENABLED              (  1 )
/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define MBM_PARSE_RSP_WRITE_MULTIPLE_COILS_ENABLED    (  1 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MBM_PARSE_RSP_READ_DISCRETE_INPUTS_ENABLED    (  1 )


/********************for slave defined***************************************/
/*! \brief the number of support multiple slaves  range(1 - 16 )*/
#define MBS_SUPPORT_MULTIPLE_NUMBER              ( 1 )

/*! \brief The character timeout value for Modbus ASCII.
 *
 * The character timeout value is not fixed for Modbus ASCII and is therefore
 * a configuration option. It should be set to the maximum expected delay
 * time of the network.
 */
#define MBS_ASCII_TIMEOUT_SEC                    (  1 )
/*! \brief Timeout to wait in ASCII prior to enabling transmitter.
 *
 * If defined the function calls MbPortSerialDelay with the argument
 * MBS_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS to allow for a delay before
 * the serial transmitter is enabled. This is required because some
 * targets are so fast that there is no time between receiving and
 * transmitting the frame. If the master is to slow with enabling its 
 * receiver then he will not receive the response correctly.
 */
#ifndef MBS_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS
#define MBS_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS    ( 0 )
#endif
/*! \brief Maximum number of Modbus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported Modbus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
#define MBS_FUNC_HANDLERS_MAX                    ( 10 )
/*! \brief Number of bytes which should be allocated for the <em>Report Slave ID
 *    </em>command.
 *
 * This number limits the maximum size of the additional segment in the
 * report slave id function. See MbsSetSlaveID(  ) for more information on
 * how to set this value. It is only used if MBS_FUNC_OTHER_REP_SLAVEID_ENABLED
 * is set to <code>1</code>.
 */
#define MBS_FUNC_OTHER_REP_SLAVEID_BUF           ( 32 )

/*! \brief If the <em>Report Slave ID</em> function should be enabled. */
#define MBS_FUNC_OTHER_REP_SLAVEID_ENABLED       (  0 )

/*! \brief If the <em>Read Holding Registers</em> function should be enabled. */
#define MBS_FUNC_READ_HOLDING_ENABLED            (  1 )
/*! \brief If the <em>Write Single Register</em> function should be enabled. */
#define MBS_FUNC_WRITE_HOLDING_ENABLED           (  1 )
/*! \brief If the <em>Write Multiple registers</em> function should be enabled. */
#define MBS_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED  (  1 )
/*! \brief If the <em>Read/Write Multiple Registers</em> function should be enabled. */
#define MBS_FUNC_READWRITE_HOLDING_ENABLED       (  1 )

/*! \brief If the <em>Read Input Registers</em> function should be enabled. */
#define MBS_FUNC_READ_INPUT_ENABLED              (  0  )
/*! \brief If the <em>Read Coils</em> function should be enabled. */
#define MBS_FUNC_READ_COILS_ENABLED              (  0 )
/*! \brief If the <em>Write Coils</em> function should be enabled. */
#define MBS_FUNC_WRITE_COIL_ENABLED              (  0 )
/*! \brief If the <em>Write Multiple Coils</em> function should be enabled. */
#define MBS_FUNC_WRITE_MULTIPLE_COILS_ENABLED    (  0 )

/*! \brief If the <em>Read Discrete Inputs</em> function should be enabled. */
#define MBS_FUNC_READ_DISCRETE_INPUTS_ENABLED    (  0 )

#include "log.h"

#define mblogln(fmt, ...) //mo_logln(DEBUG, fmt, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif
#endif
