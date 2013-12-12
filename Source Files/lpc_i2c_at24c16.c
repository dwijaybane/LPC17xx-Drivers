/**********************************************************************
* $Id$		lpc_i2c_at24c16.c
*//**
* @file		lpc_i2c_at24c16.c
* @brief	Contains all functions support for I2C based E2PROM AT24C16
*           library on LPC17xx
* @version	1.0
* @date		11. Dec. 2013
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
/** @addtogroup EEPROM
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_i2c_at24c16.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */



/** @addtogroup EEPROM_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    Writes byte at given address
 * @param[in]	eep_address    Word Address range[0000 - 4000]
 * @param[in]   byte_data      Byte value
 * @return 		status
 **********************************************************************/
char I2C_Eeprom_Write (uint16 eep_address, uint8_t* byte_data, uint8_t length)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;
	uint8_t buf[length+2],i;

	buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
	buf[1] = (uchar) eep_address;        // 2nd byte extract

	for(i=2;i<10;i++)
	{
		buf[i]=*byte_data++;
	}
	txsetup.sl_addr7bit = E2P24C16_ID;
	txsetup.tx_data = buf;
	txsetup.tx_length = sizeof(buf);
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	/* write byte to addr  */
	if(I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING)==SUCCESS) //return status
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}


/*********************************************************************//**
 * @brief	    Reads byte from given address
 * @param[in]	eep_address    Word Address range[0000 - 4000]
 * @return 		Byte value
 **********************************************************************/
char I2C_Eeprom_Read (uint8_t* buf_data, uint8_t length)
{
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;
	uint8_t buf1[]={0x00,0x07};

	rxsetup.sl_addr7bit = E2P24C16_ID;
	rxsetup.tx_data = buf1;	// Get address to read at writing address
	rxsetup.tx_length = sizeof(buf1);
	rxsetup.rx_data = buf_data;
	rxsetup.rx_length = length;
	rxsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(LPC_I2C0, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		return (0);
	}
	else
	{
		return (-1);
	}
}

/*********************************************************************//**
 * @brief	    Display Read data stored in array
 * @param[in]   *dest_addr     buffer address
 * @param[in]   length         size of buffer
 * @return 		None
 **********************************************************************/
void Display_Eeprom_Array (uint8_t *string, uint8_t length)
{
	while(length)
	{
		printf(LPC_UART0,"%x02  ",*string++);
		length--;
	}
}


/*********************************************************************//**
 * @brief	    Ask for Address Range you want to display
 * @param[in]   *dest_addr     buffer address
 * @param[in]   length         size of buffer
 * @return 		None
 **********************************************************************/
void Display_Eeprom_Loc (uint16 mem_start_address, uint16 mem_end_address)
{
	uint8_t line=0,count=0;
	uint16_t dat;
	uint16_t addr;

	printf(LPC_UART0,"EEPROM Range = 0x000 - 0x7FF \r\n");

	clr_scr_rst_cur(LPC_UART0);
	printf(LPC_UART0,"Start: %x03   End: %x03 \r\n",mem_start_address,mem_end_address);

	for(addr=mem_start_address; addr<mem_end_address+1; addr++)
	{
		if(count == 0) printf(LPC_UART0,"%x03   ",addr);

	//	dat = I2C_Eeprom_Read_Byte(addr);   // read byte from address

		printf(LPC_UART0,"%x02  ",dat);
		count++;

		if(count == 16)                    // check for last digit entered
		{
			line++;
			count = 0;
			printf(LPC_UART0,"\r\n");
		}

		if(line == 20 || addr == mem_end_address)
		{
			printf(LPC_UART0,"\x1b[24;01HPress any key to continue.");
			line = 0;
			getche(LPC_UART0);

			clr_scr_rst_cur(LPC_UART0);
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

