/**********************************************************************
* $Id$		main.c
*//**
* @file		main.c
* @brief	This example describes how to use UART in polling mode
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
#include "lpc_system_init.h"

/* Example group ----------------------------------------------------------- */
/** @defgroup UART_Polling	Polling
 * @ingroup UART_Examples
 * @{
 */

/************************** PRIVATE VARIABLES *************************/
// Message Blocks
uint8_t menu1[] = "Hello Edutech \n\r";
uint8_t menu2[] = "UART polling mode demo \n\r\t MCU LPC17xx - ARM Cortex-M3\n\r";
uint8_t menu3[] = "UART demo terminated!";

/************************** PRIVATE FUNCTIONS *************************/
void print_menu(LPC_UART_TypeDef *UARTx);

/*-------------------------PRIVATE FUNCTIONS------------------------------*/
/*********************************************************************//**
 * @brief		Print Welcome menu
 * @param[in]	UARTx	UART peripheral selected, should be:
 *  			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * 				- LPC_UART3: UART3 peripheral
 * @return 		None
 **********************************************************************/
void print_menu(LPC_UART_TypeDef *UARTx)
{
	UART_Send((LPC_UART_TypeDef *)UARTx, menu1, sizeof(menu1), BLOCKING);
	UART_Send((LPC_UART_TypeDef *)UARTx, menu2, sizeof(menu2), BLOCKING);
}

/*-------------------------MAIN Page------------------------------*/
/** @mainpage UART Polling: Uart Test Example
*   @par Description:
*   - Uart0 Test
*   - Uart2 Test
*
*   @par Activity - more information:
*    Send and Recieve through Uart0 and Uart2
*   - Set Uart0 Baud Rate: 57600 for Terminal
*   - Set Uart2 Baud Rate: 115200 for Terminal
*/
/*-------------------------MAIN FUNCTION------------------------------*/
/*********************************************************************//**
 * @brief	Main UART testing example sub-routine
 * 			Print welcome screen first, then press any key to have it
 * 			read in from the terminal and returned back to the terminal.
 * 			- Press ESC to exit
 * 			- Press 'r' to print welcome screen menu again
 **********************************************************************/

/* With ARM and GHS toolsets, the entry point is main() - this will
   allow the linker to generate wrapper code to setup stacks, allocate
   heap area, and initialize and copy code and data segments. For GNU
   toolsets, the entry point is through __start() in the crt0_gnu.asm
   file, and that startup code will setup stacks and data */
int main(void)
{
	uchar buffer,buf[10],flag;

	System_Init();

	// print welcome screen
	print_menu(LPC_UART0);
	print_menu(LPC_UART2);

    /* Read some data from the buffer */
    while (1)
    {
    	buffer = getche(LPC_UART0);

        /* Got some data */
        if (EscFlag)
        {
        	UART_Send(LPC_UART2, menu3, sizeof(menu3), BLOCKING);
        	break;
        }

        if (buffer == 'r')
        {
        	print_menu(LPC_UART2);
        	get_line(LPC_UART0,buf,6);
        	printf(LPC_UART2,buf);
        }
        else
        {
           /* Echo it back */
        	UART_SendByte((LPC_UART_TypeDef *)LPC_UART2, buffer);
        }
    }

    // wait for current transmission complete - THR must be empty
    while (UART_CheckBusy(LPC_UART0) == SET);
    while (UART_CheckBusy(LPC_UART2) == SET);

    // DeInitialize UART0 peripheral
    UART_DeInit(LPC_UART0);
    UART_DeInit(LPC_UART2);

    /* Loop forever */
    while(1);
    return 1;
}

#ifdef  DEBUG
/*******************************************************************************
* @brief		Reports the name of the source file and the source line number
* 				where the CHECK_PARAM error has occurred.
* @param[in]	file Pointer to the source file name
* @param[in]    line assert_param error line source number
* @return		None
*******************************************************************************/
void check_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
	 ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while(1);
}
#endif

/*
 * @}
 */
