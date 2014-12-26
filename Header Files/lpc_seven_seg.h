/******************************************************************//**
* @file		lpc_seven_seg.h
* @brief	Contains all Seven Segment Initialization code
* @version	1.0
* @date		18. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @defgroup SEVEN_SEG
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SEVEN_SEG_H_
#define LPC_SEVEN_SEG_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"
#include "lpc_system_init.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup SEVEN_SEG_Public_Macros SEVEN_SEG Public Macros
 * @{
 */

/******************************************************************************/
/*                         Seven Segment Specification                        */
/******************************************************************************/

#define 	a 		0x01
#define 	b 		0x02
#define 	c 		0x04
#define 	d 		0x08
#define 	e 		0x10
#define 	f 		0x20
#define 	g 		0x40
#define 	dot		0x80
#define 	all 	0xFF

/******************************************************************************/
/*              Seven Segment Display Character Specifications                */
/******************************************************************************/

#define 	ZERO  	a+b+c+d+e+f
#define 	ONE    	b+c
#define 	TWO    	a+b+d+e+g
#define 	THREE  	a+b+c+d+g
#define 	FOUR   	b+c+f+g
#define 	FIVE   	a+c+d+f+g
#define 	SIX    	a+c+d+e+f+g
#define 	SEVEN  	a+b+c
#define 	EIGHT  	a+b+c+d+e+f+g
#define 	NINE  	a+b+c+d+f+g

#define 	TOTAL_SEGMENTS	      2	    // Define number of Segments to be connected maximum up to 4
#define 	COMMON_ANODE_SEG	 ON		// Define type of Segmnent connections
#define 	COMMON_CATHODE_SEG	OFF

#define     Seg1    1
#define     Seg2    2

/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup SEVEN_SEG_Public_Functions SEVEN SEG Public Functions
 * @{
 */

void Seven_Seg_Init (void);
void Display_Digit (uint8_t Digit, uint8_t Seg);
void Display_Data (uint16_t Number);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SEVEN_SEG_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
