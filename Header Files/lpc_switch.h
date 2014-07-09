/******************************************************************//**
* @file		lpc_switch.h
* @brief	Contains all Switch Initialization code
* @version	1.0
* @date		15. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup SWITCH
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SWITCH_H_
#define LPC_SWITCH_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Functions ----------------------------------------------------------- */
/** @defgroup SWITCH_Public_Functions SWITCH Public Functions
 * @{
 */

uchar Detect_Key (void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SWITCH_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
