/**********************************************************************
* $Id$		lpc_messages.c
*//**
* @file		lpc_messages.c
* @brief	Contains all Messages for Diagnostic Menu
* @version	1.0
* @date		13. June. 2014
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
/** @addtogroup MSG
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_messages.h"

/* Local Variables------------------------------------------------------------ */
const char *main_menu[13]={"\t\t EPB1768 Diagnostic \r\n",
		                  "Software Ver 1.0  13-June-2014  Dwijay Bane \n\n\r"
	      	  	  	  	  "\t\tMain Menu\n\r",
	      	  	  	  	  "1.  Led Checks\n\r",
	      	  	  	  	  "2.  PushButton Checks\n\r",
	      	  	  	  	  "3.  Buzzer Checks\n\r",
	      	  	  	  	  "4.  Sleep Mode\n\r",
	      	  	  	  	  "5.  RTC Checks\n\r",
	      	  	  	  	  "6.  Adc Checks\n\r",
	      	  	  	  	  "7.  I2CBusPeripherals Checks\n\r",
	      	  	  	  	  "8.  SPIBusPeripherals Checks\n\r",
	      	  	  	  	  "9.  Uart2 Checks\n\r",
	      	  	  	  	  "Select option (1-16)"
};


const char *buzzer_menu[6]={"\t\tBuzzer Menu\n\r",
	      	  	  	  	  	 "1.  Play Beep Test\n\r",
	      	  	  	  	  	 "2.  Play Welcome Tune\n\r",
	      	  	  	  	  	 "3.  Play Memoldy\n\r",
	      	  	  	  	  	 "Select option (1-3)",
	      	  	  	  	  	 "\x1b[24;10HPress ESCAPE to exit\x1b[5;20H",
};

/************************** PUBLIC FUNCTIONS *************************/
/* Public Functions ----------------------------------------------------------- */
/** @addtogroup MSG_Public_Functions
 * @{
 */

/********************************************************************//**
 * @brief		Main Menu Message
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Print_Main_Menu(void)
{
	uint8_t count;

	clr_scr_rst_cur(LPC_UART0);

	for(count = 0; count<13; ++count)
	{
		printf(LPC_UART0,main_menu[count]);
	}
}


/********************************************************************//**
 * @brief		Buzzer Message
 * @param[in]	None
 * @return 		None
 *********************************************************************/
void Print_Buzzer_Menu(void)
{
	uint8_t count;

	clr_scr_rst_cur(LPC_UART0);

	for(count = 0; count<6; ++count)
	{
		printf(LPC_UART0,buzzer_menu[count]);
	}
}



/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

