/******************************************************************//**
* @file		lpc17xx_exti.c
* @brief	Contains all functions support for EXTI firmware library on LPC17xx
* @version	1.0
* @date		20. Dec. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup EXTI
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_exti.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */


/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		External interrupt 0 handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EINT0_IRQHandler(void)
{
	//clear the EINT0 flag
	EXTI_ClearEXTIFlag(EXTI_EINT0);
}

/*********************************************************************//**
 * @brief		External interrupt 1 handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EINT1_IRQHandler(void)
{
	status = ~status;
	//clear the EINT1 flag
	EXTI_ClearEXTIFlag(EXTI_EINT1);
}

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup EXTI_Public_Functions
 * @{
 */

/*********************************************************************//**
 * @brief 		Initial Configuration for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR registers value
 * @param[in]	EXTILine	 external interrupt line, should be:
 * 				- EXTI_EINT0: external interrupt line 0
 * 				- EXTI_EINT1: external interrupt line 1
 * 				- EXTI_EINT2: external interrupt line 2
 * 				- EXTI_EINT3: external interrupt line 3
 * @param[in]	mode 	external mode, should be:
 * 				- EXTI_MODE_LEVEL_SENSITIVE
 * 				- EXTI_MODE_EDGE_SENSITIVE
 * @param[in]	polarity	 external polarity value, should be:
 * 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
 * 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
 * @return 		None
 **********************************************************************/
void EXTI_Config (EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode, EXTI_POLARITY_ENUM polarity)
{
	// Pin configuration for EXTI
	PINSEL_CFG_Type PinCfg;
	// EXTI Configuration structure variable
	EXTI_InitTypeDef EXTICfg;

	EXTI_Init();

	EXTICfg.EXTI_Line = EXTILine;
	/* edge sensitive */
	EXTICfg.EXTI_Mode = mode;
	EXTICfg.EXTI_polarity = polarity;
	EXTI_ClearEXTIFlag(EXTILine);
	EXTI_ConfigStructInit(&EXTICfg);

	NVIC_SetPriorityGrouping(4);

	switch (EXTILine)
	{
	case EXTI_EINT0:
		/* P2.10 as /EINT0 */
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 10;
		PinCfg.Portnum = 2;
		PINSEL_ConfigPin(&PinCfg);

		LPC_SC->EXTINT = 0x1;   // clear bit

		NVIC_SetPriority(EINT0_IRQn, 0);
		NVIC_EnableIRQ(EINT0_IRQn);

		break;

	case EXTI_EINT1:
		/* P2.11 as /EINT1 */
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 11;
		PinCfg.Portnum = 2;
		PINSEL_ConfigPin(&PinCfg);

		LPC_SC->EXTINT = 0x2;   // clear bit

		NVIC_SetPriority(EINT1_IRQn, 0);
		NVIC_EnableIRQ(EINT1_IRQn);

		break;

	case EXTI_EINT2:
		/* P2.12 as /EINT2 */
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 12;
		PinCfg.Portnum = 2;
		PINSEL_ConfigPin(&PinCfg);

		LPC_SC->EXTINT = 0x4;   // clear bit

		NVIC_SetPriority(EINT2_IRQn, 0);
		NVIC_EnableIRQ(EINT2_IRQn);

		break;

	case EXTI_EINT3:
		/* P2.13 as /EINT3 */
		PinCfg.Funcnum = 1;
		PinCfg.OpenDrain = 0;
		PinCfg.Pinmode = 0;
		PinCfg.Pinnum = 13;
		PinCfg.Portnum = 2;
		PINSEL_ConfigPin(&PinCfg);

		LPC_SC->EXTINT = 0x8;   // clear bit

		NVIC_SetPriority(EINT3_IRQn, 0);
		NVIC_EnableIRQ(EINT3_IRQn);

		break;

	default:
		break;
	}
}


/*********************************************************************//**
 * @brief 		Initial for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR registers to default value
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void EXTI_Init(void)
{
	LPC_SC->EXTMODE = 0x0;
	LPC_SC->EXTPOLAR = 0x0;
}


/*********************************************************************//**
* @brief 		Close EXT
* @param[in]	None
* @return 		None
**********************************************************************/
void EXTI_DeInit(void)
{
	;
}


/*********************************************************************//**
 * @brief 		Configuration for EXT
 * 				- Set EXTINT, EXTMODE, EXTPOLAR register
 * @param[in]	EXTICfg	Pointer to a EXTI_InitTypeDef structure
 *              that contains the configuration information for the
 *              specified external interrupt
 * @return 		None
 **********************************************************************/
void EXTI_ConfigStructInit(EXTI_InitTypeDef *EXTICfg)
{
	LPC_SC->EXTINT = 0x0;
	EXTI_SetMode(EXTICfg->EXTI_Line, EXTICfg->EXTI_Mode);
	EXTI_SetPolarity(EXTICfg->EXTI_Line, EXTICfg->EXTI_polarity);
}


/*********************************************************************//**
* @brief 		Set mode for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	mode 	external mode, should be:
* 				- EXTI_MODE_LEVEL_SENSITIVE
* 				- EXTI_MODE_EDGE_SENSITIVE
* @return 		None
*********************************************************************/
void EXTI_SetMode(EXTI_LINE_ENUM EXTILine, EXTI_MODE_ENUM mode)
{
	if(mode == EXTI_MODE_EDGE_SENSITIVE)
	{
		LPC_SC->EXTMODE |= (1 << EXTILine);
	}
	else if(mode == EXTI_MODE_LEVEL_SENSITIVE)
	{
		LPC_SC->EXTMODE &= ~(1 << EXTILine);
	}
}


/*********************************************************************//**
* @brief 		Set polarity for EXTI pin
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @param[in]	polarity	 external polarity value, should be:
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* 				- EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE
* @return 		None
*********************************************************************/
void EXTI_SetPolarity(EXTI_LINE_ENUM EXTILine, EXTI_POLARITY_ENUM polarity)
{
	if(polarity == EXTI_POLARITY_RISING_EDGE)
	{
		LPC_SC->EXTPOLAR |= (1 << EXTILine);
	}
	else if(polarity == EXTI_POLARITY_FALLING_EDGE)
	{
		LPC_SC->EXTPOLAR &= ~(1 << EXTILine);
	}
}


/*********************************************************************//**
* @brief 		Clear External interrupt flag
* @param[in]	EXTILine	 external interrupt line, should be:
* 				- EXTI_EINT0: external interrupt line 0
* 				- EXTI_EINT1: external interrupt line 1
* 				- EXTI_EINT2: external interrupt line 2
* 				- EXTI_EINT3: external interrupt line 3
* @return 		None
*********************************************************************/
void EXTI_ClearEXTIFlag(EXTI_LINE_ENUM EXTILine)
{
		LPC_SC->EXTINT |= (1 << EXTILine);
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

