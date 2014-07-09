/******************************************************************//**
* @file		lpc_utility.h
* @brief	Contains all macro definitions and function prototypes
* 			support for Utility Functions
* @version	1.0
* @date		13. June. 2014
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup UTILITY
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC_UTILITY_H
#define __LPC_UTILITY_H

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup UART_Public_Macros
 * @{
 */


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup UART_Public_Functions UART Public Functions
 * @{
 */

uint32_t getAtoI(LPC_UART_TypeDef *UARTx, uint8_t digit);
uint32_t get_hex(LPC_UART_TypeDef *UARTx, uint8_t digit);

/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC_UTILITY_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
