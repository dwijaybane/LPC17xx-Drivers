/**********************************************************************
* $Id$		lpc_spi_sd.c
*//**
* @file		lpc_spi_sd.c
* @brief	Contains all functions support for Spi based SD
*           library on LPC17xx
* @version	1.0
* @date		10. Dec. 2013
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
/** @addtogroup SD
 * @{
 */

/* Includes ------------------------------------------------------------------- */
#include "lpc_spi_sd.h"

/* If this source file built with example, the LPC17xx FW library configuration
 * file in each example directory ("lpc17xx_libcfg.h") must be included,
 * otherwise the default FW library configuration file must be included instead
 */


/** @addtogroup SD_Public_Functions
 * @{
 */

/* Public Functions ----------------------------------------------------------- */
/*********************************************************************//**
 * @brief		check if SD card is inserted or not
 * @param[in]	none
 * @return 		SD_CONNECTED or SD_DISCONNECTED
 **********************************************************************/
sd_connect_status SD_GetCardConnectStatus (void)
{
	sd_connect_status ret=SD_DISCONNECTED;

	if((GPIO_ReadValue(4)&_BIT(29))== 0)
		ret = SD_CONNECTED;

	return ret;
}


/*********************************************************************//**
 * @brief		Calculate CRC-7 as required by SD specification
 * @param[in]	- old_crc: 0x00 to start new CRC
 * 			    or value from previous call to continue.
 * 				- data: data byte to add to CRC computation
 * @return 		CRC-7 checksum which MUST be augmented by crc_7augment() before used
 **********************************************************************/
uint8_t CRC_7 (uint8_t old_crc, uint8_t data)
{
	uint8_t new_crc,x;

	new_crc = old_crc;
	for (x = 7; x >= 0; x--)
	{
		new_crc <<= 1;
		new_crc |= GETBIT(data,x);
		if (GETBIT(new_crc, 7) == 1)
		{
			new_crc ^= 0x89; /*CRC-7's polynomial is x^7 + x^3 + 1*/
		}
		if(x==0) break;
	}
	return new_crc;
}


/*********************************************************************//**
 * @brief		Provides the zero-padding final step to CRC-7
 * @param[in]	- old_crc: value from last crc_7()call
 * @return 		Finalized CRC-7 checksum
 **********************************************************************/
uint8_t CRC_7Final (uint8_t old_crc)
{
	uint8_t new_crc,x;

	new_crc = old_crc;
	for (x = 0; x < 7; x++)
	{
		new_crc <<= 1;
		if (GETBIT(new_crc, 7) == 1)
		{
			new_crc ^= 0x89; /*CRC-7's polynomial is x^7 + x^3 + 1*/
		}
	}
	return new_crc;
}


/*********************************************************************//**
 * @brief		Send/receive data over SPI bus
 * @param[in]	- tx_buf: pointer to transmit buffer.
 * 					      NULL if send 0xFF.
 * 				- rx_buf: pointer to receive buffer
 * 						  NULL if nothing to receive.
 * 			    - length: number of data to send or receive
 * @return 		the actual data sent or received.
 **********************************************************************/
uint32_t SD_SendReceiveData_Polling (void* tx_buf, void* rx_buf, uint32_t length)
{
	// SPI Data Setup structure variable
	SPI_DATA_SETUP_Type xferConfig;

	CS_Force(DISABLE);
	// delay_ms(1);

	xferConfig.tx_data = tx_buf;
	xferConfig.rx_data = rx_buf;
	xferConfig.length = length;
	SPI_ReadWrite(LPC_SPI, &xferConfig, SPI_TRANSFER_POLLING);

	// delay_ms(1);
	CS_Force(ENABLE);
	return xferConfig.counter;
}


/*********************************************************************//**
 * @brief		Send command to SD card
 * @param[in]	- cmd: SD command code
 * 			    - arg: pointer to array of 4x8 bytes, argument of command
 * @return 		n/a
 **********************************************************************/
void SD_SendCommand(uint8_t cmd, uint8_t *arg)
{
	uint8_t crc = 0x00;

	/* First byte has framing bits and command */
	sd_cmd_buf[0] = 0x40 | (cmd & 0x3f);
	sd_cmd_buf[1] = arg[0];
	sd_cmd_buf[2] = arg[1];
	sd_cmd_buf[3] = arg[2];
	sd_cmd_buf[4] = arg[3];
	//calculate CRC
	crc = CRC_7(crc, sd_cmd_buf[0]);//start new crc-7
	crc = CRC_7(crc, sd_cmd_buf[1]);
	crc = CRC_7(crc, sd_cmd_buf[2]);
	crc = CRC_7(crc, sd_cmd_buf[3]);
	crc = CRC_7(crc, sd_cmd_buf[4]);
	crc = CRC_7Final(crc);
	sd_cmd_buf[5] = (crc << 1) | 0x01;//stop bit

	SD_SendReceiveData_Polling(sd_cmd_buf,NULL,SD_CMD_BLOCK_LENGTH);
}


/*********************************************************************//**
 * @brief		Wait for SD card R1 response
 * @param[in]	- buffer: pointer to receive buffer
 * 			    - length: length of receive data, must equal 1+actual length of data
 * 					      length = 0 if receive R1 only
 * 				- timeout: timeout for retry
 * @return 		error code
 **********************************************************************/
sd_error SD_WaitR1 (uint8_t *buffer, uint32_t length, uint32_t timeout)
{
    uint32_t j;
	uint8_t dummy[2];
	uint8_t wait_idle;

	/* No null pointers allowed */
	if (buffer == NULL)	return SD_CMD_BAD_PARAMETER;

	/* Wait for start bit on R1 */
	j=0;dummy[0]=0xFF;
	while (GETBIT(dummy[0],7) == 1)
	{
		if (j>timeout)return SD_ERROR_TIMEOUT;
		dummy[0]=0x00;
		SD_SendReceiveData_Polling(NULL,dummy,1);
		j++;
	}
	*buffer=dummy[0];//store R1
	if (length > 0)//read followed data
	{
		/* Wait for start token on data portion, if any */
		dummy[0] = 0xff;
		j = 0;
		while (dummy[0] != 0xfe)
		{
			if (j > timeout)return SD_ERROR_TIMEOUT;
			dummy[0]=0x00;
			SD_SendReceiveData_Polling(NULL,dummy,1);
			if ((dummy[0] != 0xff) && (dummy[0] != 0xfe)) // not idle or start token?
			{
				return SD_ERROR_TOKEN;
			}
			j++;
		}
		/* Read all bytes */
		SD_SendReceiveData_Polling(NULL,(buffer+1),(length - 1));
	}
	/* Some more bit clocks to finish internal SD operations */
	dummy[0]=0x00;
	wait_idle=0;
	while((dummy[0]!=0xff)&&(wait_idle<20))
	{
		dummy[0]=0x00;
		SD_SendReceiveData_Polling(NULL,dummy,1);
		for(j=0;j<1000;j++);
		wait_idle++;
	}
	if(wait_idle>=20) return SD_ERROR_BUS_NOT_IDLE;
	return SD_OK;
}


/*********************************************************************//**
 * @brief		Wait for SD card idle
 * @param[in]	- num_char: number characters (8 bits clock) to wait
 * @return 		device already in idle state or need more time
 **********************************************************************/
sd_error SD_WaitDeviceIdle (uint32_t num_char)
{
	uint8_t dummy[2]={0,0};
	uint32_t i=0;

	while ((i < num_char) && (dummy[0] != 0xff))
	{
		dummy[0]=0x00;
		SD_SendReceiveData_Polling(NULL,dummy,1);
		if (dummy[0] == 0xff)
		{
			dummy[0]= 0x00;
			SD_SendReceiveData_Polling(NULL,dummy,1);
			if (dummy[0] == 0xff)
			{
				dummy[0]= 0x00;
				SD_SendReceiveData_Polling(NULL,dummy,1);
			}
		}
		i++;
	}
	if (dummy[0] != 0xff)return SD_ERROR_TIMEOUT;

	return SD_OK;
}


/*********************************************************************//**
 * @brief		Initialize SD card in SPI mode
 * @param[in]	- retries: number retry time
 * @return 		initialization successful or terminated with specific error code
 **********************************************************************/
sd_error SD_Init (uint8_t retries)
{
	uint8_t rxdata,errors;
	uint8_t SD_arg[4]={0,0,0,0};
	uint16_t i;

	// check for SD card insertion
	printf(LPC_UART0,"\n\rPlease plug-in SD card!");
	while(SD_GetCardConnectStatus()==SD_DISCONNECTED);
	printf(LPC_UART0,"...Connected!\n\r");
	// Wait for bus idle
	if(SD_WaitDeviceIdle(160) != SD_OK) return SD_ERROR_BUS_NOT_IDLE;
	printf(LPC_UART0,"Initialize SD card in SPI mode...");

	errors = 0;
	/* Send the CMD0_GO_IDLE_STATE while CS is asserted */
	/* This signals the SD card to fall back to SPI mode */
	while(errors < retries)
	{
		SD_SendCommand(CMD0_GO_IDLE_STATE, SD_arg);
		if(SD_WaitR1(&rxdata,0,1000)!= SD_OK)
		{
			errors++;
			continue;
		}
		if(rxdata != R1_IDLE)
		{
			errors++;
			continue;
		}
		else break;
	}
	if(errors >= retries)return SD_ERROR_CMD0;
    printf(LPC_UART0,"I have cleared errors");
	/* Check if the card is not MMC */
	/* Start its internal initialization process */
	while(1)
	{
		SD_SendCommand(CMD55_APP_CMD, SD_arg);
		if(SD_WaitR1(&rxdata,0,1000)!= SD_OK) return SD_ERROR_CMD55;

		SD_SendCommand(ACMD41_SEND_OP_COND, SD_arg);
		SD_WaitR1(&rxdata,0,1000);
		if (rxdata & R1_IDLE) //in_idle_state = 1
			for (i = 0; i < 1000; i++); /* wait for a while */
		else
			break; //in_idle_state=0 --> ready
	}
	printf(LPC_UART0,"I have cleared waiting");
	/* Enable CRC */
	SD_arg[3] = 0x01;
	SD_SendCommand(CMD59_CRC_ON_OFF, SD_arg);
	if(SD_WaitR1(&rxdata,0,1000)!= SD_OK) return SD_ERROR_CMD59;
	if(rxdata != R1_NOERROR) return SD_ERROR_CMD59;

	return SD_OK;
}


/*********************************************************************//**
 * @brief		Get SD card's CID register
 * @param[in]	none
 * @return 		OK or NG
 * 				OK: the returned data from SD card is stored in sd_data_buf
 **********************************************************************/
sd_error SD_GetCID (void)
{
	uint8_t SD_arg[4]={0,0,0,0};

	SD_SendCommand(CMD10_SEND_CID, SD_arg);
	if(SD_WaitR1(sd_data_buf,18,1000)!= SD_OK) return SD_NG;
	if(sd_data_buf[0]!= R1_NOERROR) return SD_NG;
	return SD_OK;
}


/*********************************************************************//**
 * @brief		Print Error Message
 * @param[in]	Print Message according to sd_status
 * @return 		None
 **********************************************************************/
void SD_ErrorMsg (sd_error sd_status)
{
	switch(sd_status)
	{
	case SD_ERROR_CMD0:
		printf(LPC_UART0,"Fail CMD0\n\r");
		break;

	case SD_ERROR_CMD55:
		printf(LPC_UART0,"Fail CMD55\n\r");
		break;

	case SD_ERROR_ACMD41:
		printf(LPC_UART0,"Fail ACMD41\n\r");
		break;

	case SD_ERROR_CMD59:
		printf(LPC_UART0,"Fail CMD59\n\r");
		break;

	case SD_ERROR_BUS_NOT_IDLE:
		printf(LPC_UART0,"Fail...Device is not in idle state.\n\r");
		break;

	case SD_OK:
		printf(LPC_UART0,"Done!\n\r");
		break;

	default:
		printf(LPC_UART0,"Fail\n\r");
		break;
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

