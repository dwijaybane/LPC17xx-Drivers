/**********************************************************************
* $Id$		lpc_system_init.h
*//**
* @file		lpc_system_init.h
* @brief	Contains all System And Port Initialization code 
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
/** @defgroup SYSTEM_INIT SYSTEM_INIT
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SYSTEM_INIT_H_
#define LPC_SYSTEM_INIT_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc17xx_nvic.h"
#include "lpc17xx_pinsel.h"
#include "lpc17xx_clkpwr.h"

/* Peripherals Include----------------------------------------------------------*/
#include "lpc17xx_systick.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_wdt.h"
#include "lpc17xx_uart.h"
#include "lpc17xx_ssp.h"
#include "lpc17xx_i2c.h"
#include "lpc_i2c_tsc2004.h"
#include "lpc_ssp_glcd.h"


#ifdef __cplusplus
extern "C"
{
#endif

/** @addtogroup GLOBAL_Variables
 * @{
 */

/************************** GLOBAL VARIABLES *************************/
/**
 *  Systick
 */
extern uint32_t led_delay;
extern uchar status;

/**
 *  Lcd Custom Patterns
 */
//extern uchar Character1[8];






/**
 * @}
 */

/* Public Functions ----------------------------------------------------------- */
/** @defgroup SYSTEM_INIT_Public_Functions SYSTEM_INIT Public Functions
 * @{
 */

void System_Init(void);
void Port_Init(void);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SYSTEM_INIT_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
