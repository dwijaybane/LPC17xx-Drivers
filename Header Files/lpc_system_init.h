/******************************************************************//**
* @file		lpc_system_init.h
* @brief	Contains all System And Port Initialization code 
* @version	1.0
* @date		24. July. 2013
* @author	Dwijay.Edutech Learning Solutions
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
 *  Stepper Motor
 */
extern uchar SmAntClk[4];
extern uchar SmClk[4];
extern uchar temp;

/**
 * @}
 */

/**
 *  ADC
 */
extern uint32_t adc_value;

/**
 * @}
 */


/**
 *  Timer3
 */
extern uint32_t T1, T2;
extern BOOL_8 toggle_tim3;

/**
 * @}
 */


/**
 *  Timer1
 */
extern BOOL_8 first_capture,done;
extern uint32_t capture;
extern uint8_t count;

/**
 * @}
 */


/**
 *  vibra sense
 */
extern BOOL_8 vibration;

/**
 * @}
 */






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
