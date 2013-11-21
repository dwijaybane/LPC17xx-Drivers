/**********************************************************************
* $Id$		lpc_lcd.c
*//**
* @file		lpc_lcd.c
* @brief	Contains all functions support for LCD library
* 			on LPC17xx
* @version	1.0
* @date		19. Nov. 2013
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
/** @addtogroup LCD
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_system_init.h"
#include "lpc_lcd.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup LCD_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    This routine configures pin assignment
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Lcd_Config (void)
{
    #ifdef LCD_4BIT
	{
		GPIO_SetDir(1, _SBF(18,0xF0), 1);   // Set P1.18 to P1.21 as output
	}
    #endif
    #ifdef LCD_8BIT
	{
		GPIO_SetDir(1, _SBF(18,0xFF), 1);   // Set P1.18 to P1.25 as output
	}
    #endif
	GPIO_SetDir(2, _SBF(11, 0x07), 1);      // Set P2.11 to P2.13 as Output
}


/*********************************************************************//**
 * @brief	    This function initializes LCD
 * @param[in]	AddrCount	select increment or decrement address counter
 *                          - Dec : 0
 *                          - Inc : 1
 * @param[in]	DispShift	select display shift on or off
 *                          - DispShiftOff: 0
 *                          - DispShiftOn : 1
 * @return 		None
 **********************************************************************/
void Lcd_Init (AddrCount_e AddrCount, DispShift_e DispShift)
{
	uint16 i;
	Lcd_Config();                  // Configure Pins

	GPIO_ClearValue(2, _BIT(11));  // Clear RS
	GPIO_ClearValue(2, _BIT(12));  // Clear RW
	GPIO_SetValue(2, _BIT(13));    // Set   EN
	for(i=0;i<1500;i++);           // 15 us

	Write_Command_Lcd(LCD_CLEAR);
	delay_ms(2);
	Write_Command_Lcd(LCD_CLEAR);
    delay_ms(4);

    #ifdef LCD_4BIT
    {
        Write_Command_Lcd(LCD_4BIT_2LINE);
        for(i=0;i<5000;i++);             // 50 us
    }
    #endif
    #ifdef LCD_8BIT
    {
        Write_Command_Lcd(LCD_8BIT_2LINE);
        delay_ms(1);
    }
    #endif

    Write_Command_Lcd(CURSOR_HOME);
    for(i=0;i<5000;i++);             // 50 us

    if(AddrCount == Dec)
    {
        if(DispShift == DispShiftOff)
        {
            Write_Command_Lcd(LCD_DEC_ADD_DISP_SHIFT_OFF);
        }
        else
        {
            Write_Command_Lcd(LCD_DEC_ADD_DISP_SHIFT_ON);
        }
    }
    else
    {
        if(DispShift == DispShiftOff)
        {
            Write_Command_Lcd(LCD_INC_ADD_DISP_SHIFT_OFF);
        }
        else
        {
            Write_Command_Lcd(LCD_INC_ADD_DISP_SHIFT_ON);
        }
    }
    Write_Command_Lcd(LCD_OFF);
    Write_Command_Lcd(LCD_ON);
    Write_Command_Lcd(CURSOR_OFF);
    delay_ms(10);
}


/*********************************************************************//**
 * @brief	    This function sends enable signal to LCD
 * @param[in]	None
 * @return 		None
 **********************************************************************/
void Lcd_Enable (void)
{
    uint16 i;
    GPIO_SetValue(2, _BIT(13));    // Set   EN
    #ifdef LCD_4BIT
        for(i=0;i<75;i++);         // 1 us
    #endif
    #ifdef LCD_8BIT
        for(i=0;i<300;i++);        // 3 us
    #endif
    GPIO_ClearValue(2, _BIT(13));  // Clear   EN
}


/*********************************************************************//**
 * @brief	    This function writes commands to the LCD
 * @param[in]	Command		command to be written on LCD
 * @return 		None
 **********************************************************************/
void Write_Command_Lcd (uchar Command)
{
	uint16 Mask;
	uint16 LcdData = Command;
	LcdData = LcdData << LCD_DATA_START_PIN;
	Mask = (0x0F << (LCD_DATA_START_PIN));

	#ifdef LCD_4BIT
	{
		GPIO_ClearValue(2, _BIT(11));         // Clear RS
	    GPIO_ClearValue(2, _BIT(12));         // Clear RW
	    GPIO_ClearValue(1, _SBF(18, 0xF0));   // Clear Old Value
	    GPIO_SetValue(1, _SBF(18,((LcdData >> 4) & Mask)));
	    Lcd_Enable();

	    GPIO_ClearValue(1, _SBF(18, 0xF0));   // Clear Old Value
	    GPIO_SetValue(1, _SBF(18,(LcdData & Mask)));
	    Lcd_Enable();
	}
	#endif
	#ifdef LCD_8BIT
	{
		GPIO_ClearValue(2, _BIT(11));         // Clear RS
		GPIO_ClearValue(2, _BIT(12));         // Clear RW
		GPIO_ClearValue(1, _SBF(18, 0xFF));   // Clear Old Value
	    GPIO_SetValue(1, _SBF(18,Command));   // Send Command

	    Lcd_Enable();
	}
	#endif
}


/*********************************************************************//**
 * @brief	    This function writes data to the LCD
 * @param[in]	Character		data to be written on LCD
 * @return 		None
 **********************************************************************/
void Write_Data_Lcd (uchar Character)
{
	 uint16 Mask;
	 uint16 LcdData = Character;
	 LcdData = LcdData << LCD_DATA_START_PIN;
	 Mask = (0x0F << (LCD_DATA_START_PIN));

	 Write_Command_Lcd(CURSOR_OFF); // Needed

     #ifdef LCD_4BIT
	 {
	     GPIO_SetValue(2, _BIT(11));             // Set RS
		 GPIO_ClearValue(2, _BIT(12));           // Clear RW
		 GPIO_ClearValue(1, _SBF(18, 0xF0));     // Clear Old Value
	     GPIO_SetValue(1, _SBF(18,((LcdData >> 4) & Mask)));
	     Lcd_Enable();

	     GPIO_ClearValue(1, _SBF(18, 0xF0));     // Clear Old Value
	     GPIO_SetValue(1, _SBF(18,(LcdData & Mask)));
	     Lcd_Enable();
	 }
	 #endif
	 #ifdef LCD_8BIT
	 {
		 GPIO_SetValue(2, _BIT(11));             // Set RS
		 GPIO_ClearValue(2, _BIT(12));           // Clear RW
		 GPIO_ClearValue(1, _SBF(18, 0x0F));     // Clear Old Value
		 GPIO_SetValue(1, _SBF(18,Character));   // Send Data

	     Lcd_Enable();
	 }
	 #endif
}


/*********************************************************************//**
 * @brief	    This function sets coursor to desired Position
 * @param[in]	LineNum		enter line number
 * @param[in]	Position	enter the Position ie. column number
 * @param[in]	CursorType	select cursor ON/OFF/BLINK
 *              - Off   : 0
 *              - On    : 1
 *              - Blink : 2
 * @return 		None
 **********************************************************************/
void Set_Cursor (uchar LineNum, uchar Position, CursorType_e CursorType)
{
	uchar LcdCmd=0;
    switch(LineNum)
	{
	    case 1:
	        LcdCmd = ROWADDR1 + (Position - 1);
	        break;
	    case 2:
	        LcdCmd = ROWADDR2 + (Position - 1);
	        break;
	    case 3:
	        LcdCmd = ROWADDR3 + (Position - 1);
	        break;
	    case 4:
	        LcdCmd = ROWADDR4 + (Position - 1);
	        break;
	}
	Write_Command_Lcd(LcdCmd);

	if(CursorType == Off)
	{
		Write_Command_Lcd(CURSOR_OFF);
	}
	else if(CursorType == On)
	{
	    Write_Command_Lcd(CURSOR_ON);
	}
	else if(CursorType == Blink)
	{
	    Write_Command_Lcd(CURSOR_BLINK);
	}
}


/*********************************************************************//**
 * @brief	    This function display Character to desired Position
 * @param[in]	Charater	string to be displayed
 * @param[in]	LineNum		enter line number
 * @param[in]	Position	enter the Position ie. column number
 * @param[in]	CursorType	select cursor ON/OFF/BLINK
 *              - Off   : 0
 *              - On    : 1
 *              - Blink : 2
 * @return 		None
 **********************************************************************/
void Display_Character (uchar Character, uchar LineNum, uchar Position, CursorType_e CursorType)
{
	Set_Cursor(LineNum, Position, CursorType);
	Write_Data_Lcd(Character);
}


/*********************************************************************//**
 * @brief	    This function display string to desired Position
 * @param[in]	String		string to be displayed
 * @param[in]	LineNum		enter line number
 * @param[in]	Position	enter the Position ie. column number
 * @param[in]	CursorType	select cursor ON/OFF/BLINK
 *              - Off   : 0
 *              - On    : 1
 *              - Blink : 2
 * @return 		None
 **********************************************************************/
void Display_String (uchar *String, uchar LineNum, uchar Position, CursorType_e CursorType)
{
	while (*String)
	{
	    Display_Character(*String++,LineNum,Position++,CursorType);

	    if(Position-1 == COLUMNSIZE)
	    {
	        LineNum++;
	        Position =1;
	    }
	}
}


/*********************************************************************//**
 * @brief	    This function shifts whole display in desired direction
 * @param[in]	Direction	select direction LEFT/RIGHT
 * @param[in]	CursorType	select cursor ON/OFF/BLINK
 *              - Off   : 0
 *              - On    : 1
 *              - Blink : 2
 * @return 		None
 **********************************************************************/
void Display_Shift (ShiftDir_e Direction, CursorType_e CursorType)
{
	if(CursorType == Off)
	{
		Write_Command_Lcd(CURSOR_OFF);
	}
    else if(CursorType == On)
	{
	    Write_Command_Lcd(CURSOR_ON);
	}
	else if(CursorType == Blink)
	{
	    Write_Command_Lcd(CURSOR_BLINK);
	}

	if(Direction == Left)
	{
	    Write_Command_Lcd(LCD_SHIFT_LEFT);
	}
	else if(Direction == Right)
	{
	    Write_Command_Lcd(LCD_SHIFT_RIGHT);
	}
}


/*********************************************************************//**
 * @brief	    This function builds custom character
 * @param[in]	loc		position
 * @param[in]	*p		Custom Character
 * @return 		None
 **********************************************************************/
void CGRAM_Char_Gen (uchar loc,uchar *p)
{
    uchar i;

    if(loc<8)                             // If valid
    {
        Write_Command_Lcd(0x40+(loc*8));  // Access to CGRAM
        for(i=0;i<8;i++)
        {
            Write_Data_Lcd(p[i]);         // Write Pattern
        }
    }
    Write_Command_Lcd(0x80);              // Shift to DDRAM location 0
}


/*********************************************************************//**
 * @brief	    This function converts from integer to ascii
 *              and displays on LCD
 * @param[in]	VarData	 integer variable
 * @param[in]	Row		 Row Position
 * @param[in]	Col		 Col Position
 * @return 		None
 **********************************************************************/
void Display_Decimal_Lcd (uint16 VarData, uchar Row, uchar Col)
{
    uchar Character[17]={"0123456789"};
    uint16 DivValue=10,BaseValue=10;
    while(DivValue)
    {
        Display_Character(Character[VarData/DivValue],Row,Col++,Off);
        VarData%=DivValue;
        DivValue/=BaseValue;
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

