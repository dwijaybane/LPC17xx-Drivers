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
uchar I2C_Eeprom_Write_Byte (uint16 eep_address, uint8_t byte_data)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;

	I2C_Tx_Buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
	I2C_Tx_Buf[1] = (uchar) eep_address;        // 2nd byte extract
	I2C_Tx_Buf[2] = byte_data;

	txsetup.sl_addr7bit = E2P24C16_ID;
	txsetup.tx_data = I2C_Tx_Buf;
	txsetup.tx_length = 3;
	txsetup.rx_data = NULL;
	txsetup.rx_length = 0;
	txsetup.retransmissions_max = 3;

	/* write byte to addr  */
	return I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING); //return status
}


/*********************************************************************//**
 * @brief	    Reads byte from given address
 * @param[in]	eep_address    Word Address range[0000 - 4000]
 * @return 		Byte value
 **********************************************************************/
uint8_t I2C_Eeprom_Read_Byte (uint16 eep_address)
{
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;

	I2C_Tx_Buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
	I2C_Tx_Buf[1] = (uchar) eep_address;        // 2nd byte extract

	rxsetup.sl_addr7bit = E2P24C16_ID;
	rxsetup.tx_data = I2C_Tx_Buf;	// Get address to read at writing address
	rxsetup.tx_length = 2;
	rxsetup.rx_data = I2C_Rx_Buf;
	rxsetup.rx_length = 1;
	rxsetup.retransmissions_max = 3;

	if (I2C_MasterTransferData(LPC_I2C0, &rxsetup, I2C_TRANSFER_POLLING) == SUCCESS)
	{
		return I2C_Rx_Buf[0];
	}
	else
	{
		return (0);
	}
}


/*********************************************************************//**
 * @brief	    Write value array at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @param[in]   data_start     address of buffer
 * @param[in]   length         size of buffer
 * @return 		status
 **********************************************************************/
uchar I2C_Eeprom_Write (uint16_t eep_address, uint8_t *data_start, uint8_t length)
{
	/* Transmit setup */
	I2C_M_SETUP_Type txsetup;

	uint8_t ip_len,new_length;
	uint16_t new_address,i;

	ip_len = 0x10 - (eep_address % 0x10);
	new_address = eep_address + ip_len;

	if(length > ip_len)
	{
		new_length = length - ip_len;

		I2C_Tx_Buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
		I2C_Tx_Buf[1] = (uchar) eep_address;        // 2nd byte extract

		for(i=2; i<(length+2); i++)
		{
			I2C_Tx_Buf[i]=data_start[i-2];
		}

		txsetup.sl_addr7bit = E2P24C16_ID;
		txsetup.tx_data = I2C_Tx_Buf;
		txsetup.tx_length = ip_len+2;
		txsetup.rx_data = NULL;
		txsetup.rx_length = 0;
		txsetup.retransmissions_max = 3;

		I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING);
		// Reccursive function
		I2C_Eeprom_Write (new_address,(data_start + ip_len),new_length);
	}

	if(length <= ip_len+1)
	{
		I2C_Tx_Buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
		I2C_Tx_Buf[1] = (uchar) eep_address;        // 2nd byte extract

		for(i=2; i<(length+2); i++)
		{
			I2C_Tx_Buf[i]=data_start[i-2];
		}

		txsetup.sl_addr7bit = E2P24C16_ID;
		txsetup.tx_data = I2C_Tx_Buf;
		txsetup.tx_length = length+2;
		txsetup.rx_data = NULL;
		txsetup.rx_length = 0;
		txsetup.retransmissions_max = 3;

		return I2C_MasterTransferData(LPC_I2C0, &txsetup, I2C_TRANSFER_POLLING);
	}
}


/*********************************************************************//**
 * @brief	    Reads array from given address
 * @param[in]	eep_address    Word Address range[0000 - 4000]
 * @param[in]   data_start     address of buffer
 * @param[in]   length         size of buffer
 * @return 		Byte value
 **********************************************************************/
void I2C_Eeprom_Read (uint16 eep_address, uint8_t *dest_addr, uint8_t length)
{
	/* Receive setup */
	I2C_M_SETUP_Type rxsetup;

	I2C_Tx_Buf[0] = (uchar)(eep_address>>8);    // 1st byte extract
	I2C_Tx_Buf[1] = (uchar) eep_address;        // 2nd byte extract

	rxsetup.sl_addr7bit = E2P24C16_ID;
	rxsetup.tx_data = I2C_Tx_Buf;	// Get address to read at writing address
	rxsetup.tx_length = 2;
	rxsetup.rx_data = dest_addr;
	rxsetup.rx_length = length;
	rxsetup.retransmissions_max = 3;

	I2C_MasterTransferData(LPC_I2C0, &rxsetup, I2C_TRANSFER_POLLING);
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

		dat = I2C_Eeprom_Read_Byte(addr);   // read byte from address

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

