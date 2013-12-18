/**********************************************************************
* $Id$		lpc_ssp_glcd.c
*//**
* @file		lpc_ssp_glcd.c
* @brief	Contains all functions support for SSP based GLCD
*           library on LPC17xx
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
/** @addtogroup GLCD
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_ssp_glcd.h"
#include "mario.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */



/** @addtogroup GLCD_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */

void GLCD_Backlight (FunctionalState NewState)
{
	if(NewState)
	{
		GPIO_SetValue(2, LCD_BK);
	}
	else
	{
		GPIO_ClearValue(2, LCD_BK);
	}
}

void GLCD_Reset(void)
{
	GPIO_SetValue(0, LCD_RST);
	delay_ms(3);
	GPIO_ClearValue(0, LCD_RST);  //reset low
	delay_ms(5);
	GPIO_SetValue(0, LCD_RST);  //reset low
}


void GLCD_Driver_OutCtrl(DRIVER_OUT_Type drv)
{
	Write_Command_Glcd(0x01);    // Driver Output Control

	switch (drv)
	{
	case TOP_LEFT:
		Write_Data_Glcd(0x72EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case TOP_RIGHT:
		Write_Data_Glcd(0x70EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case BOTTOM_LEFT:
		Write_Data_Glcd(0x32EF);       // Page 36-39 of SSD2119 datasheet
		break;

	case BOTTOM_RIGHT:
		Write_Data_Glcd(0x30EF);       // Page 36-39 of SSD2119 datasheet
		break;

	default:
		break;
	}
}

void GLCD_Display_Home (void)
{
	Write_Command_Glcd(0x4E);    // RAM address set
	Write_Data_Glcd(0x0000);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x4F);    // RAM address set
	Write_Data_Glcd(0x0000);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x44);    // Vertical RAM address position
	Write_Data_Glcd(0xEF00);       // Page 57 of SSD2119 datasheet

	Write_Command_Glcd(0x45);    // Horizontal RAM address position
	Write_Data_Glcd(0x0000);       // Page 57 of SSD2119 datasheet

	Write_Command_Glcd(0x46);    // Horizontal RAM address position
	Write_Data_Glcd(0x013F);       // Page 57 of SSD2119 datasheet

	Write_Command_Glcd(0x22);    // RAM data write/read
}

void GLCD_Init (void)
{
	GPIO_SetDir(2, LCD_RS, 1);   // RS as output
	GPIO_SetDir(0, LCD_RST, 1);  // Reset as Output
	GPIO_SetDir(2, LCD_BK, 1);   // Backlight as output

	GLCD_Reset();                // Reset GLCD

	Write_Command_Glcd(0x28);    // VCOM OTP
	Write_Data_Glcd(0x0006);       // Page 55-56 of SSD2119 datasheet

	Write_Command_Glcd(0x00);    // start Oscillator
	Write_Data_Glcd(0x0001);       // Page 36 of SSD2119 datasheet

	Write_Command_Glcd(0x10);    // Sleep mode
	Write_Data_Glcd(0x0000);       // Page 49 of SSD2119 datasheet

	GLCD_Driver_OutCtrl (TOP_LEFT);

	Write_Command_Glcd(0x02);    // LCD Driving Waveform Control
	Write_Data_Glcd(0x0600);       // Page 40-42 of SSD2119 datasheet

	Write_Command_Glcd(0x03);    // Power Control 1
	Write_Data_Glcd(0x6A38);       // Page 43-44 of SSD2119 datasheet

	Write_Command_Glcd(0x11);    // Entry Mode
	Write_Data_Glcd(0x6870);       // Page 50-52 of SSD2119 datasheet

	Write_Command_Glcd(0x0F);    // Gate Scan Position
	Write_Data_Glcd(0x0000);       // Page 49 of SSD2119 datasheet

	Write_Command_Glcd(0x0B);    // Frame Cycle Control
	Write_Data_Glcd(0x5308);       // Page 45 of SSD2119 datasheet

	Write_Command_Glcd(0x0C);    // Power Control 2
	Write_Data_Glcd(0x0003);       // Page 47 of SSD2119 datasheet

	Write_Command_Glcd(0x0D);    // Power Control 3
	Write_Data_Glcd(0x000A);       // Page 48 of SSD2119 datasheet

	Write_Command_Glcd(0x0E);    // Power Control 4
	Write_Data_Glcd(0x2E00);       // Page 48 of SSD2119 datasheet

	Write_Command_Glcd(0x1E);    // Power Control 5
	Write_Data_Glcd(0x00BE);       // Page 53 of SSD2119 datasheet

	Write_Command_Glcd(0x25);    // Frame Frequency Control
	Write_Data_Glcd(0x8000);       // Page 53 of SSD2119 datasheet

	Write_Command_Glcd(0x26);    // Analog setting
	Write_Data_Glcd(0x7800);       // Page 54 of SSD2119 datasheet

	Write_Command_Glcd(0x4E);    // Ram Address Set
	Write_Data_Glcd(0x0000);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x4F);    // Ram Address Set
	Write_Data_Glcd(0x0000);       // Page 58 of SSD2119 datasheet

	Write_Command_Glcd(0x12);    // Sleep mode
	Write_Data_Glcd(0x08D9);       // Page 49 of SSD2119 datasheet

	// Gamma Control (R30h to R3Bh) -- Page 56 of SSD2119 datasheet
	Write_Command_Glcd(0x30);
	Write_Data_Glcd(0x0000);

	Write_Command_Glcd(0x31);
	Write_Data_Glcd(0x0104);

	Write_Command_Glcd(0x32);
	Write_Data_Glcd(0x0100);

	Write_Command_Glcd(0x33);
	Write_Data_Glcd(0x0305);

	Write_Command_Glcd(0x34);
	Write_Data_Glcd(0x0505);

	Write_Command_Glcd(0x35);
	Write_Data_Glcd(0x0305);

	Write_Command_Glcd(0x36);
	Write_Data_Glcd(0x0707);

	Write_Command_Glcd(0x37);
	Write_Data_Glcd(0x0300);

	Write_Command_Glcd(0x3A);
	Write_Data_Glcd(0x1200);

	Write_Command_Glcd(0x3B);
	Write_Data_Glcd(0x0800);

	Write_Command_Glcd(0x07);      // Display Control
	Write_Data_Glcd(0x0033);       // Page 45 of SSD2119 datasheet

	delay_ms(5);

	Write_Command_Glcd(0x22);    // RAM data write/read
}


/*********************************************************************//**
 * @brief	    This function writes commands to the GLCD
 * @param[in]	Command		command to be written on GLCD
 * @return 		status
 **********************************************************************/
uchar Write_Command_Glcd (uint8_t Command)
{
	SSP_DATA_SETUP_Type xferConfig;
	uint8_t WriteStatus =0;

	GPIO_ClearValue(2, LCD_RS);  //select command mode

	CS_Force1 (LPC_SSP1, DISABLE);                        /* Select device           */
	xferConfig.tx_data = &Command;               /* Send Instruction Byte    */
	xferConfig.rx_data = NULL;
	xferConfig.length = 1;
	WriteStatus = SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force1 (LPC_SSP1, ENABLE);                          /* CS high inactive        */
		delay_ms(5);
		GPIO_SetValue(2, LCD_RS);  // select data mode
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    This function writes data to the GLCD
 * @param[in]	data	data to be written on GLCD
 * @return 		None
 **********************************************************************/
uchar Write_Data_Glcd (uint16_t data)
{
	SSP_DATA_SETUP_Type xferConfig;
	uint8_t WriteStatus =0;

	Tx_Buf1[0] = (uchar)(data>>8);    // 1st byte extract
	Tx_Buf1[1] = (uchar) data;        // 2nd byte extract

	GPIO_SetValue(2, LCD_RS);  // select data mode

	CS_Force1 (LPC_SSP1, DISABLE);                        /* Select device           */
	xferConfig.tx_data = Tx_Buf1;               /* Send Instruction Byte    */
	xferConfig.rx_data = NULL;
	xferConfig.length = 2;
	WriteStatus = SSP_ReadWrite(LPC_SSP1, &xferConfig, SSP_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force1 (LPC_SSP1, ENABLE);                          /* CS high inactive        */
		return(1);
	}
	else
		return(0);
}


void Display_RGB (uint16_t data)
{
	uint16_t i,j;
	GLCD_Display_Home();

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			Write_Data_Glcd(data);
		}
	}
}


void GLCD_Test(void)
{
	uint16_t i,j,k;
	k=16;

	GLCD_Display_Home();
	GLCD_Backlight(ENABLE);

	for(i=0;i<320;i++)
	{
		for(j=0;j<240;j++)
		{
			Write_Data_Glcd(image[k]);
			k++;
		}
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

