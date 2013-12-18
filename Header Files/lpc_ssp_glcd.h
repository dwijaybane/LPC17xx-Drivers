/**********************************************************************
* $Id$		lpc_ssp_glcd.h
*//**
* @file		lpc_ssp_glcd.h
* @brief	Contains all SSP based GLCD Initialization code
* @version	1.0
* @date		18. Dec. 2013
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
/** @defgroup GLCD
 * @ingroup LPC1700CMSIS_FwLib_Drivers
 * @{
 */

#ifndef LPC_SSP_GLCD_H_
#define LPC_SSP_GLCD_H_

/* Includes ------------------------------------------------------------------- */
#include "LPC17xx.h"
#include "lpc_system_init.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* Public Macros -------------------------------------------------------------- */
/** @defgroup GLCD_Public_Macros
 * @{
 */
#define LCD_RS 		(1<<0)  //port2
#define	LCD_RST		(1<<5)	//port0
#define LCD_BK		(1<<8)	//port2


// color definitions
#define	BLACK	 	0x0000
#define	BLUE	  	0x001F
#define	RED 	  	0xF800
#define	GREEN   	0x07E0
#define CYAN	  	0x07FF
#define MAGENTA 	0xF81F
#define YELLOW  	0xFFE0
#define WHITE	  	0xFFFF


/**
 * @brief GLCD Driver Output Type definitions
 */
typedef enum {
	TOP_LEFT = 0,	/**< Polling transfer */
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT
} DRIVER_OUT_Type;


/**
 * @}
 */


/* Public Functions ----------------------------------------------------------- */
/** @defgroup GLCD_Public_Functions GLCD Public Functions
 * @{
 */

void GLCD_Backlight (FunctionalState NewState);
void GLCD_Reset(void);
void GLCD_Driver_OutCtrl(DRIVER_OUT_Type drv);
void GLCD_Display_Home (void);
void GLCD_Init (void);
uchar Write_Command_Glcd (uint8_t Command);
uchar Write_Data_Glcd (uint16_t data);
void Display_RGB (uint16_t data);
void GLCD_Test(void);


/**
 * @}
 */

#ifdef __cplusplus
}
#endif


#endif /* LPC_SSP_GLCD_H_ */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */
