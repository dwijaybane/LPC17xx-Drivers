/******************************************************************//**
* @file		lpc_messages.h
* @brief	Contains all definitions and function prototypes
* 			support for Message Functions
* @version	1.0
* @date		13. June. 2014
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup MSG
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef __LPC_MESSAGES_H
#define __LPC_MESSAGES_H

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_uart.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Constants --------------------------------------------------------------- */
/** @defgroup MSG_Constants
 * @{
 */




/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup MSG_Public_Functions MSG Public Functions
 * @{
 */

void Print_Main_Menu(void);
void Print_Buzzer_Menu(void);





/**
 * @}
 */


#ifdef __cplusplus
}
#endif


#endif /* __LPC_MESSAGES_H */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
