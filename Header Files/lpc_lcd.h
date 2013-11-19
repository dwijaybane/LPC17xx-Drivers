/**********************************************************************
* $Id$		lpc_lcd.h
*//**
* @file		lpc_lcd.h
* @brief	Contains all LCD Initialization code
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
/** @defgroup LCD
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_LCD_H_
#define LPC_LCD_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_types.h"


#ifdef __cplusplus
extern "C"
{
#endif

/* Public Macros -------------------------------------------------------------- */
/** @defgroup LCD_Public_Macros LCD Public Macros
 * @{
 */

#ifndef ENABLE
#define	ENABLE		1
#endif
#ifndef DISABLE
#define DISABLE		0
#endif

/******************************************************************************/
/*                              LCD Commands                                  */
/******************************************************************************/
#define		LCD_CLEAR			0X01
#define		CURSOR_HOME			0x02

#define		LCD_OFF			    0X08
#define		LCD_ON			    0X0C
#define		CURSOR_ON			0X0E
#define		CURSOR_OFF			0X0C
#define		CURSOR_BLINK		0X0F

#define		LCD_SHIFT_LEFT		0x18
#define		LCD_SHIFT_RIGHT		0x1C
#define		CURSOR_SHIFT_LEFT	0X10
#define		CURSOR_SHIFT_RIGHT	0X14

#define		LCD_DEC_ADD_DISP_SHIFT_OFF  0x04   //decrement address & display shift off
#define		LCD_DEC_ADD_DISP_SHIFT_ON   0x05   //decrement address & display shift on
#define		LCD_INC_ADD_DISP_SHIFT_OFF  0X06   //increment address & display shift off
#define		LCD_INC_ADD_DISP_SHIFT_ON   0X0    //increment address & display shift on

#define		LCD_4BIT_1LINE		0x20		   //4-bit interface, 1 line,  5*7 Pixels
#define		LCD_4BIT_2LINE		0x28	  	   //4-bit interface, 2 lines, 5*7 Pixels
#define		LCD_8BIT_1LINE		0X30		   //8-bit interface, 1 line,  5*7 Pixels
#define		LCD_8BIT_2LINE		0X38		   //8-bit interface, 2 lines, 5*7 Pixels

/******************************************************************************/
/*                     LCD Enum Definition & Declaration                      */
/******************************************************************************/
typedef enum					// Use following notations in function where
{								// this enum is used as a parameter
    Dec = 0,
    Inc = 1
}AddrCount_e;

typedef enum
{
    DispShiftOff = 0,
    DispShiftOn = 1
}DispShift_e;

typedef enum
{
    Off = 0,
    On  = 1,
    Blink =2
}CursorType_e;

typedef enum
{
    Left = 1,
    Right = 2
}ShiftDir_e;


/******************************************************************************/
/*                              LCD Selection                                 */
/******************************************************************************/
#define 	LCD16X1			DISABLE			// Specify the type of LCD
#define 	LCD16X2			ENABLE
#define 	LCD16X4			DISABLE
#define 	LCD20X2			DISABLE
#define 	LCD20X4			DISABLE
#define 	LCD40X2			DISABLE
#define 	LCD40X4			DISABLE


/******************************************************************************/
/*                       LCD Selection Validation                             */
/******************************************************************************/
#if	LCD20X2|LCD20X4
	#define		ROWADDR1		0x80
	#define		ROWADDR2		0xC0
	#define		ROWADDR3		0x94
	#define		ROWADDR4		0xD4
	#define 	COLUMNSIZE		20
#elif LCD40X2|LCD40X4
	#define		ROWADDR1		0x80
	#define		ROWADDR2		0xC0
	#define		ROWADDR3		0x80
	#define		ROWADDR4		0xC0
	#define 	COLUMNSIZE		40
#elif (LCD16X1|LCD16X2|LCD16X4)
	#define		ROWADDR1		0x80
	#define		ROWADDR2		0xC0
	#define		ROWADDR3		0x90
	#define		ROWADDR4		0xD0
	#define 	COLUMNSIZE		16
#else
	#error LCD not selected
#endif


/******************************************************************************/
/*                       LCD Interface Mode                                   */
/******************************************************************************/
#define 	LCD_DATA_START_PIN 	0    // Specify the starting pin number of port

#define 	FOUR_BIT        DISABLE	 // Specify the type of interface
#define 	EIGHT_BIT       ENABLE

/******************************************************************************/
/*                       LCD Interface Mode validation                        */
/******************************************************************************/
#if (FOUR_BIT == EIGHT_BIT)
	#error Interface mode not correctly defined
#endif

#if FOUR_BIT
	#define LCD_4BIT
	#if (LCD_DATA_START_PIN > 4)
		#error Datalines not correctly defined
	#endif
#elif EIGHT_BIT
	#define LCD_8BIT
	#if (LCD_DATA_START_PIN > 0)
		#error Datalines not correctly defined
	#endif
#else
	#error Interface Mode not defined
#endif


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup LCD_Public_Functions LCD Public Functions
 * @{
 */

void Lcd_Init (AddrCount_e AddrCount, DispShift_e DispShift);
void Lcd_Enable (void);
void Write_Command_Lcd (uchar Command);
void Write_Data_Lcd (uchar Character);
void Set_Cursor (uchar LineNum, uchar Position, CursorType_e CursorType);
void Display_Character (uchar Character, uchar LineNum, uchar Position, CursorType_e CursorType);
void Display_String (uchar *String, uchar LineNum, uchar Position, CursorType_e CursorType);
void Display_Shift (ShiftDir_e Direction, CursorType_e CursorType);
void CGRAM_Char_Gen (uchar loc,uchar *p);



/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_LCD_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
