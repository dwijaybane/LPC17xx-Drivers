/******************************************************************//**
* @file		lpc_switch.c
* @brief	Contains all functions support for Switch library
* 			on LPC17xx
* @version	1.0
* @date		15. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
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

