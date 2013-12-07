/**********************************************************************
* $Id$		lpc17xx_systick.c
*//**
* @file		lpc17xx_systick.c
* @brief	Contains all functions support for SYSTICK firmware library
* 			on LPC17xx
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
/** @addtogroup SYSTICK
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"

/*
 * Variables
 */
__IO uint32_t delay_timer;
uint32_t led_timer;

/*----------------- INTERRUPT SERVICE ROUTINES --------------------------*/
/*********************************************************************//**
 * @brief 		SysTick interrupt handler
 * @param		None
 * @return 		None
 ***********************************************************************/
void SysTick_Handler(void)
{
    if(led_timer)
    {
    	--led_timer;
    }
    else
    {
    	LPC_GPIO3->FIOPIN ^= _BIT(25); //Toggle P3.25 Hearbeat led
    	led_timer=led_delay;
    }

	if(delay_timer)
    {
      --delay_timer;           /*decrement Delay Timer */
    }
	
	//Clear System Tick counter flag
	SYSTICK_ClearCounterFlag();
}

/* Public Functions ----------------------------------------------------------- */
/** @addtogroup SYSTICK_Public_Functions
 * @{
 */
/*********************************************************************//**
 * @brief 		Delay Function
 * @param		value in ms
 * @return 		None
 ***********************************************************************/
void delay_ms (uint32_t dly_ticks) 
{
  delay_timer = dly_ticks;
  while(delay_timer)
  {
    /* do nothing */
  } 
}

 /*********************************************************************//**
 * @brief 		Initial System Tick with Config
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SYSTICK_Config(void)
{
  //Initialize System Tick with 10ms time interval
  SYSTICK_InternalInit(1);
  //Enable System Tick interrupt
  SYSTICK_IntCmd(ENABLE);
  //Enable System Tick Counter
  SYSTICK_Cmd(ENABLE);
}

/*********************************************************************//**
 * @brief 		Initial System Tick with using internal CPU clock source
 * @param[in]	time	time interval(ms)
 * @return 		None
 **********************************************************************/
void SYSTICK_InternalInit(uint32_t time)
{
	uint32_t cclk;
	float maxtime;

	cclk = SystemCoreClock;
	/* With internal CPU clock frequency for LPC17xx is 'SystemCoreClock'
	 * And limit 24 bit for RELOAD value
	 * So the maximum time can be set:
	 * 1/SystemCoreClock * (2^24) * 1000 (ms)
	 */
	//check time value is available or not
	maxtime = (1<<24)/(SystemCoreClock / 1000) ;
	if(time > maxtime)
		//Error loop
		while(1);
	else
	{
		//Select CPU clock is System Tick clock source
		SysTick->CTRL |= ST_CTRL_CLKSOURCE;
		/* Set RELOAD value
		 * RELOAD = (SystemCoreClock/1000) * time - 1
		 * with time base is millisecond
		 */
		SysTick->LOAD = (cclk/1000)*time - 1;
	}
}

/*********************************************************************//**
 * @brief 		Initial System Tick with using external clock source
 * @param[in]	freq	external clock frequency(Hz)
 * @param[in]	time	time interval(ms)
 * @return 		None
 **********************************************************************/
void SYSTICK_ExternalInit(uint32_t freq, uint32_t time)
{
	float maxtime;

	/* With external clock frequency for LPC17xx is 'freq'
	 * And limit 24 bit for RELOAD value
	 * So the maximum time can be set:
	 * 1/freq * (2^24) * 1000 (ms)
	 */
	//check time value is available or not
	maxtime = (1<<24)/(freq / 1000) ;
	if (time>maxtime)
		//Error Loop
		while(1);
	else
	{
		//Select external clock is System Tick clock source
		SysTick->CTRL &= ~ ST_CTRL_CLKSOURCE;
		/* Set RELOAD value
		 * RELOAD = (freq/1000) * time - 1
		 * with time base is millisecond
		 */
		maxtime = (freq/1000)*time - 1;
		SysTick->LOAD = (freq/1000)*time - 1;
	}
}

/*********************************************************************//**
 * @brief 		Enable/disable System Tick counter
 * @param[in]	NewState	System Tick counter status, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @return 		None
 **********************************************************************/
void SYSTICK_Cmd(FunctionalState NewState)
{
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if(NewState == ENABLE)
		//Enable System Tick counter
		SysTick->CTRL |= ST_CTRL_ENABLE;
	else
		//Disable System Tick counter
		SysTick->CTRL &= ~ST_CTRL_ENABLE;
}

/*********************************************************************//**
 * @brief 		Enable/disable System Tick interrupt
 * @param[in]	NewState	System Tick interrupt status, should be:
 * 					- ENABLE
 * 					- DISABLE
 * @return 		None
 **********************************************************************/
void SYSTICK_IntCmd(FunctionalState NewState)
{
	CHECK_PARAM(PARAM_FUNCTIONALSTATE(NewState));

	if(NewState == ENABLE)
		//Enable System Tick counter
		SysTick->CTRL |= ST_CTRL_TICKINT;
	else
		//Disable System Tick counter
		SysTick->CTRL &= ~ST_CTRL_TICKINT;
}

/*********************************************************************//**
 * @brief 		Get current value of System Tick counter
 * @param[in]	None
 * @return 		current value of System Tick counter
 **********************************************************************/
uint32_t SYSTICK_GetCurrentValue(void)
{
	return (SysTick->VAL);
}

/*********************************************************************//**
 * @brief 		Clear Counter flag
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void SYSTICK_ClearCounterFlag(void)
{
	SysTick->CTRL &= ~ST_CTRL_COUNTFLAG;
}
/**
 * @}
 */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

