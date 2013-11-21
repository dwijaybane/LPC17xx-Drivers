/**********************************************************************
* $Id$		lpc_mat_kb.h
*//**
* @file		lpc_mat_kb.h
* @brief	Contains all Matrix Keyboard Initialization code
* @version	1.0
* @date		21. Nov. 2013
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
/** @defgroup MAT_KB
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_MAT_KB_H_
#define LPC_MAT_KB_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup MAT_KB_Public_Macros MAT_KB Public Macros
 * @{
 */

#ifndef ENABLE
#define	ENABLE		1
#endif
#ifndef DISABLE
#define DISABLE		0
#endif

/******************************************************************************/
/*                           MAT_KB Selection                                 */
/******************************************************************************/
#define		MATKB_2X2			ENABLE
#define		MATKB_3X3			DISABLE
#define		MATKB_4X4			DISABLE
#define		MATKB_5X5			DISABLE
#define		MATKB_6X6			DISABLE
#define		MATKB_7X7			DISABLE
#define		MATKB_8X8			DISABLE

/******************************************************************************/
/*                           MAT_KB Selection                                 */
/******************************************************************************/
#if MATKB_2X2
	#define ROW 	2
	#undef ROW3
	#undef ROW4
	#undef ROW5
	#undef ROW6
	#undef ROW7
	#undef ROW8

	#undef COL3
	#undef COL4
	#undef COL5
	#undef COL6
	#undef COL7
	#undef COL8
#endif


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup MAT_KB_Public_Functions MAT_KB Public Functions
 * @{
 */

void Mat_Kb_Init(void);
uint16 Detect_Mat_Key(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_MAT_KB_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
