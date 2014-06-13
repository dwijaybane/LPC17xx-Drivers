/**********************************************************************
* $Id$		lpc_utility.c
*//**
* @file		lpc_utility.c
* @brief	Contains all support utility functions
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
/** @addtogroup UTILITY
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_utility.h"


/************************** PUBLIC FUNCTIONS *************************/
/* Public Functions ----------------------------------------------------------- */
/** @addtogroup UART_Public_Functions
 * @{
 */
 
/********************************************************************//**
 * @brief		Get Integer value from ascii entered value
 * @param[in]	UARTx	UART peripheral selected, should be:
 *   			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * @param[in]   digit   Give Number of digit place you want to enter
 * @return 		Integer value
 *********************************************************************/
uint32_t getAtoI(LPC_UART_TypeDef *UARTx, uint8_t digit)
{
	uint16_t string_buffer[8];
	uint16_t index = 0;		                    /* Pointer in buffer   */
	uint16_t count = 0;		                    /* character count     */

	uint32_t result = 0;
	int8_t cp;
	// int sign;

	for(;;)
	{
		cp = getche(UARTx,BLOCKING);	                    /* get input character */

		if (cp == In_ESC)	                    /* if ESCAPE pressed then exit */
		{
			return (cp);
		}
		else if (cp == In_CR)	                /* CARRIAGE RETURN ? */
		{
			if(count==0)	                    /* any characters at all ? */
			{
				continue;		                /* no, so get another character */
			}
			/* check for any character */
			break;
		}
		else if (cp == In_DELETE || cp == In_BACKSPACE) /* delete or back space */
		{
			if(index==0)	                    /* any characters entered */
			{
				continue;	                    /* no, so get another character */
			}

			printf(UARTx,"%c",Out_BACKSPACE);   /* erase character on the screen */
			printf(UARTx,"%c",Out_SPACE);       /* erase character on the screen */
			printf(UARTx,"%c",Out_BACKSPACE);   /* erase character on the screen */

			index--;			                /* decrement pointer */
			count--;		                    /* decrement character count */
			continue;
		}
		else if (cp>= '0' && cp <='9')		    /* is character between 0 to 9? */
		{
			if(index < digit)
			{
				string_buffer[index] = cp;	    /* save character and */
				index++;					    /* increment pointer */
				count++;					    /* increment count */
				printf(UARTx,"%c",cp);          /* echo character */

				continue;
			}
			else
			{
				printf(UARTx,"%c",'\7');	    /* ring the bell */
			}
			/*				continue;			 and get some more */
		}
		else
		{
			printf(UARTx,"%c",cp);				/* echo character */
			printf(UARTx,"%c",Out_BACKSPACE);	/* erase character on the screen */
			printf(UARTx,"%c",Out_SPACE);		/* erase character on the screen */
			printf(UARTx,"%c",Out_BACKSPACE);	/* erase character on the screen */
			printf(UARTx,"%c",'\7');		 	/* ring the bell */

			continue;
		}
	}
	/* now convert the ascii string into 16 bit integer */
	index =0;	                                /* point to first character entered */
	cp = string_buffer[index];	                /* read character */

	while(count != 0)
	{
		result *= 10;		                    /* multiply buy 10 */
		result += cp-'0';	                    /* data - 30 */
		cp = string_buffer[++index];	        /* increment pointer */
		count--;			                    /* decrement count */
	}
	return(result);
}


/********************************************************************//**
 * @brief		Get value in HEX format from user
 * @param[in]	UARTx	UART peripheral selected, should be:
 *   			- LPC_UART0: UART0 peripheral
 * 				- LPC_UART1: UART1 peripheral
 * 				- LPC_UART2: UART2 peripheral
 * @param[in]   digit   Give Number of digit place you want to enter
 * @return 		HEX value
 *********************************************************************/
uint32_t get_hex(LPC_UART_TypeDef *UARTx, uint8_t digit)
{
	char key;			                /* input character */
	char hex_buffer[8];

	uint16_t pointer=0;	                /* Pointer in buffer */
	uint16_t count=0;		            /* character count */

	uint32_t hex_value = 0;		        /* ASCII 4 bytes into integer */

	for(;;)
	{
		key = getche(UARTx,BLOCKING);
		key = to_upper(key);	            /* get character from serial and */
											/* if it is lower then convert to upper */

		if (key == In_ESC)	            /* if ESCAPE pressed then exit */
		{
			return (key);
		}
		else if (key == In_CR)	        /* CARRIAGE RETURN ? */
		{
			if(count==0)		            /* any characters at all ? */
			{
				continue;		            /* no, so get another character */
			}
			/* check for any character */
			if(count < digit)
			{
				continue;                    /* did not received required bytes, start again */
			}
			break;			                /* yes, exit from this loop */
		}
		else if (key == In_DELETE || key == In_BACKSPACE) /* delete or back space */
		{
			if(pointer==0)	                /* any characters entered */
			{
				continue;	                /* no, so get another character */
			}
			printf(UARTx,"%c",Out_BACKSPACE);      /* erase character on the screen */
			printf(UARTx,"%c",Out_SPACE);          /* erase character on the screen */
			printf(UARTx,"%c",Out_BACKSPACE);      /* erase character on the screen */

			pointer--;			            /* decrement pointer */
			count--;                       /* decrement character count */
			continue;
		}

		/* character 0 to 9 and A to F? */
		else if ((key >= '0' && key <='9')||( key >= 'A' && key <='F'))
		{
			if(pointer < digit)
			{
				hex_buffer[pointer] = key;   /* save character and increment pointer */
				pointer++;
				count++;			            /* increment count */
				printf(UARTx,"%c",key);		        /* echo character */
			}
			else
			{
				printf(UARTx,"%c",'\7');	            /* ring the bell */
			}
			continue;				        /* and get some more */
		}
		else
		{
			printf(UARTx,"%c",'\7');	            /* ring the bell */
		}
	}

	/* now convert 8 bytes into 16 bit integer address */

	pointer = 0;	                    /* point to last character entered */
	//  count--;	                    /* decrement character count */
	key = hex_buffer[pointer];

	while(pointer != digit)
	{
		hex_value *= 16;
		key = hex_buffer[pointer];
		if(key >= '0' && key <= '9')
			hex_value += (key -'0');
		else
			hex_value += ((key -'0')-('A'-'0'-10));

		key = hex_buffer[++pointer];
	}
	return(hex_value);	            /* yes so exit */
}

/**
 * @}
 */


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

