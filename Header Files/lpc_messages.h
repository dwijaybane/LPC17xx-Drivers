/**********************************************************************
* $Id$		lpc_messages.h
*//**
* @file		lpc_messages.h
* @brief	Contains all definitions and function prototypes
* 			support for Message Functions
* @version	1.0
* @date		13. June. 2014
* @author	Dwijay.Edutech Learning Solutions
***********************************************************************
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* products. This software is supplied "AS IS" without any warranties.
* NXP Semiconductors assumes no responsibility or liability for the
* use of the software, conveys no license or title under any patent,
* copyright, or mask work right to the product. NXP Semiconductors
* reserves the right to make changes in the software without
* notification. NXP Semiconductors also make no representation or
* warranty that such application will be suitable for the specified
* use without further testing or modification.
* Permission to use, copy, modify, and distribute this software and its
* documentation is hereby granted, under NXP Semiconductors'
* relevant copyright in the software, without fee, provided that it
* is used in conjunction with NXP Semiconductors microcontrollers.  This
* copyright, permission, and disclaimer notice must appear in all copies of
* this code.
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
