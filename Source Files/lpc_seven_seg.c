/******************************************************************//**
* @file		lpc_seven_seg.c
* @brief	Contains all functions support for Seven Segment library
* 			on LPC17xx
* @version	1.0
* @date		18. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup SEVEN_SEG
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"
#include "lpc_seven_seg.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup SEVEN_SEG_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    This routine initializes the Seven Segment
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Seven_Seg_Init (void)
{
	GPIO_ClearValue(2, _BIT(12));      // Select first Segment
	GPIO_SetValue(1, _SBF(18, 0xFF));  // Common anode so 0xFF clears display
	GPIO_SetValue(2, _BIT(13));        // gate signal high
	GPIO_ClearValue(2, _BIT(13));      // latch signal on falling edge

	GPIO_SetValue(2, _BIT(12));        // Select second Segment
	GPIO_SetValue(1, _SBF(18, 0xFF));  // Common anode so 0xFF clears display
	GPIO_SetValue(2, _BIT(13));        // gate signal high
    GPIO_ClearValue(2, _BIT(13));      // latch signal on falling edge
}


/*********************************************************************//**
 * @brief	    This function displays digit on seven-segment display
 * @param[in]	Digit	number to be displayed on seven segment
 * @param[in]	Seg		select segment for display number
 *                      - 1: Seg1
 *                      - 2: Seg2
 * @return 		None
 **********************************************************************/
void Display_Digit (uchar Digit, uchar Seg)
{
	uchar Number[16]= {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};

	/* Select segment */
	if(Seg == Seg1)
	{
		GPIO_ClearValue(2, _BIT(12));      // Select first Segment
	}
	else if(Seg == Seg2)
	{
		GPIO_SetValue(2, _BIT(12));        // Select second Segment
	}

	GPIO_SetValue(1, _SBF(18, 0xFF));  // Common anode so 0xFF clears display

	/* Give Data according to configuration */
	if(COMMON_CATHODE_SEG)
	{
		GPIO_SetValue(1, _SBF(18, Number[Digit]));
	}
	else if(COMMON_ANODE_SEG)
	{
		GPIO_ClearValue(1, _SBF(18, Number[Digit]));
	}

	/* Latch data */
	GPIO_SetValue(2, _BIT(13));            // gate signal high
	GPIO_ClearValue(2, _BIT(13));          // latch signal on falling edge
}


/*********************************************************************//**
 * @brief	    This function displays number on seven-segment display
 * @param[in]	Number	Two digit Number to be displayed
 * @return 		None
 **********************************************************************/
void Display_Data (uint16 Number)
{
	uint16 i=0, j=TOTAL_SEGMENTS-1;
	uint16 Digits[TOTAL_SEGMENTS];

	while(1)
	{
		Digits[i] = Number % 10;
		Display_Digit(Digits[i],TOTAL_SEGMENTS - j);
		j--;
		i++;
		Number = Number /10;
		if(i >= TOTAL_SEGMENTS)
		{
			break;
		}
	}
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

