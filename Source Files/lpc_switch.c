/**********************************************************************
* $Id$		lpc_switch.c
*//**
* @file		lpc_switch.c
* @brief	Contains all functions support for Switch library
* 			on LPC17xx
* @version	1.0
* @date		15. Nov. 2013
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
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SWITCH
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"
#include "lpc_switch.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup SWITCH_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    Detects pressed Key
 * @param[in]	None
 * @return 		Key Value
 **********************************************************************/
uchar Detect_Key (void)
{
	uint32_t x;
	x = GPIO_ReadValue(0);    // Read Port0 Pin state and store in x
	if(!(x & _SBF(19,1)))      // check if Pin 0.19 is zero
	{
		return(1);
	}
	else if(!(x & _SBF(20,1))) // check if Pin 0.20 is zero
	{
		return(2);
	}
	else if(!(x & _SBF(21,1))) // check if Pin 0.21 is zero
	{
		return(3);
	}
	else if(!(x & _SBF(22,1))) // check if Pin 0.22 is zero
	{
		return(4);
	}

    return(0);
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

