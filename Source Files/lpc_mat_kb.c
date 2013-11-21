/**********************************************************************
* $Id$		lpc_mat_kb.c
*//**
* @file		lpc_mat_kb.c
* @brief	Contains all functions support for Matrix Keyboard library
* 			on LPC17xx
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
/** @addtogroup MAT_KB
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"
#include "lpc_mat_kb.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

uchar Find_Column (void);
uchar Find_Row (void);
void Col_High (void);
void Row_High (void);
void Row_Low (void);


/*********************************************************************//**
 * @brief	    This routine configures pin assignment
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Mat_Kb_Config (void)
{
	GPIO_SetDir(0, _SBF(19,0x03), 1);  // Set P0.19 & P0.20 as ROW output
	GPIO_SetDir(0, _SBF(21,0x03), 0);  // Set P0.21 & P0.22 as COL input
}


/*********************************************************************//**
 * @brief	    This function initializes matrix keyboard.
 *              Make ROWs as o/p & Columns as an i/p.
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Mat_Kb_Init (void)
{
	Mat_Kb_Config();

    Col_High();
	Row_Low();
}


/*********************************************************************//**
 * @brief	    This function will scan whether any key is pressed or not
 * @param[in]	None
 * @return 		Returns Status of Key Pressed
 **********************************************************************/
uchar Scan_Key (void)
{
	uchar ReadValue = 0,COL1,COL2;
	uint32_t col1,col2;
	col1 = GPIO_ReadValue(0);
	col2 = GPIO_ReadValue(0);
	col1 &= 0x00200000;
	col2 &= 0x00400000;
	if(col1 == 0x00200000)
	{
		COL1 = 1;
	}
	else
	{
		COL1 = 0;
	}

	if(col2 == 0x00400000)
	{
		COL2 = 1;
	}
	else
	{
		COL2 = 0;
	}

	#if MATKB_2X2
	    ReadValue = (COL1 & COL2);
	#endif

	if((ReadValue != 1))
	    return(1);
	else
	    return(0);
}

/*********************************************************************//**
 * @brief	    This function detects whether any key is pressed or
 *              not also returns its its location
 * @param[in]	None
 * @return 		Returns Pressed Key Code
 **********************************************************************/
uint16 Detect_Mat_Key(void)
{
    uchar Col = 0,Row = 0,Key = 0;

    Row = Find_Row();
    Col = Find_Column();
    Key = Col + (10 * Row);
    return(Key);
}


/*********************************************************************//**
 * @brief	    This function will find the column from where key is pressed
 * @param[in]	None
 * @return 		Returns Column index
 **********************************************************************/
uchar Find_Column(void)
{
	uint32_t col;

    if (Scan_Key() != 1)
    {
        return(0);
    }
    #if MATKB_2X2
        col = GPIO_ReadValue(0);
        col |= 0xFF9FFFFF;
        if (col == 0xFFDFFFFF) // if COL1 = 0
	    {
            return(1);
        }
        if (col == 0xFFBFFFFF) // if COL2 = 0
        {
	        return(2);
        }
    #endif
    return 0;
}


/*********************************************************************//**
 * @brief	    This function will find the row from where key is pressed
 * @param[in]	None
 * @return 		Returns Row Index
 **********************************************************************/
uchar Find_Row(void)
{
    uchar i;

    for(i = 1; i <= ROW ;i++)
    {
        Row_High();
        switch (i)
        {
            case 1:
            	GPIO_ClearValue(0,_BIT(19)); // ROW1
		        break;

	        case 2:
	        	GPIO_ClearValue(0,_BIT(20)); // ROW2
                break;
        }
        if (Scan_Key() == 1)
        {
            Row_Low();
            return(i);
        }
    }
    return 0;
}


/*********************************************************************//**
 * @brief	    This function will set all columns
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Col_High (void)
{
    GPIO_SetValue(0,_BIT(21));  // COL1 High
    GPIO_SetValue(0,_BIT(22));  // COL2 High
}


/*********************************************************************//**
 * @brief	    This function will set all Rows
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Row_High (void)
{
    GPIO_SetValue(0,_BIT(19));  // ROW1 High
    GPIO_SetValue(0,_BIT(20));  // ROW2 High
}


/*********************************************************************//**
 * @brief	    This function will clear all Rows
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Row_Low (void)
{
    GPIO_ClearValue(0,_BIT(19));  // ROW1 Low
    GPIO_ClearValue(0,_BIT(20));  // ROW2 Low
}


/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

