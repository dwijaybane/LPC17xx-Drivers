/******************************************************************//**
* @file		lpc17xx_rit.h
* @brief	Contains all macro definitions and function prototypes
* 			support for RIT firmware library on LPC17xx
* @version	1.0
* @date		29. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup RIT RIT
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC17XX_RIT_H_
#define LPC17XX_RIT_H_

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Private Macros ------------------------------------------------------------- */
/** @defgroup RIT_Private_Macros RIT Private Macros
 * @{
 */

/* --------------------- BIT DEFINITIONS -------------------------------------- */
/*********************************************************************//**
 * Macro defines for RIT control register
 **********************************************************************/
/**	Set interrupt flag when the counter value equals the masked compare value */
#define RIT_CTRL_INTEN	((uint32_t) (1))
/** Set timer enable clear to 0 when the counter value equals the masked compare value  */
#define RIT_CTRL_ENCLR 	((uint32_t) _BIT(1))
/** Set timer enable on debug */
#define RIT_CTRL_ENBR	((uint32_t) _BIT(2))
/** Set timer enable */
#define RIT_CTRL_TEN	((uint32_t) _BIT(3))

/** Macro to determine if it is valid RIT peripheral */
#define PARAM_RITx(n)	(((uint32_t *)n)==((uint32_t *)LPC_RIT))

/**
 * @brief RIT Time type definitions
 */
typedef enum {
    RIT_MS      = (0),                      /*!< RIT value in millisecond Select */
    RIT_US                                  /*!< RIT value in microsecond Select */
} RIT_TIME_Type;

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup RIT_Public_Functions RIT Public Functions
 * @{
 */
/* RIT Init/DeInit functions */
void RIT_Config(LPC_RIT_TypeDef *RITx, uint32_t time_interval, RIT_TIME_Type psType);
void RIT_Init(LPC_RIT_TypeDef *RITx);
void RIT_DeInit(LPC_RIT_TypeDef *RITx);

/* RIT config timer functions */
void RIT_TimerConfig(LPC_RIT_TypeDef *RITx, uint32_t time_interval);
void RIT_TimerConfig_US(LPC_RIT_TypeDef *RITx, uint32_t time_interval);

/* Enable/Disable RIT functions */
void RIT_TimerClearCmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);
void RIT_Cmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);
void RIT_TimerDebugCmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState);

/* RIT Interrupt functions */
IntStatus RIT_GetIntStatus(LPC_RIT_TypeDef *RITx);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif /* LPC17XX_RIT_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
