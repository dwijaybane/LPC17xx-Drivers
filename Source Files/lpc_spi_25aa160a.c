/**********************************************************************
* $Id$		lpc_spi_25aa160a.c
*//**
* @file		lpc_spi_25aa160a.c
* @brief	Contains all functions support for Spi based E2PROM 25aa160a
*           library on LPC17xx
* @version	1.0
* @date		09. Dec. 2013
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
#include "lpc_spi_25aa160a.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */

/** @addtogroup EEPROM_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief	    Get the status about locked or unlocked memory blocks.
 * @param[in]	None
 * @return 		status byte
 **********************************************************************/
uint8_t Spi_Eeprom_Read_Status_Reg (void)
{
	SPI_DATA_SETUP_Type xferConfig;

	Tx_Buf[0] = EEP_RDSR;                      /* Read Status 8bit msb    */
	CS_Force (DISABLE);                        /* Select device           */

	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
	xferConfig.tx_data = NULL;               /* Send Instruction Byte    */
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(Rx_Buf[0])
	{
		CS_Force (ENABLE);                     /* Select device           */
		return(Rx_Buf[0]);                    /* Return value            */
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    To lock or unlock data sectors
 * @param[in]	status_reg     Write Status Value
 * @return 		status byte
 **********************************************************************/
uchar Spi_Eeprom_Write_Status_Reg (uint8_t status_reg)
{
	SPI_DATA_SETUP_Type xferConfig;

	uint8_t WriteData[1],WriteStatus =0;

	Tx_Buf[0] = EEP_WRSR;                 /* Write Status 8bit msb                     */
	Tx_Buf[1] = status_reg;               /* STATUS REGISTER                           */
	                                       /* W/R                    W/R  W/R  R    R   */
                                           /* D7   D6   D5   D4      D3   D2   D1   D0  */
                                           /* WPEN X    X    X   --  BP1  BP0  WEL  WIP */

	WriteData[0] = EEP_WREN;

	CS_Force (DISABLE);                            /* Select device           */

	xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
	CS_Force (ENABLE);                          /* CS high inactive        */

	CS_Force (DISABLE);                            /* Select device           */
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.length = 2;
	WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force (ENABLE);                          /* CS high inactive        */
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Write Byte value at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @param[in]   byte_data      byte data to be written
 * @return 		status byte
 **********************************************************************/
uchar Spi_Eeprom_Write_Byte (uint16 eep_address, uint8_t byte_data)
{
	SPI_DATA_SETUP_Type xferConfig;
	uint8_t WriteData[1],WriteStatus =0;

	Tx_Buf[0] = EEP_WRITE;                  /* WRITE IR 8bit msb       */
	Tx_Buf[1] = (uchar)(eep_address>>8);    // 1st byte extract
	Tx_Buf[2] = (uchar) eep_address;        // 2nd byte extract
	Tx_Buf[3] = byte_data;                  /* byte data lsb           */

	WriteData[0] = EEP_WREN;

	CS_Force (DISABLE);
	xferConfig.tx_data = WriteData;            /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);  /* Write Enable Latch      */
	CS_Force (ENABLE);

	CS_Force (DISABLE);                              /* CS low active           */
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.length = 4;
	WriteStatus = SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(WriteStatus)
	{
		CS_Force (ENABLE);                            /* CS high inactive        */
		return(1);
	}
	else
		return(0);
}


/*********************************************************************//**
 * @brief	    Read Byte value at desired address(0x000 to 0x7FF)
 * @param[in]	eep_address    EEPROM 16bit address
 * @return 		Read Byte is returned
 **********************************************************************/

uint8_t Spi_Eeprom_Read_Byte (uint16 eep_address)
{
	SPI_DATA_SETUP_Type xferConfig;

	Tx_Buf[0] = EEP_READ;                 /* READ IR 8bit msb        */
	Tx_Buf[1] = (uchar)(eep_address>>8);  // 1st byte extract
	Tx_Buf[2] = (uchar)eep_address;       // 2nd byte extract

	CS_Force (DISABLE);
	xferConfig.tx_data = Tx_Buf;               /* Send Instruction Byte    */
	xferConfig.rx_data = Rx_Buf;               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 3;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);
	xferConfig.tx_data = NULL;
	xferConfig.rx_data = &Rx_Buf[0];               /* Store byte in Rx_Buf[0]  */
	xferConfig.length = 1;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	if(Rx_Buf[0])
	{
		CS_Force (ENABLE);
		return(Rx_Buf[0]);                    /* Return value            */
	}
	else
		return(0);
}


/**
 * @}
 */

/* End of Public Functions ---------------------------------------------------- */

/**
 * @}
 */

/* --------------------------------- End Of File ------------------------------ */

