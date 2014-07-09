/******************************************************************//**
* @file		lpc_st_motor.c
* @brief	Contains all functions support for Stepper Motor library
* 			on LPC17xx
* @version	1.0
* @date		21. Nov. 2013
* @author	Dwijay.Edutech Learning Solutions
**********************************************************************/

/* Peripheral group ----------------------------------------------------------- */
/** @addtogroup ST_MOTOR
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"
#include "lpc_st_motor.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

void Send_Sequence(StMotorDirection_e StMotorDirection, uint16 Speed);

/** @addtogroup ST_MOTOR_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    This function rotates stepper motor in desired direction and angle
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Rotate_Stepper_Motor (StMotorDirection_e StMotorDirection, uint16 Angle, uint16 Speed)
{
    uint16 i,Count;
    Count = (Angle / CAL_ANGLE);
    for(i=0;i < Count;i++)
    {
        Send_Sequence(StMotorDirection,Speed);
    }
}


/*********************************************************************//**
 * @brief	    This function will send sequence to Stepper motor
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Send_Sequence(StMotorDirection_e StMotorDirection, uint16 Speed)
{
    if(StMotorDirection == StMotorClockwise)
    {
    	GPIO_ClearValue(1, _SBF(18, 0xAA));
        GPIO_SetValue(1, _SBF(18, SmClk[temp]));
    }
    if(StMotorDirection == StMotorAntiClockwise)
    {
        GPIO_ClearValue(1, _SBF(18, 0xAA));
        GPIO_SetValue(1, _SBF(18, SmAntClk[temp]));
    }
    delay_ms(Speed);
    temp++;
    if(temp>3) temp=0;
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

