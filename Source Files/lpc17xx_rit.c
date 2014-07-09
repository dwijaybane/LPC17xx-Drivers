/******************************************************************//**
* @file		lpc17xx_rit.c
* @brief	Contains all functions support for RIT firmware library on LPC17xx
* @version	1.0
* @date		29. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup RIT
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc17xx_rit.h"
#include "lpc17xx_clkpwr.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief		RIT interrupt handler sub-routine
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void RIT_IRQHandler(void)
{
	RIT_GetIntStatus(LPC_RIT); //call this to clear interrupt flag
	LPC_GPIO0->FIOPIN ^= _BIT(10); //Toggle P0.10 led
}

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup RIT_Public_Functions
 * @{
 */
/* RIT Initialization Config function ---------------------------------*/
/********************************************************************//**
* @brief		Initializes the RITx peripheral according to the specified
*               parameters.
* @param[in]	RITx	  RIT peripheral selected, should be:
*   			- LPC_RIT: RIT peripheral
* @param[in]	Interval  Time interval in millisecond
* @return 		None
*********************************************************************/
void RIT_Config(LPC_RIT_TypeDef *RITx, uint32_t time_interval)
{
	RIT_Init(RITx);
			/* Configure time_interval for RIT
			 * In this case: time_interval = 1000 ms = 1s
			 * So, RIT will generate interrupt each 1s
			 */
	RIT_TimerConfig(RITx,time_interval);
	NVIC_EnableIRQ(RIT_IRQn);
}

/******************************************************************************//*
 * @brief 		Initial for RIT
 * 					- Turn on power and clock
 * 					- Setup default register values
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @return 		None
 *******************************************************************************/
void RIT_Init(LPC_RIT_TypeDef *RITx)
{
	CHECK_PARAM(PARAM_RITx(RITx));
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCRIT, ENABLE);
	//Set up default register values
	RITx->RICOMPVAL = 0xFFFFFFFF;
	RITx->RIMASK	= 0x00000000;
	RITx->RICTRL	= 0x0C;
	RITx->RICOUNTER	= 0x00000000;
	// Turn on power and clock

}
/******************************************************************************//*
 * @brief 		DeInitial for RIT
 * 					- Turn off power and clock
 * 					- ReSetup default register values
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @return 		None
 *******************************************************************************/
void RIT_DeInit(LPC_RIT_TypeDef *RITx)
{
	CHECK_PARAM(PARAM_RITx(RITx));

	// Turn off power and clock
	CLKPWR_ConfigPPWR (CLKPWR_PCONP_PCRIT, DISABLE);
	//ReSetup default register values
	RITx->RICOMPVAL = 0xFFFFFFFF;
	RITx->RIMASK	= 0x00000000;
	RITx->RICTRL	= 0x0C;
	RITx->RICOUNTER	= 0x00000000;
}

/******************************************************************************//*
 * @brief 		Set compare value, mask value and time counter value
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @param[in]	time_interval: timer interval value (ms)
 * @return 		None
 *******************************************************************************/
void RIT_TimerConfig(LPC_RIT_TypeDef *RITx, uint32_t time_interval)
{
	uint32_t clock_rate, cmp_value;
	CHECK_PARAM(PARAM_RITx(RITx));

	// Get PCLK value of RIT
	clock_rate = CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT);

	/* calculate compare value for RIT to generate interrupt at
	 * specified time interval
	 * COMPVAL = (RIT_PCLK * time_interval)/1000
	 * (with time_interval unit is millisecond)
	 */
	cmp_value = (clock_rate /1000) * time_interval;
	RITx->RICOMPVAL = cmp_value;

	/* Set timer enable clear bit to clear timer to 0 whenever
	 * counter value equals the contents of RICOMPVAL
	 */
	RITx->RICTRL |= (1<<1);
}


/******************************************************************************//*
 * @brief 		Enable/Disable Timer
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @param[in]	NewState 	New State of this function
 * 					-ENABLE: Enable Timer
 * 					-DISABLE: Disable Timer
 * @return 		None
 *******************************************************************************/
void RIT_Cmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RITx(RITx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	//Enable or Disable Timer
	if(NewState==ENABLE)
	{
		RITx->RICTRL |= RIT_CTRL_TEN;
	}
	else
	{
		RITx->RICTRL &= ~RIT_CTRL_TEN;
	}
}

/******************************************************************************//*
 * @brief 		Timer Enable/Disable on debug
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @param[in]	NewState 	New State of this function
 * 					-ENABLE: The timer is halted whenever a hardware break condition occurs
 * 					-DISABLE: Hardware break has no effect on the timer operation
 * @return 		None
 *******************************************************************************/
void RIT_TimerDebugCmd(LPC_RIT_TypeDef *RITx, FunctionalState NewState)
{
	CHECK_PARAM(PARAM_RITx(RITx));
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	//Timer Enable/Disable on break
	if(NewState==ENABLE)
	{
		RITx->RICTRL |= RIT_CTRL_ENBR;
	}
	else
	{
		RITx->RICTRL &= ~RIT_CTRL_ENBR;
	}
}
/******************************************************************************//*
 * @brief 		Check whether interrupt flag is set or not
 * @param[in]	RITx is RIT peripheral selected, should be: LPC_RIT
 * @return 		Current interrupt status, could be: SET/RESET
 *******************************************************************************/
IntStatus RIT_GetIntStatus(LPC_RIT_TypeDef *RITx)
{
	uint8_t result;
	CHECK_PARAM(PARAM_RITx(RITx));
	if((RITx->RICTRL&RIT_CTRL_INTEN)==1)	result= SET;
	else return RESET;
	//clear interrupt flag
	RITx->RICTRL |= RIT_CTRL_INTEN;
	return result;
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
