/**********************************************************************
* $Id$		lpc_global.c
*//**
* @file		lpc_global.c
* @brief	Contains Global Variables of LPC17xx
* @version	1.0
* @date		24. July. 2013
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
/** @addtogroup GLOBAL
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_types.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup GLOBAL_Variables
 * @{
 */

/**
 *   Systick
 */
uint32_t led_delay;
uchar status = 0;

/**
 *   Lcd Custom Patterns
 */
//uchar Character1[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00};

/**
 *  Stepper Motor
 */
uchar SmAntClk[4]= {0x80,0x08,0x20,0x02};
uchar SmClk[4]= {0x02,0x20,0x08,0x80};
uchar temp =0;

/**
 *  Timer3
 */
uint32_t T1=300;    // Off Time
uint32_t T2=1000;   // On Time
BOOL_8 toggle_tim3=TRUE;

/**
 *  ADC
 */
uint32_t adc_value=0;

/**
 * @}
 */


/**
 *  Timer1
 */
BOOL_8 first_capture,done;
uint32_t capture;
uint8_t count=0;

/**
 * @}
 */

/**
 *  vibra sense
 */
BOOL_8 vibration = 0;

/**
 * @}
 */



/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

